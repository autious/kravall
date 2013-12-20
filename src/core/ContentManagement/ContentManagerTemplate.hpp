#ifndef CORE_CONTENT_MANAGER_TEMPLATE_HPP
#define CORE_CONTENT_MANAGER_TEMPLATE_HPP

#include <functional>
#include <future>
#include <mutex>
#include <array>
#include <vector>
#include <tuple>
#include <chrono>
#include <cassert>
#include <cstring>

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>
#include <ContentManagement/LoaderIncludes.hpp>
#include <ContentManagement/MurmurHash.hpp>

#include <ComponentFramework/CheckTemplatePresence.hpp>
#include <ComponentFramework/ComponentType.hpp>

#include <logger/Logger.hpp>

#define LOADER_ERROR_MESSAGE "Loader is not in loader list, add it to the ContentManager in ContentManager.hpp"

#define LOGCM_DEBUG	LogSystem::LogData( LogSystem::LogType::logType_debug,		"cm_debug" )
#define LOGCM_INFO    LogSystem::LogData( LogSystem::LogType::logType_debug,    "cm_info" )
#define LOGCM_FATAL	LogSystem::LogData( LogSystem::LogType::logType_fatal,		"cm_fatal" )
#define LOGCM_ERROR	LogSystem::LogData( LogSystem::LogType::logType_error,		"cm_error" )
#define LOGCM_WARNING	LogSystem::LogData( LogSystem::LogType::logType_warning,	"cm_warning" )

namespace Core
{    
    typedef unsigned int AssetHash;

    template<typename... Loaders>
    class ContentManagerTemplate
    {
    public:
        /*!
            Constructor for ContentManagerTemplate.
          */
        ContentManagerTemplate()
        {
            m_loaders = {{(new Loaders())...}};
        }
        
        /*!
            Destructor for ContentManagerTemplate.            
          */           
        ~ContentManagerTemplate()
        {
            for(int i=0; i < LOADER_COUNT; ++i)
            {
                delete m_loaders[i];
            }
        }

