#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_TTFLOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_TTFLOADER_HPP

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>


namespace Core
{
    class TTFLoader: public Core::BaseAssetLoader
    {
    public:
        TTFLoader();
        ~TTFLoader();

        Core::AssetHandle Load(const char* assetName) override;
        Core::AssetHandle LoadAsync(const char* assetName) override;
        void FinishLoadAsync(Core::AssetHandle& handle) override;
        void Destroy(const Core::AssetHandle handle) override;
    private:
        

    };
}


#endif
