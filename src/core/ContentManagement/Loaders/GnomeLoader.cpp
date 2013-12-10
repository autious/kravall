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
        modelData->iSize = modelData->vSize = gnome->numberOfVertices;

        m_modelData.push_back(modelData);            

        GFX::Content::LoadStaticMesh(modelData->IBO, modelData->VAO, gnome->numberOfVertices, gnome->numberOfIndices, gnome->vertices, gnome->indices);
        delete[] gnome->materials;
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

        modelData->iSize = modelData->vSize = gnome->numberOfVertices;

        m_modelData.push_back(modelData);            

        GFX::Content::LoadStaticMesh(modelData->IBO, modelData->VAO, gnome->numberOfVertices, gnome->numberOfVertices, gnome->vertices, gnome->indices);
        
        delete[] gnome->materials;
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
        GFX::Content::DeleteStaticMesh(modelData->IBO, modelData->VAO);        
        delete modelData;
    }

    const ModelData* GnomeLoader::getData(const Core::AssetHandle handle) const
    {
        return static_cast<const Core::ModelData*>(handle);
    }

    Core::GnomeLoader::Gnome* GnomeLoader::LoadGnomeFromFile(const char* fileName)
    {
        std::fstream file;

        file.open(fileName, std::ios::in);
        if (file)
        {
            int materialId;
            std::string line;

            Core::GnomeLoader::Animation animation;
            Core::GnomeLoader::Bone bone;
            Core::GnomeLoader::Material material;	
            Core::GnomeLoader::Vertex vertex;
            Core::GnomeLoader::Header header;

			file >> line;

			file >> line;
			file >> header.numberOfMaterials;
			file >> line;
			file >> header.numberOfVertices;
			file >> line;
			file >> header.numberOfTriangles;
			file >> line;
			file >> header.numberOfBones;
			file >> line;
			file >> header.numberOfAnimations;

            Core::GnomeLoader::Mesh mesh;
            Core::GnomeLoader::Gnome* gnome = new Core::GnomeLoader::Gnome;

			mesh.vertices = new Core::GnomeLoader::Vertex[header.numberOfVertices];
            gnome->materials = new Core::GnomeLoader::Material[header.numberOfMaterials];
            gnome->numberOfVertices = header.numberOfVertices;
            gnome->bones = new Core::GnomeLoader::Bone[header.numberOfBones];
            gnome->animations = new Core::GnomeLoader::Animation[header.numberOfBones];


			//Material
			file >> line;
			for (int i = 0; i < header.numberOfMaterials; ++i)
			{
				file >> material.name;
				file >> line;
				file >> material.ambient[0] >> material.ambient[1] >> material.ambient[2];
				file >> line;
				file >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2];
				file >> line;
				file >> material.specularity[0] >> material.specularity[1] >> material.specularity[2];
				file >> line;
				file >> material.specularityPower;
				file >> line;
				file >> material.reflectivity;
				file >> line;
				file >> material.transparency;
				file >> line;
				file >> material.alphaClip;
				file >> line;
				file >> material.diffuseTexture;
				file >> line;
				file >> material.normalMap;
				file >> line;
				file >> material.alphaMap;

				gnome->materials[i] = material;
			}
           
			std::cout << "Parsed material" << std::endl;

            file >> line;
            file >> line >> materialId; //todo: make dynamic solution for mutiple materials
            vertex.materialId = materialId;
            for (int i = 0; i < header.numberOfVertices; ++i)
            {
                file >> line;
                file >> vertex.position[0]	 >> vertex.position[1]	 >> vertex.position[2];
                file >> line;
                file >> vertex.normal[0]	 >> vertex.normal[1]	 >> vertex.normal[2];
                file >> line;
                file >> vertex.tangent[0]	 >> vertex.tangent[1]	 >> vertex.tangent[2];
                file >> line;
                file >> vertex.binormal[0]	 >> vertex.binormal[1]	 >> vertex.binormal[2];
                file >> line;
                file >> vertex.uv[0]		 >> vertex.uv[1];
                file >> line;
                file >> vertex.boneWeight[0] >> vertex.boneWeight[1] >> vertex.boneWeight[2] >> vertex.boneWeight[3];
                file >> line;
                file >> vertex.boneIndex[0]	 >> vertex.boneIndex[1]	 >> vertex.boneIndex[2]	 >> vertex.boneIndex[3];

                mesh.vertices[i] = vertex;
            }

			std::cout << "Parsed verts" << std::endl;

            //Bones
            // file >> line;
		    //
            // for (int k = 0; k < header.numberOfBones; ++k)
            // {
            //     file >> bone.Name;
            //     for (int i = 0; i < 4; i++)
            //     {
            //         for (int j = 0; j < 4; j++)
            //         {
            //             file >> bone.offsetMatrix[i][j];
            //         }
            //     }
            //     bones[k] = bone;
            // }
            // 
            // file >> line;
            // for (int i = 0; i < header.numberOfBones; ++i)
            // {
            //     file >> line;
            //     file >> bones[i].parentID;
            //     bones[i].id = i;
            // }
		    //
            // //Animations
            // file >> line;
            // file >> line;
		    //
            // for (int i = 0; i < header.numberOfAnimations; ++i)
            // {
            //     file >> line;
            //     file >> animation.name;
            //     file >> line;
		    //
            //     for (int j = 0; j < header.numberOfBones; ++j)
            //     {
            //         int noKeys;
            //         file >> line >> line >> noKeys;
            //         file >> line;
            //         
            //         std::vector<Keyframe> keys; //TODO: unvectorize //c:\users\alice\downloads\flag.gnome
            //         for (int k = 0; k < noKeys; k++)
            //         {
            //             Keyframe keyframe;
            //             file >> line;
            //             file >> keyframe.time;
            //             file >> line;
            //             file >> keyframe.position[0] >> keyframe.position[1] >> keyframe.position[2];
            //             file >> line;
            //             file >> keyframe.scale[0]	 >> keyframe.scale[1]	 >> keyframe.scale[2]; 
            //             file >> line;
            //             file >> keyframe.rotation[0] >> keyframe.rotation[1] >> keyframe.rotation[2] >> keyframe.rotation[3];
		    //
		    //
            //             keys.push_back(keyframe);
            //         }
            //         file >> line;
		    //
            //         animations[i].keyframes.push_back(keys);
            //         animations[i].namedKeyframes[bones[j].Name] = keys;
            //     }
            // }

            //Apply data to GFX buffers
            gnome->indices = new int[gnome->numberOfVertices];           
            gnome->vertices = new GFX::StaticVertex[gnome->numberOfVertices];
            gnome->numberOfIndices = gnome->numberOfVertices;
            for (int i = 0; i < gnome->numberOfVertices; i++)
            {
                gnome->indices[i] = i;
            }
            
            for (int i = 0; i < gnome->numberOfVertices; i++)
            {
                GFX::StaticVertex v;
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].position[j] = mesh.vertices[i].position[j];
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].normal[j] = mesh.vertices[i].normal[j];
                for (int j = 0; j < 2; j++)
                {
                    //if (j == 1)
                    //	vertices[i].uv[j] = 1 - mesh.vertices[i].uv[j];
                    //else
                        gnome->vertices[i].uv[j] = mesh.vertices[i].uv[j];
                }
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].tangent[j] = mesh.vertices[i].tangent[j];
                for (int j = 0; j < 3; j++)
                    gnome->vertices[i].binormal[j] = mesh.vertices[i].binormal[j];

                gnome->vertices[i].position[3] = 1.0f;
                gnome->vertices[i].normal[3] = 0.0f;
                gnome->vertices[i].tangent[3] = 0.0f;
                gnome->vertices[i].tangent[3] = 0.0f;
            }
            delete[] mesh.vertices;

            std::cout << "parsed data" << std::endl;
            file.close();
            return gnome;
        }
        file.close();
        return nullptr;
    }
}
