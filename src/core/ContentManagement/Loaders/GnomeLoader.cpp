#include "GnomeLoader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>
#include <Animation/AnimationManager.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/ext.hpp>

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
			LOG_ERROR << "NO ASYNC ANIMATION LOADING YET, PLEASE IMPLEMENT THIS! BLAME JAOEL /Jaoel";

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
	void Core::GnomeLoader::InterpolateBoneAnimation(int animationIndex, const Core::GnomeLoader::Gnome* gnome, float time, std::vector<glm::mat4x4>& boneTransforms)
	{
		for (int b = 0; b < gnome->numberOfBones; b++)
		{
			glm::mat4x4 &M = boneTransforms[b];
			glm::vec3 S, P;
			glm::quat Q;

			Core::GnomeLoader::BoneForAnimation &boneAnim = gnome->animations[animationIndex].boneAnim[b];
			

			int numKeys = boneAnim.numKeys;
			if (time <= boneAnim.Keyframes[0].time)
			{
				S = glm::vec3(
							boneAnim.Keyframes[0].scale[0],
							boneAnim.Keyframes[0].scale[1],
							boneAnim.Keyframes[0].scale[2]);
					
				P = glm::vec3(
							boneAnim.Keyframes[0].position[0],
							boneAnim.Keyframes[0].position[1],
							boneAnim.Keyframes[0].position[2]);
					
				Q = glm::quat(
							boneAnim.Keyframes[0].rotation[0], 
							boneAnim.Keyframes[0].rotation[1],
							boneAnim.Keyframes[0].rotation[2],
							boneAnim.Keyframes[0].rotation[3]);
			}
			else if (time >= boneAnim.Keyframes[numKeys-1].time)
			{
				S = glm::vec3(
							boneAnim.Keyframes[numKeys-1].scale[0],
							boneAnim.Keyframes[numKeys-1].scale[1],
							boneAnim.Keyframes[numKeys-1].scale[2]);
					
				P = glm::vec3(
							boneAnim.Keyframes[numKeys-1].position[0],
							boneAnim.Keyframes[numKeys-1].position[1],
							boneAnim.Keyframes[numKeys-1].position[2]);
					
				Q = glm::quat(
							boneAnim.Keyframes[numKeys-1].rotation[0], 
							boneAnim.Keyframes[numKeys-1].rotation[1],
							boneAnim.Keyframes[numKeys-1].rotation[2],
							boneAnim.Keyframes[numKeys-1].rotation[3]);
			}
			else
			{
				for (int i = 0; i < boneAnim.numKeys; i++)
				{
					if (time >= boneAnim.Keyframes[i].time && time <= boneAnim.Keyframes[i + 1].time)
					{
						float lerpPercent = (time - boneAnim.Keyframes[i].time) / (boneAnim.Keyframes[i+1].time - boneAnim.Keyframes[i].time);
						
						glm::vec3 s0 = glm::vec3(
							boneAnim.Keyframes[i].scale[0],
							boneAnim.Keyframes[i].scale[1],
							boneAnim.Keyframes[i].scale[2]);
						glm::vec3 s1 = glm::vec3(
							boneAnim.Keyframes[i+1].scale[0],
							boneAnim.Keyframes[i+1].scale[1],
							boneAnim.Keyframes[i+1].scale[2]);
						
						glm::vec3 p0 = glm::vec3(
							boneAnim.Keyframes[i].position[0],
							boneAnim.Keyframes[i].position[1],
							boneAnim.Keyframes[i].position[2]);
						glm::vec3 p1 = glm::vec3(
							boneAnim.Keyframes[i+1].position[0],
							boneAnim.Keyframes[i+1].position[1],
							boneAnim.Keyframes[i+1].position[2]);
						
						glm::quat q0 = glm::quat(
							boneAnim.Keyframes[i].rotation[0],
							boneAnim.Keyframes[i].rotation[1], 
							boneAnim.Keyframes[i].rotation[2],
							boneAnim.Keyframes[i].rotation[3]
							);
						glm::quat q1 = glm::quat(
							boneAnim.Keyframes[i+1].rotation[0],
							boneAnim.Keyframes[i+1].rotation[1], 
							boneAnim.Keyframes[i+1].rotation[2],
							boneAnim.Keyframes[i+1].rotation[3]
							);
						
						S = glm::mix(s0, s1, lerpPercent);
						P = glm::mix(p0, p1, lerpPercent);
						Q = glm::slerp(q0, q1, lerpPercent);

						break;
					}
				}
			}

			glm::mat4x4 rotMatrix = glm::toMat4(Q);
			glm::mat4x4 transMatrix =  glm::translate(glm::mat4x4(1.0f), P);
			glm::mat4x4 scaleMatrix =	glm::scale(glm::mat4x4(1.0f), S);
			
			M = transMatrix * rotMatrix * scaleMatrix;
			//M = rotMatrix;
		}
	}
	void Core::GnomeLoader::GetFinalTransforms(int animationIndex, const Core::GnomeLoader::Gnome* gnome, float time, std::vector<glm::mat4x4>& finalTransforms)
	{
		int numBones = gnome->numberOfBones;
		
		if(finalTransforms.size() < numBones)
			finalTransforms.resize(numBones);
		
		std::vector<glm::mat4x4> toParentTransforms = std::vector<glm::mat4x4>(numBones);
		std::vector<glm::mat4x4> toRootTransforms = std::vector<glm::mat4x4>(numBones);

		InterpolateBoneAnimation(animationIndex, gnome, time, toParentTransforms);

		toRootTransforms[0] = toParentTransforms[0];

		for (int i = 1; i < numBones; i++)
		{
			glm::mat4x4 toParent = toParentTransforms[i];

			int parentIndex = gnome->bones[i].parentID;
			if (parentIndex >= 0)
			{
				glm::mat4x4 parentToRoot = toRootTransforms[parentIndex];

				glm::mat4x4 toRoot = toParent * parentToRoot;

				toRootTransforms[i] = toRoot;
			}
			else
			{
				toRootTransforms[i] = glm::mat4x4(
					1.0f, 0.0f, 0.0f, 0.0f, 
					0.0f, 1.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f, 
					0.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		for (int i = 0; i < numBones; i++)
		{
			glm::mat4x4 offset = glm::mat4x4(
							gnome->bones[i].offsetMatrix[0][0], gnome->bones[i].offsetMatrix[1][0], gnome->bones[i].offsetMatrix[2][0], gnome->bones[i].offsetMatrix[3][0],
							gnome->bones[i].offsetMatrix[0][1], gnome->bones[i].offsetMatrix[1][1], gnome->bones[i].offsetMatrix[2][1], gnome->bones[i].offsetMatrix[3][1],
							gnome->bones[i].offsetMatrix[0][2], gnome->bones[i].offsetMatrix[1][2], gnome->bones[i].offsetMatrix[2][2], gnome->bones[i].offsetMatrix[3][2],
							gnome->bones[i].offsetMatrix[0][3], gnome->bones[i].offsetMatrix[1][3], gnome->bones[i].offsetMatrix[2][3], gnome->bones[i].offsetMatrix[3][3]
							);
			glm::mat4x4 toRoot = toRootTransforms[i];
			finalTransforms[i] = offset * toRoot;
		}

	}

	void Core::GnomeLoader::LoadAnimations(const Core::GnomeLoader::Gnome* gnome, const unsigned int& meshID)
	{
		int skeletonID = -1;
		if (GFX::Content::CreateSkeleton(skeletonID) == GFX_SUCCESS
			&& GFX::Content::BindSkeletonToMesh(meshID, skeletonID) == GFX_SUCCESS)
		{
			for (int a = 0; a < gnome->numberOfAnimations; ++a)
			{
				LOG_INFO << "Loading animation \'" << gnome->animations[a].name << "\'";

				
				std::vector<glm::mat4x4> frames;
				float duration = 0.0f;
				
				// Find animation duration
				for (int e = 0; e < gnome->numberOfBones; e++)
				{
					for (int k = 0; k < gnome->animations[a].boneAnim[e].numKeys; k++)
					{
						duration = std::max(duration, gnome->animations[a].boneAnim[e].Keyframes[k].time);
					}
				}
				int numFrames = 1 + duration * GFX::Settings::GetAnimationFramerate();
				numFrames *= 1;

				// Create frames
				for (int f = 0; f < numFrames; f++)
				{
					float time = duration * (f/float(numFrames-1));
					std::vector<glm::mat4x4> transforms;
					GetFinalTransforms(a, gnome, time, transforms);
					for (int t = 0; t < transforms.size(); t++)
						frames.push_back(transforms[t]);
				}
				

				// If everything was successful, move the animation to GFX and store its name to make it easier to use later
				int result = GFX::Content::AddAnimationToSkeleton(skeletonID, frames.data(), numFrames, gnome->numberOfBones);

				if (result == GFX_INVALID_ANIMATION)
					LOG_ERROR << "Could not add animation \'" << gnome->animations[a].name << "\' Animation is invalid.";
				else if (result == GFX_INVALID_SKELETON)
					LOG_ERROR << "Could not add animation \'" << gnome->animations[a].name << "\' Skeleton with ID " << skeletonID << " does not exist.";
				else
					AnimationManager::StoreAnimationID(meshID, result, gnome->animations[a].name);
			}
		}
	}


    Core::AssetHandle GnomeLoader::Load(const char* assetName)
    {        
        Core::ModelData* modelData = new Core::ModelData();
        Core::GnomeLoader::Gnome* gnome = LoadGnomeFromFile(assetName);

        if(gnome != nullptr)
        {
            GFX::Content::LoadMesh(modelData->meshID, gnome->numberOfVertices, gnome->numberOfIndices, gnome->vertices, gnome->indices);


			// Load the animations for this mesh
			if (gnome->numberOfBones)
				LoadAnimations(gnome, modelData->meshID);

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
        if (m_file.good())
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
								m_animationFile.read((char*)&gnome->animations[i].nameSize, sizeof(int));
								char* tmp = new char[gnome->animations[i].nameSize];
								m_animationFile.read((char*)tmp, gnome->animations[i].nameSize);
								gnome->animations[i].name = std::string(tmp);
								delete[] tmp;
								gnome->animations[i].boneAnim = new BoneForAnimation[header.numberOfBones];

								for (int j = 0; j < header.numberOfBones; ++j)
								{
									m_animationFile.read((char*)&gnome->animations[i].boneAnim[j].numKeys, sizeof(int));
									gnome->animations[i].boneAnim[j].Keyframes = new Keyframe[gnome->animations[i].boneAnim[j].numKeys];
									m_animationFile.read((char*)gnome->animations[i].boneAnim[j].Keyframes, sizeof(Keyframe)* gnome->animations[i].boneAnim[j].numKeys);
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