#include "MaterialLoader.hpp"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.cpp"

#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <fstream>
#include <limits>

#include <ContentManagement/MurmurHash.hpp>

#include <logger/Logger.hpp>

#include <gfx/GFXInterface.hpp>

namespace Core
{
    MaterialLoader::MaterialLoader()
    {

    }

    MaterialLoader::~MaterialLoader()
    {
        for(Core::MaterialVector::size_type i = 0; i < m_materials.size(); ++i)
        {
            Destroy(static_cast<Core::AssetHandle>(m_materials[i]));
        }

        assert(m_textureCache.size() == 0);
    }

    Core::AssetHandle MaterialLoader::Load(const char* assetName)
    {
        Core::MaterialData* data = nullptr;

        std::string shaderName;
        std::string diffuseTexture;
        std::string specularTexture;
        std::string normalBlendTexture;
        std::string glowTexture;

        if(ParseFile(assetName, shaderName, diffuseTexture, specularTexture, normalBlendTexture, glowTexture))
        {
            data = new Core::MaterialData;
            Core::TextureData* texture = nullptr;

            GFX::Content::CreateMaterial(data->materialId);
            unsigned int shaderId = std::numeric_limits<decltype(shaderId)>::max();
            GFX::Content::GetShaderId(shaderId, shaderName.c_str());
            GFX::Content::AttachShaderToMaterial(data->materialId, shaderId);

            if(shaderId == std::numeric_limits<decltype(shaderId)>::max())
            {            
                LOG_WARNING << "Failed to retrieve shader with name: " << shaderName << std::endl;
            }

            unsigned int textureHash;

            if(diffuseTexture.size())
            {
                textureHash = MurmurHash2(diffuseTexture.c_str(), diffuseTexture.size(), diffuseTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    BufferTextureData(LoadTextureData(diffuseTexture.c_str(), texture), texture);
                    AddTextureToMaterial(data->materialId, texture->textureId);
                }
                data->diffuseTexture = texture;
            }

            if(specularTexture.size())
            {
                textureHash = MurmurHash2(specularTexture.c_str(), specularTexture.size(), specularTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    BufferTextureData(LoadTextureData(specularTexture.c_str(), texture), texture);
                    AddTextureToMaterial(data->materialId, texture->textureId);
                }
                data->specularTexture = texture;
            }

            if(normalBlendTexture.size())
            {
                textureHash = MurmurHash2(normalBlendTexture.c_str(), normalBlendTexture.size(), normalBlendTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    BufferTextureData(LoadTextureData(normalBlendTexture.c_str(), texture), texture);
                    AddTextureToMaterial(data->materialId, texture->textureId);
                }
                data->normalBlendTexture = texture;
            }

            if(glowTexture.size())
            {
                textureHash = MurmurHash2(glowTexture.c_str(), glowTexture.size(), glowTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    BufferTextureData(LoadTextureData(glowTexture.c_str(), texture), texture);
                    AddTextureToMaterial(data->materialId, texture->textureId);
                }
                data->glowTexture = texture;
            }

            m_materials.push_back(data);
        }

        return data;
    }

