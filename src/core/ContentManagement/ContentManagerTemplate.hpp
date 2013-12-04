#ifndef CORE_CONTENT_MANAGER_TEMPLATE_HPP
#define CORE_CONTENT_MANAGER_TEMPLATE_HPP

#include <functional>
#include <future>
#include <array>
#include <vector>
#include <chrono>
#include <cassert>
#include <cstring>

#include <Memory/MallocCounter.hpp>
#include <ContentManagement/TemplatePresence.hpp>
#include <ContentManagement/TemplateListIndex.hpp>
#include <ContentManagement/Loaders/BaseAssetLoader.hpp>

namespace Core
{    
    typedef void* AssetHandle;
    typedef std::vector<std::tuple<short, unsigned int, unsigned int, AssetHandle>, Core::MallocCounter<std::tuple<short, unsigned int, unsigned int, AssetHandle>>> AssetVector;
    typedef std::vector<std::tuple<unsigned int, std::future<void*>, std::function<void(Core::BaseAssetLoader*, AssetHandle)>>, Core::MallocCounter<std::tuple<short, unsigned int, unsigned int, std::future<void*>, std::function<void(Core::BaseAssetLoader*, AssetHandle)>>>> FinisherVector;

    template<typename... Loaders>
    class ContentManagerTemplate
    {
    public:
        ContentManagerTemplate()
        {
            m_loaders = {(new Loaders())...};
        }

        template<typename Loader>
        void Load( const char* asset, std::function<void(Core::BaseAssetLoader*)> finisher, const bool async = false )
        {
            static_assert(Core::Match<Loader, Loaders...>::exists, "Loader is not in loader list, add it to the ContentManager");           
            static const int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;            
            unsigned int assetHash = MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 

            AssetHandle handle;

            if(IsCached<Loader>(asset, handle))
            {                
                //Loading cached asset synchronous
                finisher(m_loaders[loaderId]->GetReader(handle));
                IncreaseReference<Loader>(assetHash);
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
                    AssetHandle handle;
                    bool result = m_loaders[loaderId]->Load(asset, handle);                                
                    
                    //TODO::Add log message
                    assert(result);
                   
                    AddReference(assetHash, handle);
                    finisher(m_loaders[loaderId]->GetReader(handle));
                }
            }
        }

        template<typename Loader>
        void Free(const char* asset)
        {            
            static_assert(Core::Match<Loader, Loaders...>::exists, "Loader is not in loader list, add it to the ContentManager");           
            static const int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;
            unsigned int assetHash = MurmurHash2(asset, static_cast<int>(std::strlen(asset)), loaderId); 
            
            AssetHandle handle;

            if(IsCached(assetHash, handle))
            {
                if(DecreaseReference<Loader>(assetHash) == 0)
                {                     
                    std::promise<AssetHandle> promise;
                    
                    m_finisherList.push_back(std::make_tuple(Core::Index<Loader, std::tuple<Loaders...>>::value, promise.get_future(),[](Core::BaseAssetLoader* assetLoader, AssetHandle handle)
                        {   
                            assetLoader->Destroy(handle); 
                        }));
                    promise.set_value(handle);
                }
            }
            else
            {
                //TODO: Add log message
                assert(false);
            }
        }


        void CallFinishers()
        {
            for(FinisherVector::iterator it = m_finisherList.begin(); it != m_finisherList.end(); ++it)
            {
                if(std::get<1>(*it).wait_for(std::chrono::seconds(0)) == std::future_status::ready)
                {
                    std::get<2>(*it)(m_loaders[std::get<0>(*it)], std::get<1>(*it).get());
                }
            }
            m_finisherList.clear();
        }
    private:
        template<typename Loader>
        bool IsCached( const unsigned int assetHash, AssetHandle handle )
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
            //TODO: Add log message
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
                    return std::get<0>(*it) = std::get<0>(*it) ;
                }
            }
            
            //TODO: Add log message
            assert(false);
        }

        template<typename Loader>
        void AddReference( const unsigned int assetHash, AssetHandle handle)
        {
            m_loaders.push_back(std::make_tuple(1, Core::Index<Loader, std::tuple<Loaders...>>::value, assetHash, handle));            
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

        //|-----------------Key--------------------|---Value-----|
        // Users, Loader index, Hash of asset name, AssetHandle
        AssetVector m_assetList;
        FinisherVector m_finisherList;

    };
}

#endif
