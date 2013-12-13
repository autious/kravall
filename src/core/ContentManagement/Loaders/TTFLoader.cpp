#include "TTFLoader.hpp"

#include <algorithm>
#include <string>
#include <cstring>
#include <cassert>
#include <fstream>

#include <logger/Logger.hpp>

namespace Core
{
    TTFLoader::TTFLoader()
    {
        if(FT_Init_FreeType(&m_library)) 
        {
            LOG_FATAL << "Failed to initialize FreeType library" << std::endl;
        }
    }

    TTFLoader::~TTFLoader()
    {
        for(std::vector<Core::FontData*>::iterator it = m_fontData.begin(); it != m_fontData.end(); ++it)
        {
            if(ReduceUserOfFace((*it)->fontFace) == 0)
            {
                FT_Done_Face(*((*it)->fontFace));
            }
            glDeleteTextures(1, &(*it)->fontAtlas);
            delete (*it);
        }
        FT_Done_FreeType(m_library);
    }

    Core::AssetHandle TTFLoader::Load(const char* assetName)
    {
        std::string fontName;
        unsigned int fontSize;
        FontData* fontData = nullptr;

        if(ParseFile(assetName, fontName, fontSize))
        {
            unsigned int fontHash = MurmurHash2(fontName.c_str(), fontName.size(), fontSize);

            FT_Face* face;
            if(!GetFaceCachedStatus(fontHash, face))
            {
                face = new FT_Face;
                FT_New_Face(m_library, fontName.c_str(), 0, &(*face));
                m_facesCache.push_back(std::make_tuple(fontHash, 0, face));
            }
           
            AddUserOfFace(face);

            fontData = new FontData;
            fontData->fontFace = face;

            CreateMeasurements(fontData, fontSize);

            CreateTextureAtlas(fontData);

            m_fontData.push_back(fontData);
        }
        return fontData;
    }

    Core::AssetHandle TTFLoader::LoadAsync(const char* assetName)
    {
        return nullptr;
    }

    void TTFLoader::FinishLoadAsync(Core::AssetHandle& handle)
    {
        
    }

    void TTFLoader::Destroy(const Core::AssetHandle handle)
    {
        Core::FontData* data = static_cast<Core::FontData*>(handle);

        for(std::vector<FontData*>::iterator it = m_fontData.begin(); it != m_fontData.end(); ++it)
        {
            if((*it) == static_cast<Core::FontData*>(handle))
            {
                m_fontData.erase(it);
                break;
            }
        }

        if(ReduceUserOfFace(data->fontFace) == 0)
        {
            FT_Done_Face(*data->fontFace);
        }

        glDeleteTextures(1, &data->fontAtlas);
        delete static_cast<Core::FontData*>(handle);
    }

    void TTFLoader::CreateMeasurements(FontData* &data, const unsigned int fontSize)
    {
        FT_Set_Pixel_Sizes(*data->fontFace, 0, fontSize);
        FT_GlyphSlot glyphSlot = (*data->fontFace)->glyph;

        int roww = 0;
        int rowh = 0;
        int offSetX = 0;
        int offSetY = 0;

        data->atlasWidth = 0;
        data->atlasHeight = 0;

        memset(data->characters, 0, FontData::NUM_CHARACTERS * sizeof(FontCharacter));

        for(int i = 32; i < FontData::NUM_CHARACTERS; ++i)
        {
            if(FT_Load_Char(*data->fontFace, i, FT_LOAD_RENDER))
            {
                LOG_WARNING << "Loading character:" << static_cast<char>(i) << " with value: " << i << " failed" << std::endl;
                continue;
            }

            if(roww + glyphSlot->bitmap.width + 1 >= FontData::ATLAS_MAX_WIDTH)
            {
                data->atlasWidth = std::max(static_cast<int>(data->atlasWidth), roww);
                data->atlasHeight += rowh;
                roww = 0;
                rowh = 0;
            }

            data->characters[i].advanceX = static_cast<float>(glyphSlot->advance.x >> 6);
            data->characters[i].advanceY = static_cast<float>(glyphSlot->advance.y >> 6);

            data->characters[i].bitMapWidth = static_cast<float>(glyphSlot->bitmap.width);
            data->characters[i].bitMapHeight = static_cast<float>(glyphSlot->bitmap.rows);

            data->characters[i].bitMapLeft = static_cast<float>(glyphSlot->bitmap_left);
            data->characters[i].bitMapTop = static_cast<float>(glyphSlot->bitmap_top);

            roww += glyphSlot->bitmap.width + 1;
            rowh = std::max(rowh, glyphSlot->bitmap.rows);
        }
        
        data->atlasWidth = std::max(static_cast<int>(data->atlasWidth), roww);
        data->atlasHeight += rowh;

        roww = 0;
        rowh = 0;

        for(int i = 32; i < FontData::NUM_CHARACTERS; ++i)
        {
            if(FT_Load_Char(*data->fontFace, i, FT_LOAD_RENDER))
            {
                continue;
            }

            if(offSetX + glyphSlot->bitmap.width + 1 >= FontData::ATLAS_MAX_WIDTH)
            {
                offSetY += rowh;                 
                offSetX = 0;
                rowh = 0;               
            }

            data->characters[i].uvOffSetX = offSetX / static_cast<float>(data->atlasWidth);
            data->characters[i].uvOffSetY = offSetY / static_cast<float>(data->atlasHeight);

            rowh = std::max(rowh, glyphSlot->bitmap.rows);
            offSetX += glyphSlot->bitmap.width + 1;
        }

    }

