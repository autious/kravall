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

        void* Load(const char* assetName) override;
        void* LoadAsync(const char* assetName, Core::FinisherVector& finisherList, std::mutex& finisherMutex) override;
        void Destroy(const Core::AssetHandle assetHandle) override;
    private:
        

    };
}


#endif
