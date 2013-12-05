#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_BASE_ASSET_LOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_BASE_ASSET_LOADER_HPP

#include <mutex>

#include <ContentManagement/Defines.hpp>

namespace Core
{
    class BaseAssetLoader
    {
    public:
        BaseAssetLoader();
        virtual ~BaseAssetLoader();

        virtual void* Load(const char* assetName) = 0;
        virtual void* LoadAsync(const char* assetname, Core::FinisherVector& finisher, std::mutex& finisherMutex) = 0;
        virtual void Destroy(const Core::AssetHandle handle) = 0;
    private:

    };
}


#endif