    Core::AssetHandle MaterialLoader::LoadAsync(const char* assetName)
    {
        Core::MaterialLoader::MaterialLoadingData* data = nullptr;

        std::string shaderName;
        std::string diffuseTexture;
        std::string specularTexture;
        std::string normalBlendTexture;
        std::string glowTexture;

        if(ParseFile(assetName, shaderName, diffuseTexture, specularTexture, normalBlendTexture, glowTexture))
        {
            data = new Core::MaterialLoader::MaterialLoadingData;
            data->materialData                      = new Core::MaterialData;
            data->materialData->diffuseTexture      = nullptr;
            data->materialData->specularTexture     = nullptr;
            data->materialData->normalBlendTexture  = nullptr;
            data->materialData->glowTexture         = nullptr;
            data->diffuseData                       = nullptr;
            data->specularData                      = nullptr;
            data->normalBlendData                   = nullptr;
            data->glowData                          = nullptr;
            Core::TextureData* texture              = nullptr;
            data->shaderName                        = shaderName;

            unsigned int textureHash;

            if(diffuseTexture.size())
            {
                textureHash = MurmurHash2(diffuseTexture.c_str(), diffuseTexture.size(), diffuseTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->diffuseData = LoadTextureData(diffuseTexture.c_str(), texture);
                }
                data->materialData->diffuseTexture = texture;
            }

            if(specularTexture.size())
            {
                textureHash = MurmurHash2(specularTexture.c_str(), specularTexture.size(), specularTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->specularData = LoadTextureData(specularTexture.c_str(), texture);
                }
                data->materialData->specularTexture = texture;
            }

            if(normalBlendTexture.size())
            {
                textureHash = MurmurHash2(normalBlendTexture.c_str(), normalBlendTexture.size(), normalBlendTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->normalBlendData = LoadTextureData(normalBlendTexture.c_str(), texture);
                }
                data->materialData->normalBlendTexture = texture;
            }

            if(glowTexture.size())
            {
                textureHash = MurmurHash2(glowTexture.c_str(), glowTexture.size(), glowTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->glowData = LoadTextureData(glowTexture.c_str(), texture);
                }
                data->materialData->glowTexture = texture;
            }
        }

        return data;
    }