        /*!
            Loads an asset with the template parameter loader. When the asset is loaded the supplied lambda function will be called with the asset handleand loader. Set async to true to enable asynchronous asset loading. Synchronous loading guarantees that the asset will be loaded before returning.
            \param asset The asset name as a null terminated cstring.
            \param finisher Function to be called when the asset is done loading. Asset data can be applied to state in the function.
            \param async Flag for asynchronous loading. Set to true to enable asynchronous loading. Defaults to false. 
          */
        template<typename Loader>
        void Load( const char* asset, std::function<void(Core::BaseAssetLoader*, Core::AssetHandle assetHandle)> finisher, const bool async = false )
        {
            static_assert(Core::Match<Loader, Loaders...>::exists, LOADER_ERROR_MESSAGE);           
            static const unsigned int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;            
            AssetHash assetHash = MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 
            Core::AssetHandle handle = nullptr;

            Core::AssetStatus status = GetAssetStatus(loaderId, assetHash, handle);

            if(status == Core::AssetStatus::CACHED)
            {                
                //Loading cached asset synchronous
                LOGCM_INFO << "Loading cached asset: " << asset << " now has: " <<
                    IncreaseReference(loaderId, assetHash) << " references" << std::endl;
                finisher(m_loaders[loaderId], handle);
            }
            else if(status == Core::AssetStatus::UNCACHED)
            {
                if(async)
                {
                    //Loading uncached asset asynchronous.
                    LOGCM_INFO << "Loading uncached asset: " << asset << " asynchronous." << std::endl; 
                    std::shared_ptr<std::mutex> asyncMutex = std::make_shared<std::mutex>();
                    std::shared_ptr<Core::AssetHandle> asyncHandle = std::make_shared<Core::AssetHandle>();

                    asyncMutex->lock();
                    m_asyncList.push_back(std::make_tuple(loaderId, assetHash, asyncMutex, asyncHandle, finisher));

                    std::thread([asset, asyncMutex, asyncHandle, this](unsigned int loaderId)                           
                            {
                                (*asyncHandle) = this->m_loaders[loaderId]->LoadAsync(asset);
                                asyncMutex->unlock();
                            }, loaderId).detach();

                    AddReference<Loader>(assetHash, *asyncHandle, Core::AssetStatus::LOADING);
                }
                else
                {
                    //Loading uncached asset synchronous.
                    LOGCM_INFO << "Loading uncached asset: " << asset << " synchronous." << std::endl;    

                    handle = m_loaders[loaderId]->Load(asset);                                
                    
                    if(handle == nullptr)
                    {                        
                        LOGCM_FATAL << "Asset with name: " << asset << " was not found" << std::endl;
                        assert(handle != nullptr);
                    }

                    AddReference<Loader>(assetHash, handle, Core::AssetStatus::CACHED);
                    finisher(m_loaders[loaderId], handle);
                }
            }
            else
            {
                if(async)
                {
                    //Loading an asset that is currently being loaded asynchoronous.
                    bool asyncAssetFound = false;
                    for(Core::AsyncVector::iterator it = m_asyncList.begin(); it != m_asyncList.end(); ++it)
                    {
                        if(std::get<0>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value && 
                                std::get<1>(*it) == assetHash)
                        {
                            m_asyncList.push_back(std::make_tuple(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it), nullptr, finisher));
                            asyncAssetFound = true;
                            LOGCM_INFO << "Loading currently loading asset: " << asset << " asynchronous, now has: "
                                << IncreaseReference(loaderId, assetHash) << " references" << std::endl;
                            break;
                        }
                    }

                    if(asyncAssetFound == false)
                    {
                        LOGCM_FATAL << "Asset with name: " << asset << " was not found in asynchronous loading assets" << std::endl;
                        assert(asyncAssetFound);
                    }
                }
                else
                {
                    //Loading an asset that is currently beign loaded synchronous.
                    bool asyncAssetFound = false;
                    for(Core::AsyncVector::size_type i = 0; i < m_asyncList.size(); ++i)
                    {
                        if(std::get<0>(m_asyncList[i]) == loaderId && std::get<1>(m_asyncList[i]) == assetHash)
                        {
                            std::get<2>(m_asyncList[i])->lock();
                            asyncAssetFound = true;
                            Core::AssetHandle handle = (*std::get<3>(m_asyncList[i]));

                            m_loaders[std::get<0>(m_asyncList[i])]->FinishLoadAsync(handle);
                            SetAssetHandle(std::get<0>(m_asyncList[i]), std::get<1>(m_asyncList[i]), handle);

                            m_finisherList.push_back(std::make_tuple(m_loaders[std::get<0>(m_asyncList[i])], handle, std::get<4>(m_asyncList[i])));
                            SetAssetStatus(std::get<0>(m_asyncList[i]), std::get<1>(m_asyncList[i]), Core::AssetStatus::CACHED);

                            m_asyncList.erase(m_asyncList.begin()+i);
                            i=-1;
                            std::get<2>(m_asyncList[i])->unlock();                 

                            finisher(m_loaders[loaderId], handle);
                            IncreaseReference(loaderId, assetHash);
                            LOGCM_INFO << "Loading currently loading asset: " << asset << " synchronous, now has: "
                                << IncreaseReference(loaderId, assetHash) << " references" << std::endl;
                        }
                    }

                    if(asyncAssetFound == false)
                    {
                        LOGCM_FATAL << "Asset with name: " << asset << " was not found in asynchronous loading assets" << std::endl;
                        assert(asyncAssetFound);
                    }
                }
            }
        }

        template<typename Loader>
        AssetHash GetHash( const char * asset )
        {
            static const unsigned int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;
            return MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 
        }

