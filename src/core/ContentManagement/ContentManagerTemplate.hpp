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

#include <ComponentFramework/CheckTemplatePresence.hpp>
#include <ComponentFramework/ComponentType.hpp>

#include <logger/Logger.hpp>

#define LOADER_ERROR_MESSAGE "Loader is not in loader list, add it to the ContentManager in ContentManager.hpp"

namespace Core
{    
    template<typename... Loaders>
    class ContentManagerTemplate
    {
    public:
        ContentManagerTemplate()
        {
            m_loaders = {{(new Loaders())...}};
        }
        
        ~ContentManagerTemplate()
        {
            for(int i=0; i < LOADER_COUNT; ++i)
            {
                delete m_loaders[i];
            }
        }

        template<typename Loader>
        void Load( const char* asset, std::function<void(Core::BaseAssetLoader*, Core::AssetHandle assetHandle)> finisher, const bool async = false )
        {
            static_assert(Core::Match<Loader, Loaders...>::exists, LOADER_ERROR_MESSAGE);           
            static const int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;            
            unsigned int assetHash = MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 
            Core::AssetHandle handle = nullptr;

            if(IsCached<Loader>(assetHash, handle))
            {                
                //Loading cached asset synchronous
                LOG_INFO << "Loading cached asset: " << asset 
                    << " now has: " << IncreaseReference<Loader>(assetHash) << " references" << std::endl;
                finisher(m_loaders[loaderId], handle);
            }
            else
            {
                if(async)
                {
                    //Loading un-cached asset asynchronous.
                }
                else
                {
                    //Loading un-cached asset synchronous.
                    handle = m_loaders[loaderId]->Load(asset);                                
                    
                    if(handle == nullptr)
                    {                        
                        LOG_FATAL << "Asset: " << asset << " not found" << std::endl;
                        assert(false);
                    }

                    AddReference<Loader>(assetHash, handle);
                    finisher(m_loaders[loaderId], handle);
                }
            }
        }

        template<typename Loader>
        void Free(const char* asset)
        {            
            static_assert(Core::Match<Loader, Loaders...>::exists, "Loader is not in loader list, add it to the ContentManager");           
            static const int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;
            unsigned int assetHash = MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 
            
            Core::AssetHandle handle;

            if(IsCached<Loader>(assetHash, handle))
            {
                int refsRemaining = DecreaseReference<Loader>(assetHash);
                LOG_INFO << "Reducing reference of cached asset: " << asset
                    << " now has: " << refsRemaining << " references" << std::endl;
                if( refsRemaining == 0)
                {
                    LOG_INFO << "Adding destroying finisher for asset: " << asset 
                        << " with handle: " << std::hex << handle << std::endl;
                    
                    m_finisherList.push_back(std::make_tuple(m_loaders[Core::Index<Loader, std::tuple<Loaders...>>::value]
                                , handle, [](Core::BaseAssetLoader* assetLoader, AssetHandle handle)
                        {   
                            LOG_INFO << "Destroying asset with handle: " << std::hex << handle << std::endl;
                            assetLoader->Destroy(handle); 
                        }));
                }
            }
            else
            {
                LOG_FATAL << "Trying to Free unexsisting asset: " << asset << std::endl;
                assert(false);
            }
        }
        
        /*!
          Checks if asynchrounous content has been loaded and calls the user specified application functions if it has. 
          Also calls removal and finishing functions for loaders. 
          This function should only be called from the thread which has an active rendering context.
        */

        void CallFinishers()
        {
            std::lock_guard<std::mutex> load(m_finisherLock);
            for(AsyncVector::iterator it = m_asyncList.begin(); it != m_asyncList.end(); ++it)
            {
                if(std::get<1>(*it).wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                {
                    m_finisherList.push_back(std::make_tuple(m_loaders[std::get<0>(*it)], std::get<1>(*it).get(), std::get<2>(*it)));
                    m_asyncList.erase(it);
                    it = m_asyncList.begin();
                }
            }

            for(FinisherVector::iterator it = m_finisherList.begin(); it != m_finisherList.end(); ++it)
            {
                std::get<2>(*it)(std::get<0>(*it), std::get<1>(*it));
            }
            m_finisherList.clear();
        }

    private:
        template<typename Loader>
        bool IsCached( const unsigned int assetHash, AssetHandle& handle )
        {
            for(AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                    handle = std::get<3>(*it);
                    return true;
                }
            }
            return false;
        }

        template<typename Loader>
        unsigned int IncreaseReference( const unsigned int assetHash )
        {
            for(AssetVector::iterator it = m_assetList.begin(); it != m_assetList.cend(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                   return std::get<0>(*it) = std::get<0>(*it) + 1;
                }
            }           
            LOG_FATAL << "Trying to increase reference of unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
        }


        template<typename Loader>
        unsigned int DecreaseReference( const unsigned int assetHash )
        {
            for(AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                    return std::get<0>(*it) = std::get<0>(*it) - 1;
                }
            }
            
            LOG_FATAL << "Trying to decrease reference of unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
        }

        template<typename Loader>
        void AddReference( const unsigned int assetHash, AssetHandle handle)
        {
            const int id = Core::Index<Loader, std::tuple<Loaders...>>::value;
            m_assetList.push_back(std::make_tuple(1, id, assetHash, handle));            
        }

        template<typename Loader>
        void RemoveReference( const unsigned int assetHash )
        {
            for(AssetVector::iterator it = m_assetList.begin(); it != m_assetList.end(); ++it)
            {
                if( std::get<1>(*it) == Core::Index<Loader, std::tuple<Loaders...>>::value 
                    && std::get<2>(*it) == assetHash)
                {
                    m_assetList.erase(it);
                    return;
                }
            }

            LOG_FATAL << "Trying to remove unexisting asset with hash: " << assetHash << std::endl;
            assert(false);
        }

        static unsigned int MurmurHash2 ( const void* key, int len, unsigned int seed )
        {
            // 'm' and 'r' are mixing constants generated offline.
            // They're not really 'magic', they just happen to work well.

            const unsigned int m = 0x5bd1e995;
            const int r = 24;

            // Initialize the hash to a 'random' value

            unsigned int h = seed ^ len;

            // Mix 4 bytes at a time into the hash

            const unsigned char * data = (const unsigned char *)key;

            while(len >= 4)
            {
                unsigned int k = *(unsigned int *)data;

                k *= m; 
                k ^= k >> r; 
                k *= m; 
                
                h *= m; 
                h ^= k;

                data += 4;
                len -= 4;
            }
            
            // Handle the last few bytes of the input array

            switch(len)
            {
            case 3: h ^= data[2] << 16;
            case 2: h ^= data[1] << 8;
            case 1: h ^= data[0];
                    h *= m;
            };

            // Do a few final mixes of the hash to ensure the last few
            // bytes are well-incorporated.

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h;
        }

        static const int LOADER_COUNT = sizeof...(Loaders);
        std::array<Core::BaseAssetLoader*, LOADER_COUNT> m_loaders;
        std::mutex m_finisherLock;

        AssetVector m_assetList;
        AsyncVector m_asyncList;
        FinisherVector m_finisherList;

    };
}

#endif
