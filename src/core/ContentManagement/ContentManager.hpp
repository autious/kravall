#ifndef CORE_CONTENT_MANAGER_HPP
#define CORE_CONTENT_MANAGER_HPP

#include <functional>
#include <future>
#include <array>
#include <cassert>

#include <mem/LinearHeap.hpp>
#include <ContentManagement/TemplatePresence.hpp>
#include <ContentManagement/TemplateListIndex.hpp>
#include <Logger/Logger.hpp>

namespace Core
{
    
    typedef void* ContentHandle;

    template<typename... Loaders>
    class ContentManager
    {
    public:
        ContentManager(Core::LinearHeap contentMemory)
        {
            m_contentMemory = contentMemory;
            m_Loaders = {(new Loaders())...};
        }

        template<typename Loader>
        ContentHandle Load( const char* asset, std::function(void(std::decltype(T->GetReader()))) finisher, const bool async = false )
        {
            static_assert( Core::Match::<Loader, Loaders...>::exists, "Loader is not in loader list, add it to the Content Manager" );           
            static const int loaderId = Core::Index<Loader, std::tuple<Loaders...>>::value;
            
            ContentHandle handle = m_loaders[loaderId]->GenerateHandle();

            if(IsCached<Loader>(asset, handle)
            {
                finisher(Loader->GetReader(handle));
            }
            else
            {
                if(async)
                {

                }
                else
                {
                    bool result = m_loaders[loaderId]->Load(asset, handle);                                
                    
                    assert(result, std::string("Failed to load asset: ") + std::string(asset) );

                    finisher(m_loaders[loaderId]->GetReader(handle));
                }
            }
            return handle;
        }

    private:
        template<typename Loader>
        bool IsCached( const char* asset, ContentHandle handle )
        {
            
        }



        static const int LOADER_COUNT = sizeof...(Loaders);
        std::array<Core::BaseLoader*, LOADER_COUNT> m_loaders;

        Core::LinearHeap m_contentMemory;
        int m_numberOfFinishers;

    };
}

#endif
