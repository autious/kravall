#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_GNOME_LOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_GNOME_LOADER_HPP

#include <vector>
#include <map>

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>
#include <ContentManagement/AssetStructs/ModelData.hpp>

#include <gfx/GFXInterface.hpp>

namespace Core
{
    class GnomeLoader : public BaseAssetLoader
    {
    public:
        GnomeLoader();
        ~GnomeLoader();

        AssetHandle Load(const char* assetName) override;
        AssetHandle LoadAsync(const char* assetName) override;
        void FinishLoadAsync(Core::AssetHandle& handle) override;
        void Destroy(const Core::AssetHandle handle) override;

        const Core::ModelData* getData(const Core::AssetHandle handle) const;

    private:
        std::vector<Core::ModelData*> m_modelData;

		struct Header
		{
			int numberOfVertices;
			int numberOfTriangles;
			int numberOfIndices;
			int numberOfBones;
			//int numberOfAnimations;
		};

		struct AnimationHeader
		{
			int numberOfBones;
			int numberOfAnimations;
		};

		struct Bone
		{
            int id;
			int parentID;
            int nameSize;
			float offsetMatrix[4][4];
			std::string name;
		};

		struct Keyframe
		{
			float time;
			float position[3];
			float scale[3];
			float rotation[4];
		};

		struct BoneForAnimation
		{
			int numKeys;
			Keyframe* Keyframes;
		};

		struct Animation
		{
			//int fps;
			int nameSize;
			std::string name;
			BoneForAnimation* boneAnim;
			
		};

		struct VertexBoneAnimated
		{
			//Ordinary stuff
			float position[4];
			float normal[4];

			//Normalmapping
			float tangent[4];

			//Animation
			int boneIndex[4];
			float boneWeight[4];
			
			//uv
			float uv[2];
		};

        struct Gnome
        {
            int numberOfVertices;
            GFX::Vertex* vertices;
            int numberOfIndices;
            int* indices;
            int numberOfBones;
            Core::GnomeLoader::Bone* bones;
            int numberOfAnimations;
            Core::GnomeLoader::Animation* animations;
        };

        Core::GnomeLoader::Gnome* LoadGnomeFromFile(const char* fileName);

		void InterpolateBoneAnimation(int animationIndex, const Core::GnomeLoader::Gnome* gnome, float time, std::vector<glm::mat4x4>& boneTransforms);
		void GetFinalTransforms(int animationIndex, const Core::GnomeLoader::Gnome* gnome, float time, std::vector<glm::mat4x4>& finalTransforms);
		void LoadAnimations(const Core::GnomeLoader::Gnome* gnome, const unsigned int& meshID);
	};
}

#endif
