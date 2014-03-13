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
        LOG_DEBUG << "Created MaterialLoader" << std::endl;
    }

    MaterialLoader::~MaterialLoader()
    {
        
        for(Core::MaterialVector::iterator it = m_materials.begin(); it != m_materials.end(); ++it)
        {
            GFX::Content::DeleteMaterial((*it)->materialId);
            delete (*it);
        }

        for(Core::TextureCacheVector::size_type i = 0; i < m_textureCache.size(); ++i)
        {
            Core::TextureData* texture = std::get<2>(m_textureCache[i]); 
            GFX::Content::DeleteTexture(texture->textureId);
            delete texture;
        } 
        
    }

    Core::AssetHandle MaterialLoader::Load(const char* assetName)
    {
        Core::MaterialData* data = nullptr;

        std::string shaderName;
        std::string diffuseTexture;
        std::string specularTexture;
        std::string normalBlendTexture;
        std::string glowTexture;
		bool isDecal = false;

        if(ParseFile(assetName, shaderName, diffuseTexture, specularTexture, normalBlendTexture, glowTexture, isDecal))
        {
            data = new Core::MaterialData;
            Core::TextureData* texture = nullptr;
            data->diffuseTexture = nullptr;
            data->specularTexture = nullptr;
            data->normalBlendTexture = nullptr;
            data->glowTexture = nullptr;

            GFX::Content::CreateMaterial(data->materialId);
            unsigned int shaderId = std::numeric_limits<decltype(shaderId)>::max();

			if (GFX::Content::GetShaderId(shaderId, shaderName.c_str()) != GFX_SUCCESS)
			{
				LOG_WARNING << "Failed to get shader ID from name: " << shaderName << std::endl;
			}

            if (GFX::Content::AttachShaderToMaterial(data->materialId, shaderId) != GFX_SUCCESS)
			{
				LOG_WARNING << "Failed to attach shader to material, material does not exist. Material ID: " << data->materialId << std::endl;
			}


            if(shaderId == std::numeric_limits<decltype(shaderId)>::max())
            {            
                LOG_WARNING << "Failed to retrieve shader with name: " << shaderName << std::endl;
            }

            unsigned int textureHash;

            if(diffuseTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(diffuseTexture.c_str(), diffuseTexture.size(), diffuseTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
					BufferTextureData(LoadTextureData(diffuseTexture.c_str(), texture), texture, isDecal);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
                }

                AddTextureToMaterial(data->materialId, texture->textureId);
                data->diffuseTexture = texture;
                assert(texture->textureId != 0);
            }

			if(normalBlendTexture.size())
			{
                std::lock_guard<std::mutex> lock(m_cacheMutex);
				textureHash = MurmurHash2(normalBlendTexture.c_str(), normalBlendTexture.size(), normalBlendTexture.size());
				if (!GetTextureCacheStatus(textureHash, texture))
				{
					texture = new Core::TextureData;
					BufferTextureData(LoadTextureData(normalBlendTexture.c_str(), texture), texture, isDecal);
                    AddTextureToCache(textureHash, texture);
				}
                else
                {
                    AddUserOfTexture(texture);
                }

			    AddTextureToMaterial(data->materialId, texture->textureId);
				data->normalBlendTexture = texture;
                assert(texture->textureId != 0);			
            }

            if(specularTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(specularTexture.c_str(), specularTexture.size(), specularTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
					BufferTextureData(LoadTextureData(specularTexture.c_str(), texture), texture, isDecal);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
                }

                AddTextureToMaterial(data->materialId, texture->textureId);
                data->specularTexture = texture;
                assert(texture->textureId != 0);
            }
          

            if(glowTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(glowTexture.c_str(), glowTexture.size(), glowTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
					BufferTextureData(LoadTextureData(glowTexture.c_str(), texture), texture, isDecal);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
                }

                AddTextureToMaterial(data->materialId, texture->textureId);
                data->glowTexture = texture;
                assert(texture->textureId != 0);
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
		bool isDecal = false;

        if(ParseFile(assetName, shaderName, diffuseTexture, specularTexture, normalBlendTexture, glowTexture, isDecal))
        {
            data = new Core::MaterialLoader::MaterialLoadingData;
            data->materialData                      = new Core::MaterialData;
            data->materialData->diffuseTexture      = nullptr;
            data->materialData->specularTexture     = nullptr;
            data->materialData->normalBlendTexture  = nullptr;
            data->materialData->glowTexture         = nullptr;
			data->materialData->isDecal				= false;
            data->diffuseData                       = nullptr;
            data->specularData                      = nullptr;
            data->normalBlendData                   = nullptr;
            data->glowData                          = nullptr;
            Core::TextureData* texture              = nullptr;
            data->shaderName                        = shaderName;

            unsigned int textureHash;


			data->materialData->isDecal = isDecal;

            if(diffuseTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(diffuseTexture.c_str(), diffuseTexture.size(), diffuseTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->diffuseData = LoadTextureData(diffuseTexture.c_str(), texture);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
                }
                data->materialData->diffuseTexture = texture;
            }

            if(normalBlendTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(normalBlendTexture.c_str(), normalBlendTexture.size(), normalBlendTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->normalBlendData = LoadTextureData(normalBlendTexture.c_str(), texture);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
                }
                data->materialData->normalBlendTexture = texture;
            }

            if(specularTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(specularTexture.c_str(), specularTexture.size(), specularTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->specularData = LoadTextureData(specularTexture.c_str(), texture);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
                }
                data->materialData->specularTexture = texture;
            }


            if(glowTexture.size())
            {
                std::lock_guard<std::mutex> lock(m_cacheMutex);
                textureHash = MurmurHash2(glowTexture.c_str(), glowTexture.size(), glowTexture.size());
                if(!GetTextureCacheStatus(textureHash, texture))
                {
                    texture = new Core::TextureData;
                    data->glowData = LoadTextureData(glowTexture.c_str(), texture);
                    AddTextureToCache(textureHash, texture);
                }
                else
                {
                    AddUserOfTexture(texture);
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
            std::lock_guard<std::mutex> lock(m_cacheMutex);
			BufferTextureData(loadingData->diffuseData, loadingData->materialData->diffuseTexture, loadingData->materialData->isDecal);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->diffuseTexture->textureId);
        }
        if(loadingData->materialData->specularTexture)
        {
            std::lock_guard<std::mutex> lock(m_cacheMutex);
			BufferTextureData(loadingData->specularData, loadingData->materialData->specularTexture, loadingData->materialData->isDecal);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->specularTexture->textureId);
        }
        if(loadingData->materialData->normalBlendTexture)
        {
            std::lock_guard<std::mutex> lock(m_cacheMutex);
			BufferTextureData(loadingData->normalBlendData, loadingData->materialData->normalBlendTexture, loadingData->materialData->isDecal);
            AddTextureToMaterial(loadingData->materialData->materialId, loadingData->materialData->normalBlendTexture->textureId);
        }
        if(loadingData->materialData->glowTexture)
        {
            std::lock_guard<std::mutex> lock(m_cacheMutex);
			BufferTextureData(loadingData->glowData, loadingData->materialData->glowTexture, loadingData->materialData->isDecal);
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
            Core::TextureData* texture = std::get<2>(m_textureCache[i]);

            if(data->diffuseTexture == texture)
            {
                if(RemoveUserOfTexture(texture) == 0)
                {
                    m_textureCache.erase(m_textureCache.begin()+i);
                    GFX::Content::DeleteTexture(texture->textureId);
                    delete texture;
                    --i;
                }
            }

            if(data->normalBlendTexture == texture)
            {
                if(RemoveUserOfTexture(texture) == 0)
                {
                    m_textureCache.erase(m_textureCache.begin()+i);
                    GFX::Content::DeleteTexture(texture->textureId);
                    delete texture;
                    --i;
                }
            }

            if(data->specularTexture == texture)
            {
                if(RemoveUserOfTexture(texture) == 0)
                {
                    m_textureCache.erase(m_textureCache.begin()+i);
                    GFX::Content::DeleteTexture(texture->textureId);
                    delete texture;
                    --i;
                }
            }

            if(data->glowTexture == texture)
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

        GFX::Content::DeleteMaterial(data->materialId);
        delete data;
    }

    bool MaterialLoader::ParseFile(const char* assetFileName, std::string& shaderName, std::string& diffuseTexture,
            std::string& specularTexture, std::string& normalBlendTexture, std::string& glowTexture, bool& isDecal)
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
				else if (key == "decal")
				{
					std::getline(file, value, '\n');

					std::string::size_type start = value.find_first_of("\"");
					std::string::size_type end = value.find_last_of("\"");
					std::string decalVal = value.substr(start + 1, end - start - 1);

					if (decalVal == "true")
						isDecal = true;
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
        else
        {
            LOG_FATAL << "Could not open material file: " << assetFileName << std::endl;
            return false;
        }
    }

    unsigned char* MaterialLoader::LoadTextureData(const char* textureFileName, Core::TextureData* &data)
    {
        unsigned char* texturedata = stbi_load(textureFileName, &data->width, &data->height, &data->bitsPerPixel, 4);
        if(texturedata == nullptr)
        {
            LOG_WARNING << "Failed to load image with name: " << textureFileName << ". images are requried to be of 32bit color"<< std::endl;
        }
        return texturedata;
    }

    void MaterialLoader::BufferTextureData(unsigned char* textureData, Core::TextureData* data, bool decal)
    {
        if(textureData != nullptr)
        {
            GFX::Content::LoadTexture2DFromMemory(data->textureId, textureData, data->width, data->height, decal);
            stbi_image_free(textureData);
        }
    }

    void MaterialLoader::AddTextureToMaterial(const unsigned long long int materialId, const unsigned int textureId)
    {
		if (GFX::Content::AddTextureToMaterial(materialId, textureId) != GFX_SUCCESS)
		{
			LOG_WARNING << "Failed to add texture to material, material does not exist. Material ID: " << materialId << std::endl;
		}
    }

    int MaterialLoader::AddUserOfTexture(const Core::TextureData* texture)
    {
        for(Core::TextureCacheVector::iterator it = m_textureCache.begin(); it != m_textureCache.end(); ++it)
        {
            if(std::get<2>(*it) == texture)
            {
                return (std::get<1>(*it) = std::get<1>(*it) + 1);
            }
        }

        LOG_WARNING << "Trying to increase user count of unexisting texture" << std::endl;
        assert(false);
        return -1;
    }

    int MaterialLoader::RemoveUserOfTexture(const Core::TextureData* texture)
    {
        for(Core::TextureCacheVector::iterator it = m_textureCache.begin(); it != m_textureCache.end(); ++it)
        {
            if(std::get<2>(*it) == texture)
            {
                return (std::get<1>(*it) = std::get<1>(*it) - 1);
            }
        }

        LOG_WARNING << "Trying to reduce user count of unexisting texture with value: " << texture << " and Id: " << texture->textureId << std::endl;
        assert(false);
        return -1;
    }

    void MaterialLoader::AddTextureToCache(const unsigned int textureHash, Core::TextureData* texture)
    {
        m_textureCache.push_back(std::make_tuple(textureHash, 1U, texture));
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


