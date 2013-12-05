#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_GNOME_LOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_GNOME_LOADER_HPP

#include <vector>
#include <map>

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>
#include <ContentManagement/AssetStructs/ModelData.hpp>

namespace Core
{
    class GnomeLoader : public BaseAssetLoader
    {
    public:
        GnomeLoader();
        ~GnomeLoader();

        void* Load(const char* assetName) override;
        void* LoadAsync(const char* assetName, Core::FinisherVector& finisherList, std::mutex& finisherMutex) override;
        void Destroy(const Core::AssetHandle handle) override;

        const Core::ModelData* getData(const Core::AssetHandle handle) const;

    private:

        std::vector<Core::ModelData*> m_modelData;

        struct Header
        {
            int numberOfMaterials;
            int numberOfVertices;
            int numberOfAnimations;
            int numberOfBones;
            int numberOfTriangles;
        };

        struct Bone
        {
            int parentID;
            //std::vector<int> childIDs;
            std::string Name;
            int id;
            float offsetMatrix[4][4];
        };



        struct Keyframe
        {
            float rotation[4];
            float position[3];
            float time;
            float scale[3];
        };

        struct Animation
        {
            int fps;
            std::string name;
            std::vector<std::vector<Keyframe>> keyframes; //per joint
            std::map<std::string,std::vector<Keyframe>> namedKeyframes;
        };

        struct Vertex
        {
            //Materials
            int materialId;

            //Ordinary stuff
            float position[3];
            float normal[3];
            float uv[2];

            //Normalmapping
            float tangent[3];
            float binormal[3];

            //Animation
            float boneWeight[4];
            int boneIndex[4];
        };

        struct Material
        {
            //Identification
            std::string name;
            int id;

            //Numbers
            float ambient[3];
            float diffuse[3];
            float specularity[3];
            float specularityPower;
            float reflectivity;
            float transparency;
            bool alphaClip;

            //Textures
            std::string diffuseTexture;
            std::string normalMap;
            std::string alphaMap;
        };

        struct Mesh
        {
            int numberOfVertices;
            Vertex* vertices;
            Material* materials;
        };
    };
}

#endif