        /*!
            Reduces the internal counter of the asset with template parameter loader. When the internal counter reaches zero the asset is destroyed.
            \param asset The asset name as a null terminated cstring.
          */
        template<typename Loader>
        void Free( const AssetHash assetHash )
        {
            static_assert(Core::Match<Loader, Loaders...>::exists, "Loader is not in loader list, add it to the ContentManager");           
            static const unsigned int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;
            
            Core::AssetHandle handle;

            Core::AssetStatus status = GetAssetStatus(loaderId, assetHash, handle);

            int refsRemaining = DecreaseReference<Loader>(assetHash);
            LOGCM_INFO << assetHash
                << " now has: " << refsRemaining << " references" << std::endl;
            if( refsRemaining == 0)
            {
                if(status == Core::AssetStatus::CACHED)
                {
                    LOGCM_INFO << "Adding destroying finisher for asset: " << assetHash << std::endl;
                        
                    m_finisherList.push_back(std::make_tuple(m_loaders[Core::Index<Loader, std::tuple<Loaders...>>::value]
                                , handle, [assetHash, this](Core::BaseAssetLoader* assetLoader, AssetHandle handle)
                        {
                            if(this->GetReferenceCount<Loader>(assetHash) == 0)
                            {
                                LOGCM_INFO << "Destroying asset with hash: " << assetHash << std::endl;
                                assetLoader->Destroy(handle); 
                                this->RemoveReference<Loader>(assetHash);
                            }
                            else
                            {
                                LOGCM_INFO << "Prevented destruction of asset with hash: " << assetHash << std::endl;
                            }
                        }));
                }
                else if(status == Core::AssetStatus::LOADING)
                {
                    Core::AssetHandle handle = nullptr;
                    LOGCM_INFO << "Destroying loading asset: " << assetHash << std::endl;

                    for(Core::AsyncVector::size_type i = 0; i < m_asyncList.size(); ++i)
                    {    
                        if(loaderId == std::get<0>(m_asyncList[i]) && assetHash == std::get<1>(m_asyncList[i]))
                        {
                            std::get<2>(m_asyncList[i])->lock();

                            if(GetAssetStatus(loaderId, assetHash, handle) == Core::AssetStatus::LOADING)
                            {
                                handle = (*std::get<3>(m_asyncList[i]));
                                m_loaders[loaderId]->FinishLoadAsync(handle);
                                SetAssetHandle(loaderId, assetHash, handle);
                            }
                            else
                            {
                                handle = GetAssetHandle(loaderId, assetHash);
                            }

                            m_finisherList.push_back(std::make_tuple(m_loaders[loaderId], handle, std::get<4>(m_asyncList[i])));
                            std::get<2>(m_asyncList[i])->unlock();                  
                            SetAssetStatus(loaderId, assetHash, Core::AssetStatus::CACHED);
                            m_asyncList.erase(m_asyncList.begin()+i);
                            i=-1;
                        }
                    }

                    for(Core::FinisherVector::iterator it = m_finisherList.begin(); it != m_finisherList.end(); ++it)
                    {
                        std::get<2>(*it)(std::get<0>(*it), std::get<1>(*it));
                    }
                    m_finisherList.clear();
                    
                    m_loaders[loaderId]->Destroy(handle);
                    RemoveReference<Loader>(assetHash);
                }
                else
                {
                   LOGCM_FATAL << "Tryign to free unexisting asset: " << assetHash << std::endl; 
                   assert(false);
                }
            }
        }

        /*!
            Reduces the internal counter of the asset with template parameter loader. When the internal counter reaches zero the asset is destroyed.
            \param asset The asset name as a null terminated cstring.
          */
        template<typename Loader>
        void Free(const char* asset)
        {            
            static const unsigned int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;
            AssetHash assetHash = MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 
            LOGCM_INFO << "Reducing reference of asset: " << asset << "..." << std::endl;
            Free<Loader>(assetHash);
        }
        
        /*!
          Checks if asynchrounous content has been loaded and calls the user specified application functions if it has. 
          Also calls removal and finishing functions for loaders. 
          This function should only be called from the thread which has an active rendering context.
        */
        void CallFinishers()
        {
            for(Core::AsyncVector::size_type i = 0; i < m_asyncList.size(); ++i)
            {
                if(std::get<2>(m_asyncList[i])->try_lock())
                {
                    unsigned int loaderId = std::get<0>(m_asyncList[i]);
                    AssetHash assetHash = std::get<1>(m_asyncList[i]);
                    Core::AssetHandle handle;

                    if(GetAssetStatus(loaderId, assetHash, handle) == Core::AssetStatus::LOADING)
                    {
                        handle = (*std::get<3>(m_asyncList[i]));
                        m_loaders[loaderId]->FinishLoadAsync(handle);
                        SetAssetHandle(loaderId, assetHash, handle);
                    }
                    else
                    {
                        handle = GetAssetHandle(loaderId, assetHash);
                    }
                    m_finisherList.push_back(std::make_tuple(m_loaders[loaderId], handle, std::get<4>(m_asyncList[i])));
                    std::get<2>(m_asyncList[i])->unlock();                  
                    SetAssetStatus(loaderId, assetHash, Core::AssetStatus::CACHED);
                    m_asyncList.erase(m_asyncList.begin()+i);
                    i=-1;
                }
            }

            for(Core::FinisherVector::iterator it = m_finisherList.begin(); it != m_finisherList.end(); ++it)
            {
                std::get<2>(*it)(std::get<0>(*it), std::get<1>(*it));
            }
            m_finisherList.clear();
        }