    void MaterialLoader::FinishLoadAsync(Core::AssetHandle& handle)
    {
        Core::MaterialLoader::MaterialLoadingData* loadingData = static_cast<Core::MaterialLoader::MaterialLoadingData*>(handle);

        GFX::Content::CreateMaterial(loadingData->materialData->materialId);
        unsigned int shaderId = std::numeric_limits<decltype(shaderId)>::max();
        GFX::Content::GetShaderId(shaderId, loadingData->shaderName.c_str());
        GFX::Content::AttachShaderToMaterial(loadingData->materialData->materialId, shaderId);

        if(shaderId == std::numeric_limits<decltype(shaderId)>::max())
        {            
            LOG_WARNING << "Failed to retrieve shader with name: " << loadingData->shaderName << std::endl;
        }

        if(loadingData->materialData->diffuseTexture)
        {
            BufferTextureData(loadingData->diffuseData, loadingData->materialData->diffuseTexture);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->diffuseTexture->textureId);
        }
        if(loadingData->materialData->specularTexture)
        {
            BufferTextureData(loadingData->specularData, loadingData->materialData->specularTexture);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->specularTexture->textureId);
        }
        if(loadingData->materialData->normalBlendTexture)
        {
            BufferTextureData(loadingData->normalBlendData, loadingData->materialData->normalBlendTexture);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->normalBlendTexture->textureId);
        }
        if(loadingData->materialData->glowTexture)
        {
            BufferTextureData(loadingData->glowData, loadingData->materialData->glowTexture);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->glowTexture->textureId);
        }

        m_materials.push_back(loadingData->materialData);
        handle = static_cast<Core::AssetHandle>(loadingData->materialData);
        delete loadingData;
    }

    void MaterialLoader::Destroy(const Core::AssetHandle handle)
    {   
        Core::MaterialData* data = static_cast<Core::MaterialData*>(handle);

        for(Core::MaterialVector::iterator it = m_materials.begin(); it != m_materials.end(); ++it)
        {
            if((*it) == data)
            {
                m_materials.erase(it);
                break;
            }
        }

        for(Core::TextureCacheVector::size_type i = 0; i < m_textureCache.size(); ++i)
        {
            bool referenceFound = false;
            Core::TextureData* texture = std::get<2>(m_textureCache[i]);

            if(data->diffuseTexture == texture)
            {
                referenceFound = true;
            }
            else if(data->specularTexture == texture)
            {
                referenceFound = true;
            }
            else if(data->normalBlendTexture == texture)
            {
                referenceFound = true;
            }
            else if(data->glowTexture == texture)
            {
                referenceFound = true;
            }

            if(referenceFound)
            {
                if(RemoveUserOfTexture(texture) == 0)
                {
                    m_textureCache.erase(m_textureCache.begin()+i);
                    GFX::Content::DeleteTexture(texture->textureId);
                    delete texture;
                    --i;
                }
            }
        }

        delete data;
    }

    bool MaterialLoader::ParseFile(const char* assetFileName, std::string& shaderName, std::string& diffuseTexture,
            std::string& specularTexture, std::string& normalBlendTexture, std::string& glowTexture)
    {
        std::ifstream file(assetFileName, std::ios::in);

        if(file.is_open())
        {
            bool shaderFound = false; 

            while(!file.eof())
            {
                std::string key;
                std::string value;

                std::getline(file, key, '=');
                key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

                std::transform(key.begin(), key.end(), key.begin(), ::tolower);

                if(key == "shader")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    shaderName = value.substr(start + 1, end - start - 1);

                    shaderFound = true;
                }
                else if(key == "diffuse")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    diffuseTexture = value.substr(start + 1, end - start - 1);
                }
                else if(key == "specular")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    specularTexture = value.substr(start + 1, end - start - 1);
                }
                else if(key == "normal")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    normalBlendTexture = value.substr(start + 1, end - start - 1);
                }
                else if(key == "blend")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    normalBlendTexture = value.substr(start + 1, end - start - 1);
                }
                else if(key == "glow")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    glowTexture = value.substr(start + 1, end - start - 1);
                }
                else
                {
                    if(value.size() > 0)
                    {
                        std::getline(file, value, '\n');
                        LOG_WARNING << "Parsed unknown key: " << key << " in Material file: " << assetFileName << std::endl;
                    }
                }
            }
            return shaderFound;
        }
        return false;
    }

    unsigned char* MaterialLoader::LoadTextureData(const char* textureFileName, Core::TextureData* &data)
    {
        unsigned char* texturedata = stbi_load(textureFileName, &data->width, &data->height, &data->bitsPerPixel, 0);
        if(texturedata == nullptr)
        {
            LOG_WARNING << "Failed to load image with name: " << textureFileName << std::endl;
        }
        return texturedata;
    }

    void MaterialLoader::BufferTextureData(unsigned char* textureData, Core::TextureData* data)
    {
        if(textureData != nullptr)
        {
            GFX::Content::LoadTexture2DFromMemory(data->textureId, textureData, data->width, data->height);
            stbi_image_free(textureData);
        }
    }

    void MaterialLoader::AddTextureToMaterial(const unsigned long long int materialId, const unsigned int textureId)
    {
        GFX::Content::AddTextureToMaterial(materialId, textureId);
    }

    int MaterialLoader::AddUserOfTexture(const Core::TextureData* texture)
    {

        for(Core::TextureCacheVector::iterator it = m_textureCache.begin(); it != m_textureCache.end(); ++it)
        {
            if(std::get<2>(*it) == texture)
            {
                return std::get<1>(*it) = std::get<1>(*it) + 1;
            }
        }

        LOG_FATAL << "Trying to increase user count of unexisting texture" << std::endl;
        assert(false);
        return 0;
    }

    int MaterialLoader::RemoveUserOfTexture(const Core::TextureData* texture)
    {
        for(Core::TextureCacheVector::iterator it = m_textureCache.begin(); it != m_textureCache.end(); ++it)
        {
            if(std::get<2>(*it) == texture)
            {
                return std::get<1>(*it) = std::get<1>(*it) - 1;
            }
        }

        LOG_FATAL << "Trying to reduce user count of unexisting texture" << std::endl;
        assert(false);
        return 0;
    }

    bool MaterialLoader::GetTextureCacheStatus(const unsigned int textureHash, Core::TextureData* &texture)
    {
        for(Core::TextureCacheVector::iterator it = m_textureCache.begin(); it != m_textureCache.end(); ++it)
        {
            if(std::get<0>(*it) == textureHash)
            {
                texture = std::get<2>(*it);
                return true;
            }
        }
        return false;
    }
}


