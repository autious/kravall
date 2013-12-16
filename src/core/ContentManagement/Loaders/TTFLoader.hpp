#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_TTFLOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_TTFLOADER_HPP

#include <vector>
#include <tuple>

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <gfx/FontData.hpp>
#include <ContentManagement/MurmurHash.hpp>


namespace Core
{
    typedef std::vector<std::tuple<unsigned int, unsigned int, FT_Face*>> FaceCacheVector;

    class TTFLoader: public Core::BaseAssetLoader
    {
    public:
        /*!
          Constructor for TTFLoader, creates the FreeType library.
          */
        TTFLoader();

        /*!
          Destructor for TTFLoader, destroyes all loaded fonts and faces.
          */
        ~TTFLoader();

        /*!
          Synchronous load of fonts.
          \param assetName the name of the asset meta file of the font.
          \returns An AssetHandle that is complete and ready for use.
          */
        Core::AssetHandle Load(const char* assetName) override;

        /*!
          Asynchronous load of fonts.
          \param assetName the name of the asset meta file of the font.
          \returns An AssetHandle that is not yet ready for use.
          */
        Core::AssetHandle LoadAsync(const char* assetName) override;

        /*!
          Finishing function that applies the asynchronous loaded data to state dependent memory.
          \param handle An in-out going parameter that is used to pass data in and out of the function. The handle is ready to use after call.
          */
        void FinishLoadAsync(Core::AssetHandle& handle) override;

        /*!
          Destroys the given font, removing it from memory. The handle can no longer be used after calling this function.
          \param handle The asset handle to destroy.
          */
        void Destroy(const Core::AssetHandle handle) override;

    private:
        void CreateMeasurements(GFX::FontData* &face, const unsigned int fontSize);
        void CreateTextureAtlas(GFX::FontData* &data);
        
        int AddUserOfFace(const FT_Face* face);
        int ReduceUserOfFace(const FT_Face* face);
        bool GetFaceCachedStatus(const unsigned int faceHash, FT_Face* &face);

        bool ParseFile(const char* assetFileName, std::string& fontName, unsigned int& fontSize);

        std::vector<GFX::FontData*> m_fontData;
        Core::FaceCacheVector m_facesCache;
        FT_Library m_library;
    };
}


#endif
