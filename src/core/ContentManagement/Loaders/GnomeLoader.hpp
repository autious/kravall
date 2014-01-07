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
			int numberOfAnimations;
		};

		struct Bone
		{
			int parentID;
			std::string Name;
			int id;
			float offsetMatrix[4][4];
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
			int fps;
			int id;
			BoneForAnimation* boneAnim;
		};


		Animation* animations;

		struct VertexBoneAnimated
		{
			//Ordinary stuff
			float position[3];
			float normal[3];

			//Normalmapping
			float tangent[3];
			float uv[2];

			//Animation
			float boneWeight[4];
			int boneIndex[4];
		};

		struct Vertex
		{
			//Ordinary stuff
			float position[3];
			float normal[3];

			//Normalmapping
			float tangent[3];
			float uv[2];
		};

		struct Mesh
		{
			int* indices;
			Vertex* vertices;
			VertexBoneAnimated* animatedVertices;
		};

		struct MorphKey
		{
			float keyTime;
			Vertex* vertices;
		};

		struct MorphAnimation
		{
			int id;
			int numberOfKeys;
			MorphKey* keys;
		};

		//Animation* animations;
		Bone* bones;
		Header header;
		//Mesh mesh;
		MorphAnimation* morphAnimation;

        struct Gnome
        {
            int numberOfVertices;
            GFX::StaticVertex* vertices;
            int numberOfIndices;
            int* indices;
            Core::GnomeLoader::Bone* bones;
            Core::GnomeLoader::Animation* animations;
            //Core::GnomeLoader::Material* materials;
        };

        Core::GnomeLoader::Gnome* LoadGnomeFromFile(const char* fileName);
	};
}

#endif
