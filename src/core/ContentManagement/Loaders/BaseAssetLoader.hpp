#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_BASE_ASSET_LOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_BASE_ASSET_LOADER_HPP

namespace Core
{
    class BaseAssetLoader
    {
    public:
        BaseAssetLoader();
        virtual ~BaseAssetLoader();

        virtual bool Load(const char* assetName, void* &handle) = 0;
        virtual void Destroy(const void* handle) = 0;
    private:
    };
}


#endif
