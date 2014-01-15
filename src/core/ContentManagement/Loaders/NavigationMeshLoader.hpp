#ifndef CORE_CONTENT_MANAGEMENT_LOADERS_NAVIGATION_MESH_LOADER_HPP
#define CORE_CONTENT_MANAGEMENT_LOADERS_NAVIGATION_MESH_LOADER_HPP

#include <vector>
#include <map>

#include <ContentManagement/Loaders/BaseAssetLoader.hpp>
#include <ContentManagement/AssetStructs/ModelData.hpp>

#include <GameUtility/NavigationMesh.hpp>


namespace Core
{
    class NavigationMeshLoader : public BaseAssetLoader
    {
    public:
        NavigationMeshLoader();
        ~NavigationMeshLoader();

        AssetHandle Load(const char* assetName) override;
        AssetHandle LoadAsync(const char* assetName) override;
        void FinishLoadAsync(Core::AssetHandle& handle) override;
        void Destroy(const Core::AssetHandle handle) override;
		bool UseReferenceCounting() override;

        const Core::ModelData* getData(const Core::AssetHandle handle) const;

    private:
        std::vector<Core::ModelData*> m_modelData;

		NavigationMesh* LoadNavigationMeshFromFile( const char* path );

    };
}

#endif