    void TTFLoader::CreateTextureAtlas(FontData* &data)
    {
        FT_GlyphSlot glyphSlot = (*data->fontFace)->glyph;

        glGenTextures(1, &data->fontAtlas);
        glBindTexture(GL_TEXTURE_2D, data->fontAtlas);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, data->atlasWidth, data->atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        for(int i = 32; i < FontData::NUM_CHARACTERS; ++i)
        {
            if(FT_Load_Char(*data->fontFace, i, FT_LOAD_RENDER))
            {
                continue;
            }

            glTexSubImage2D(GL_TEXTURE_2D, 0,
                    static_cast<int>(data->characters[i].uvOffSetX) * static_cast<int>(data->atlasWidth),
                    static_cast<int>(data->characters[i].uvOffSetY) * static_cast<int>(data->atlasHeight),
                    glyphSlot->bitmap.width, glyphSlot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE,
                    glyphSlot->bitmap.buffer);

        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    int TTFLoader::AddUserOfFace(const FT_Face* face)
    {
        for(FaceCacheVector::iterator it = m_facesCache.begin(); it != m_facesCache.end(); ++it)
        {
            if(std::get<2>(*it) == face)
            {
                return std::get<1>(*it) = std::get<1>(*it) + 1;
            }
        }
        LOG_FATAL << "Trying to increase user count of an unexisting font face" << std::endl;
        assert(false);
        return 0;
    }

    int TTFLoader::ReduceUserOfFace(const FT_Face* face)
    {
        for(FaceCacheVector::iterator it = m_facesCache.begin(); it != m_facesCache.end(); ++it)
        {
            if(std::get<2>(*it) == face)
            {
                return std::get<1>(*it) = std::get<1>(*it) - 1;
            }
        }
        LOG_FATAL << "Trying to reduce user count of an unexisting font face" << std::endl;
        assert(false);
        return 0;
    }

    bool TTFLoader::GetFaceCachedStatus(const unsigned int faceHash, FT_Face* &face)
    {
        for(FaceCacheVector::iterator it = m_facesCache.begin(); it != m_facesCache.end(); ++it)
        {
            if(std::get<0>(*it) == faceHash)
            {
                face = std::get<2>(*it);               
                return true; 
            }
        }
        return false;
    }

    bool TTFLoader::ParseFile(const char* assetFileName, std::string& fontName, unsigned int& fontSize)
    {
        std::ifstream file(assetFileName, std::ios::in);

        if(file.is_open())
        {
            bool fontFound = false;
            bool sizeFound = false;

            while(!file.eof())
            {
                std::string key;
                std::string value;

                std::getline(file, key, '=');
                key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

                std::transform(key.begin(), key.end(), key.begin(), ::tolower);

                if(key == "font")
                {
                    std::getline(file, value, '\n');

                    std::string::size_type start = value.find_first_of("\"");
                    std::string::size_type end = value.find_last_of("\"");
                    fontName = value.substr(start + 1, end - start - 1);

                    fontFound = true;
                }
                else if(key == "size")
                {
                    std::getline(file, value, '\n');
                    value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                    fontSize = std::stoi(value);

                    sizeFound = true;
                }
                else
                {
                    if(value.size() > 0)
                    {
                        std::getline(file, value, '\n');
                        LOG_WARNING << "Parsed unknown key: " << key << " in Font file: " << assetFileName << std::endl;
                    }
                }
            }    
            return fontFound && sizeFound;
        }
        return false;
    }
}
