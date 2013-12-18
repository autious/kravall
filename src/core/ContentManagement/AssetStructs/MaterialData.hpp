#ifndef CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_MATERIAL_DATA_HPP
#define CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_MATERIAL_DATA_HPP

#include <ContentManagement/AssetStructs/TextureData.hpp>

namespace Core
{
    struct MaterialData
    {
        Core::TextureData* diffuseTexture;
        Core::TextureData* specularTexture;
        Core::TextureData* normalBlendTexture;
        Core::TextureData* glowTexture;
        unsigned long long int materialId;
    };
}

#endif
