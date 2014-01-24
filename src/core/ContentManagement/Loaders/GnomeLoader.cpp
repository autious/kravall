#include "GnomeLoader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>

namespace Core
{
    GnomeLoader::GnomeLoader()
    {

    }

    GnomeLoader::~GnomeLoader()
    {

    }

    Core::AssetHandle GnomeLoader::LoadAsync(const char* assetName)
    {
        Core::GnomeLoader::Gnome* gnome = LoadGnomeFromFile(assetName);     
        return gnome;        
    }

    void GnomeLoader::FinishLoadAsync(Core::AssetHandle& handle)
    {
        Core::GnomeLoader::Gnome* gnome = static_cast<Core::GnomeLoader::Gnome*>(handle);
        Core::ModelData* modelData = new Core::ModelData;

        if(gnome != nullptr)
        {

            GFX::Content::LoadMesh(modelData->meshID, gnome->numberOfVertices, gnome->numberOfIndices, gnome->vertices, gnome->indices);

            m_modelData.push_back(modelData);            

            //delete[] gnome->materials;
            delete[] gnome->indices;
            delete[] gnome->vertices;

            if(gnome->numberOfBones)
            {
                for (int i = 0; i < gnome->numberOfAnimations; ++i)
                {
                    for (int j = 0; j < gnome->numberOfBones; ++j)
                        delete[] gnome->animations[i].boneAnim[j].Keyframes;
                    delete[] gnome->animations[i].boneAnim;
                }
            }
            delete[] gnome->bones;
            delete[] gnome->animations;            

            delete static_cast<Core::GnomeLoader::Gnome*>(handle);
            handle = reinterpret_cast<AssetHandle>(modelData);
        }
        else
        {
            LOG_FATAL << "Data from asynchronous GnomeLoaderFileRead is null" << std::endl;
            assert(false);
        }
    }

    Core::AssetHandle GnomeLoader::Load(const char* assetName)
    {        
        Core::ModelData* modelData = new Core::ModelData();
        Core::GnomeLoader::Gnome* gnome = LoadGnomeFromFile(assetName);

        if(gnome != nullptr)
        {
            GFX::Content::LoadMesh(modelData->meshID, gnome->numberOfVertices, gnome->numberOfIndices, gnome->vertices, gnome->indices);
			

            m_modelData.push_back(modelData);
            
            //delete[] gnome->materials;
            delete[] gnome->indices;
            delete[] gnome->vertices;

            if(gnome->numberOfBones)
            {
                for (int i = 0; i < gnome->numberOfAnimations; ++i)
                {
                    for (int j = 0; j < gnome->numberOfBones; ++j)
                        delete[] gnome->animations[i].boneAnim[j].Keyframes;
                    delete[] gnome->animations[i].boneAnim;
                }
            }
            delete[] gnome->bones;
            delete[] gnome->animations;            

            delete gnome;
            return modelData;
        }
        else
        {
            LOG_FATAL << "Data from synchronous GnomeLoaderFileRead is null " << std::endl;
        }
        return nullptr;
    }

    void GnomeLoader::Destroy(const Core::AssetHandle handle)
    {
        Core::ModelData* modelData;
        for(std::vector<Core::ModelData*>::iterator it = m_modelData.begin(); it != m_modelData.end(); ++it)
        {
            if((*it) == static_cast<Core::ModelData*>(handle))
            {
                modelData = (*it);
                m_modelData.erase(it);
                break;
            }
        }
        GFX::Content::DeleteMesh(modelData->meshID);        
        delete modelData;        
    }

