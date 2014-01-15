#include "GnomeLoader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <gfx/GFXInterface.hpp>

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

		GFX::Content::LoadStaticMesh(modelData->meshID, gnome->numberOfVertices, gnome->numberOfIndices, gnome->vertices, gnome->indices);

        m_modelData.push_back(modelData);            

        //delete[] gnome->materials;
        delete[] gnome->bones;
        delete[] gnome->animations;            
        delete[] gnome->indices;
        delete[] gnome->vertices;
        delete static_cast<Core::GnomeLoader::Gnome*>(handle);
        handle = reinterpret_cast<AssetHandle>(modelData);
    }

    Core::AssetHandle GnomeLoader::Load(const char* assetName)
    {        
        Core::ModelData* modelData = new Core::ModelData();
        Core::GnomeLoader::Gnome* gnome = LoadGnomeFromFile(assetName);

		GFX::Content::LoadStaticMesh(modelData->meshID, gnome->numberOfVertices, gnome->numberOfIndices, gnome->vertices, gnome->indices);

		m_modelData.push_back(modelData);
        
        //delete[] gnome->materials;
        delete[] gnome->bones;
        delete[] gnome->animations;            
        delete[] gnome->indices;
        delete[] gnome->vertices;
        delete gnome;
        return modelData;
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
        GFX::Content::DeleteStaticMesh(modelData->meshID);        
        delete modelData;
    }

    const ModelData* GnomeLoader::getData(const Core::AssetHandle handle) const
    {
        return static_cast<const Core::ModelData*>(handle);
    }

    Core::GnomeLoader::Gnome* GnomeLoader::LoadGnomeFromFile(const char* fileName)
    {
		std::fstream m_file;

		m_file.open(fileName, std::ios::in | std::ios::binary);
        if (m_file)
        {
            Core::GnomeLoader::Animation animation;
            Core::GnomeLoader::Bone bone;
            Core::GnomeLoader::Vertex vertex;
            Core::GnomeLoader::Header header;


			/* Magic */
			char m_magicByte[6];
			m_file.read((char*)m_magicByte, 6);

			if (strcmp(m_magicByte, "GNOME") != 0)
			{
				std::cout << "ERROR: " << fileName << " is not a .BGNOME, of a obsolete version of .BGNOME or corrupted." << std::endl;
				m_file.close();
				return nullptr;
			}
			else
			{
				std::cout << "BGNOME file accepted" << std::endl;
			}

			/* Header */
			m_file.read((char*)&header, sizeof(Header));

            Core::GnomeLoader::Mesh mesh;
            Core::GnomeLoader::Gnome* gnome = new Core::GnomeLoader::Gnome;

			mesh.vertices = new Core::GnomeLoader::Vertex[header.numberOfVertices];
			mesh.indices = new int[header.numberOfIndices];
            gnome->numberOfVertices = header.numberOfVertices;
            gnome->bones = new Core::GnomeLoader::Bone[header.numberOfBones];
            gnome->animations = new Core::GnomeLoader::Animation[header.numberOfBones];

			/* Vertex */
			if (header.numberOfBones)
				m_file.read((char*)mesh.animatedVertices, sizeof(VertexBoneAnimated)*header.numberOfVertices); //TODO: animatedvertices need to be allocated
			else
				m_file.read((char*)mesh.vertices, sizeof(Vertex)* header.numberOfVertices);

			std::cout << "Parsed verts" << std::endl;


			/* Index */
			m_file.read((char*)mesh.indices, sizeof(int)* header.numberOfIndices);
			std::cout << "Parsed indices" << std::endl;

			/* Animations */
			if (header.numberOfBones)
			{
				/* Bones */
				for (int k = 0; k < header.numberOfBones; k++) //TODO: load name
				{
					m_file.read((char*)&bones[k].id, sizeof(int));
					m_file.read((char*)&bones[k].parentID, sizeof(int));
					m_file.read((char*)&bones[k].offsetMatrix, sizeof(float)* 4 * 4);
				}
				if (header.numberOfAnimations)
				{
					/* Skeletal Animation */
					for (int i = 0; i < header.numberOfAnimations; ++i)
					{
						m_file.read((char*)&animations[i].nameSize, sizeof(int));
						char* tmp = new char[animations[i].nameSize];
						m_file.read((char*)tmp, animations[i].nameSize);
						animations[i].name = std::string(tmp);
						delete [] tmp;
						animations[i].boneAnim = new BoneForAnimation[header.numberOfBones];
						for (int j = 0; j < header.numberOfBones; ++j)
						{
							m_file.read((char*)&animations[i].boneAnim[j].numKeys, sizeof(int));
							animations[i].boneAnim[j].Keyframes = new Keyframe[animations[i].boneAnim[j].numKeys];
							m_file.read((char*)animations[i].boneAnim[j].Keyframes, sizeof(Keyframe)* animations[i].boneAnim[j].numKeys);
						}
					}
				}
			}
			else if (header.numberOfAnimations)
			{
				/* Morph Animation */
				for (int i = 0; i < header.numberOfAnimations; i++)
				{
					m_file.read((char*)&morphAnimation[i].id, sizeof(int));
					m_file.read((char*)&morphAnimation[i].numberOfKeys, sizeof(int));
					morphAnimation[i].keys = new MorphKey[morphAnimation[i].numberOfKeys];

					for (int j = 0; j < morphAnimation[i].numberOfKeys; j++)
					{
						morphAnimation[i].keys[j].vertices = new Vertex[header.numberOfVertices];
						m_file.read((char*)&morphAnimation[i].keys[j].keyTime, sizeof(int));
						m_file.read((char*)&morphAnimation[i].keys[j].vertices, sizeof(Vertex)* header.numberOfVertices);
					}
				}
			}

            //Apply data to GFX buffers
			gnome->numberOfIndices = header.numberOfIndices;
            gnome->indices = new int[gnome->numberOfIndices];           
            gnome->vertices = new GFX::StaticVertex[gnome->numberOfVertices];
			for (int i = 0; i < gnome->numberOfIndices; i++)
            {
                gnome->indices[i] = mesh.indices[i];
				//std::cout << mesh.indices[i] << std::endl;
            }
            
            for (int i = 0; i < gnome->numberOfVertices; i++)
            {
                GFX::StaticVertex v;
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].position[j] = mesh.vertices[i].position[j];
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].normal[j] = mesh.vertices[i].normal[j];
                for (int j = 0; j < 2; j++)
                    gnome->vertices[i].uv[j] = mesh.vertices[i].uv[j];
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].tangent[j] = mesh.vertices[i].tangent[j];
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].binormal[j] = mesh.vertices[i].tangent[j]; //TODO: FIX: This is only while there is no calculation for the binormal

                gnome->vertices[i].position[3] = 1.0f;
                gnome->vertices[i].normal[3] = 0.0f;
                gnome->vertices[i].tangent[3] = 0.0f;
                gnome->vertices[i].tangent[3] = 0.0f;
            }

			/* Freeing allocated memory*/
            delete[] mesh.vertices;
			delete[] mesh.indices;

			if (header.numberOfBones > 0)
			{
				for (int i = 0; i < header.numberOfAnimations; ++i)
				{
					for (int j = 0; j < header.numberOfBones; ++j)
						delete[] animations[i].boneAnim[j].Keyframes;
					delete[] animations[i].boneAnim;
				}
				delete[] bones;
				delete[] animations;
				delete[] mesh.animatedVertices;
			}
			else if (header.numberOfAnimations > 0)
			{
				for (int i = 0; i < header.numberOfAnimations; i++)
				{
					for (int j = 0; j < morphAnimation[i].numberOfKeys; j++)
						delete[] morphAnimation[i].keys[j].vertices;
					delete[] morphAnimation[i].keys;
				}
				delete[] morphAnimation;
			}
			/* Done! */

            std::cout << "parsed data" << std::endl;
            m_file.close();
            return gnome;
        }
        m_file.close();
        return nullptr;
    }
}