    private:
        AssetStatus GetAssetStatus(const unsigned int loaderId, const AssetHash assetHash, AssetHandle& handle )
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == loaderId && std::get<2>(*it) == assetHash)
                {
                    handle = std::get<3>(*it);
                    return std::get<4>(*it);
                }
            }
            return Core::AssetStatus::UNCACHED;
        }
        
        void SetAssetStatus(const unsigned int loaderId, const AssetHash assetHash, Core::AssetStatus assetStatus)
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == loaderId && std::get<2>(*it) == assetHash)
                {
                    std::get<4>(*it) = assetStatus;
                }
            }
        }

        unsigned int IncreaseReference( const unsigned int loaderId, const AssetHash assetHash )
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.cend(); ++it)
            {
                if( std::get<1>(*it) == loaderId && std::get<2>(*it) == assetHash)
                {
                   return std::get<0>(*it) = std::get<0>(*it) + 1;
                }
            }           
            LOGCM_FATAL << "Trying to increase reference of unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
            return -1;
        }

        template<typename Loader>
        unsigned int GetReferenceCount( const AssetHash assetHash)
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.cend(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                   return std::get<0>(*it);
                }
            }           

            LOGCM_FATAL << "Trying to get reference count of unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
            return -1;
        }


        template<typename Loader>
        unsigned int DecreaseReference( const AssetHash assetHash )
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                    return std::get<0>(*it) = std::get<0>(*it) - 1;
                }
            }
            
            LOGCM_FATAL << "Trying to decrease reference of unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
            return -1;
        }

        template<typename Loader>
        void AddReference( const AssetHash assetHash, AssetHandle handle, Core::AssetStatus assetStatus)
        {
            const int id = Core::Index<Loader, std::tuple<Loaders...>>::value;
            m_assetList.push_back(std::make_tuple(1, id, assetHash, handle, assetStatus));
        }

        template<typename Loader>
        void RemoveReference( const AssetHash assetHash )
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                    m_assetList.erase(it);
                    return;
                }
            }

            LOGCM_FATAL << "Trying to remove unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
        }

        template<typename Loader>
        std::shared_ptr<std::mutex> GetAsyncMutex(AssetHash assetHash)
        {
            for(Core::AsyncVector::iterator it = m_asyncList.begin(); it != m_asyncList.end(); ++it)
            {
                if( std::get<0>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<1>(*it) == assetHash)
                {
                    return std::get<2>(*it);
                }
            }

            LOGCM_FATAL << "Trying to retrieve lock of non-loading asset with hash: " << assetHash << std::endl;
            assert(false);
            return nullptr;
        }

        Core::AssetHandle GetAssetHandle(const unsigned int loaderId, const AssetHash assetHash) const
        {
            for(Core::AssetVector::const_iterator it = m_assetList.cbegin(); it != m_assetList.cend(); ++it)
            {
                if( std::get<1>(*it) == loaderId && std::get<2>(*it) == assetHash)
                {
                    return std::get<3>(*it); 
                }
            }

            LOGCM_FATAL << "Trying to retrieve handle of unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
            return nullptr;
        }

        void SetAssetHandle(const unsigned int loaderId, const AssetHash assetHash, Core::AssetHandle handle)
        {
            for(Core::AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == loaderId && std::get<2>(*it) == assetHash)
                {
                    std::get<3>(*it) = handle; 
                    break;
                }
            }
        }


        static const int LOADER_COUNT = sizeof...(Loaders);
        std::array<Core::BaseAssetLoader*, LOADER_COUNT> m_loaders;

        Core::AssetVector m_assetList;
        Core::AsyncVector m_asyncList;
        Core::FinisherVector m_finisherList;
    };
}

#endif