    const ModelData* GnomeLoader::getData(const Core::AssetHandle handle) const
    {
        return static_cast<const Core::ModelData*>(handle);
    }
	std::string GetFileNameAndPath(std::string filename, std::string delim)
	{
		std::string gFile; 

		size_t pos = filename.find_last_of(delim);

		for (unsigned int i = 0; i < pos; i++)
		{
			gFile.push_back(filename[i]); //Letter-by-letter string cut
		}

		return gFile;
	}
    Core::GnomeLoader::Gnome* GnomeLoader::LoadGnomeFromFile(const char* fileName)
    {
		std::fstream m_file;
		std::fstream m_animationFile;

		m_file.open(fileName, std::ios::in | std::ios::binary);
		m_animationFile.open(GetFileNameAndPath(fileName,".") + ".bagnome", std::ios::in | std::ios::binary);
        if (m_file)
        {
            Core::GnomeLoader::Header header;
			Core::GnomeLoader::AnimationHeader animationHeader;

			/* Magic */
			char m_magicByte[6], m_magicByteAnimation[10];
			m_file.read((char*)m_magicByte, 6);
			m_animationFile.read((char*)m_magicByteAnimation, 10);

			if (strcmp(m_magicByte, "GNOME") != 0)
			{
				LOG_FATAL << fileName << " is not a .BGNOME, of a obsolete version of .BGNOME or corrupted." << std::endl;
				m_file.close();
				return nullptr;
			}

			Core::GnomeLoader::Gnome* gnome = new Core::GnomeLoader::Gnome;

			/* Header */
			m_file.read((char*)&header, sizeof(header));

            gnome->numberOfVertices = header.numberOfVertices;
			gnome->numberOfIndices = header.numberOfIndices;
            gnome->numberOfBones = header.numberOfBones;

			gnome->vertices = new GFX::Vertex[header.numberOfVertices];
			gnome->indices = new int[header.numberOfIndices];
            gnome->bones = new Core::GnomeLoader::Bone[header.numberOfBones];

			/* Vertex */
			m_file.read((char*)gnome->vertices, sizeof(GFX::Vertex) * header.numberOfVertices);

			/* Index */
			m_file.read((char*)gnome->indices, sizeof(int) * header.numberOfIndices);

			/* Animations */
			if (header.numberOfBones)
			{
				/* Bones */
				for (int k = 0; k < header.numberOfBones; k++)
				{
					m_file.read((char*)&gnome->bones[k], sizeof(int) * 3 + sizeof(float) * 16 );
                    char* boneName = new char[gnome->bones[k].nameSize];
                    m_file.read((char*)boneName, gnome->bones[k].nameSize);
                    gnome->bones[k].name = std::string(boneName);
                    delete[] boneName;
				}

				/* Animation File */
				if (m_animationFile) //Not the prettiest code ever made, but hey, if it works, it works! :D
				{
					/* Magic Byte */
					if (strcmp(m_magicByteAnimation, "ANIMGNOME") == 0)
					{
						/* Header */
						m_animationFile.read((char*)&animationHeader, sizeof(animationHeader));
						gnome->numberOfAnimations = animationHeader.numberOfAnimations;

						if (animationHeader.numberOfBones == header.numberOfBones)
						{
							gnome->animations = new Core::GnomeLoader::Animation[animationHeader.numberOfAnimations];

							/* Animation */
							for (int i = 0; i < animationHeader.numberOfAnimations; ++i)
							{
								m_file.read((char*)&gnome->animations[i].nameSize, sizeof(int));
								char* tmp = new char[gnome->animations[i].nameSize];
								m_file.read((char*)tmp, gnome->animations[i].nameSize);
								gnome->animations[i].name = std::string(tmp);
								delete[] tmp;
								gnome->animations[i].boneAnim = new BoneForAnimation[header.numberOfBones];

								for (int j = 0; j < header.numberOfBones; ++j)
								{
									m_file.read((char*)&gnome->animations[i].boneAnim[j].numKeys, sizeof(int));
									gnome->animations[i].boneAnim[j].Keyframes = new Keyframe[gnome->animations[i].boneAnim[j].numKeys];
									m_file.read((char*)gnome->animations[i].boneAnim[j].Keyframes, sizeof(Keyframe)* gnome->animations[i].boneAnim[j].numKeys);
								}
							}
						}
						else
						{
							gnome->numberOfAnimations = 0;
							gnome->animations = new Core::GnomeLoader::Animation[gnome->numberOfAnimations];
							LOG_FATAL << fileName << ": Number of bones do not match .bgnome and .bagnome. Have you used the correct animation file?" << std::endl;
							std::cout << "Binary Animation GNOME File do not match .bgnome, proceeds without animation..." << std::endl;
						}
					}
					else
					{
						gnome->numberOfAnimations = 0;
						gnome->animations = new Core::GnomeLoader::Animation[gnome->numberOfAnimations];
						std::cout << "Binary Animation GNOME File is missing for " << GetFileNameAndPath(fileName, ".") << ".bagnome, proceeds without animation..." << std::endl;
					}
				}
			}
			else
			{
				gnome->numberOfAnimations = 0;
				gnome->animations = new Core::GnomeLoader::Animation[gnome->numberOfAnimations];
				std::cout << "there was no bones, ignoring reading .bagnome" << std::endl;
			}

			/* Done! */
			m_animationFile.close();
            m_file.close();
            return gnome;
        }
        else
        {
           LOG_FATAL << "Unable to open GnomeFile with path: " << fileName << std::endl; 
        }
        m_file.close();
        return nullptr;
    }
}
