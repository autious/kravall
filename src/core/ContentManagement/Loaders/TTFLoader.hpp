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

        bool Load(const char* assetName, void* &assetHandle) override;
        void Destroy(const void* assetHandle) override;
    private:
        

    };
}


#endif