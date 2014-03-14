#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_MATERIAL_LOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_MATERIAL_LOADER_HPP

#include <tuple>
#include <vector>
#include <string>
#include <mutex>

#include <ContentManagement/AssetStructs/MaterialData.hpp>
#include <ContentManagement/AssetStructs/TextureData.hpp>
#include <ContentManagement/Loaders/BaseAssetLoader.hpp>

namespace Core
{
    typedef std::vector<Core::MaterialData*> MaterialVector;
    typedef std::vector<std::tuple<unsigned int, unsigned int, Core::TextureData*>> TextureCacheVector;

    class MaterialLoader : public Core::BaseAssetLoader
    {
    public:
        MaterialLoader();
        ~MaterialLoader();

        Core::AssetHandle Load(const char* assetName) override;
        Core::AssetHandle LoadAsync(const char* assetName) override;

        void FinishLoadAsync(Core::AssetHandle& handle) override;
        void Destroy(const Core::AssetHandle handle) override;

    private:

        struct MaterialLoadingData
        {
            unsigned char* diffuseData;
            unsigned char* specularData;
            unsigned char* normalBlendData;
            unsigned char* glowData;

            std::string shaderName;
            Core::MaterialData* materialData;
        };

		bool ParseFile(const char* assetFileName, std::string& shaderName, std::string& diffuseTexture, std::string& specularTexture, std::string& normalBlendTexture, std::string& glowTexture, bool& isDecal);

        unsigned char* LoadTextureData(const char* textureFileName, Core::TextureData* &data);
        void BufferTextureData(unsigned char* textureData, Core::TextureData* data, bool decal);
        void AddTextureToMaterial(const unsigned long long int materialId, const unsigned textureId);

        int AddUserOfTexture(const Core::TextureData* texture);
        int RemoveUserOfTexture(const Core::TextureData* texture);
        void AddTextureToCache(const unsigned int textureHash, Core::TextureData* texture);
        bool GetTextureCacheStatus(const unsigned int textureHash, Core::TextureData* &texture);

        Core::MaterialVector m_materials;
        Core::TextureCacheVector m_textureCache;

        std::mutex m_cacheMutex;
    };
}


#endif
