#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_TTFLOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_TTFLOADER_HPP

#include <vector>
#include <tuple>

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <ContentManagement/AssetStructs/FontData.hpp>
#include <ContentManagement/MurmurHash.hpp>


namespace Core
{
    typedef std::vector<std::tuple<unsigned int, unsigned int, FT_Face*>> FaceCacheVector;

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
        void CreateMeasurements(FontData* &face, const unsigned int fontSize);
        void CreateTextureAtlas(FontData* &data);
        
        int AddUserOfFace(const FT_Face* face);
        int ReduceUserOfFace(const FT_Face* face);
        bool GetFaceCachedStatus(const unsigned int faceHash, FT_Face* &face);

        bool ParseFile(const char* assetFileName, std::string& fontName, unsigned int& fontSize);

        std::vector<FontData*> m_fontData;
        Core::FaceCacheVector m_facesCache;
        FT_Library m_library;
    };
}


#endif
