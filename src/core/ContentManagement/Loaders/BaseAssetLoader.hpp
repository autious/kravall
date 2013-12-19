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

        virtual Core::AssetHandle Load(const char* assetName) = 0;
        virtual Core::AssetHandle LoadAsync(const char* assetname) = 0;
        virtual void FinishLoadAsync(Core::AssetHandle& handle) = 0;
        virtual void Destroy(const Core::AssetHandle handle) = 0;
    private:

    };
}


#endif
