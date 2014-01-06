#ifndef CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_NAVIGATION_MESH_HPP
#define CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_NAVIGATION_MESH_HPP

#include <fstream>
#include <glm/glm.hpp>

namespace Core
{
	/*!
		Data structure for the navigation mesh. 
	*/
	struct NavigationMesh
	{	
		struct Node
		{
			/*!
				The x and z coordinates for the corners of the quad/triangle.
			*/
			float points[8];

			/*!
				Metadata for each node. Each line is n to n + 1, with n to 0 for the last index.
			*/
			struct Corner
			{
				/*!
					Node list index as to which node you get into from crossing the line 
					fromed by n and n + 1. Loops around for the last index.
				*/
				int linksTo;
				/*!
					The distance from this corner to the n + 1 corner. Loops around for the last index.
				*/
				float length;
				/*!
					Inverse of the distance from this corner to the n + 1 corner.
					Loops around for the last index.
				*/
				float inverseLength;
			} corners[4];
		};

		/*!
			Struct containing a list of direction vectors with the same size as Core::NavigationMesh::nrNodes.
		*/
		struct Flowfield
		{
			glm::vec3* list;
		};

		/*!
			The list of all the flowfield instances available.
		*/
		Flowfield* flowfields;
		
		/*!
			Number of flowfields in use.
		*/
		int nrUsedFlowfields;

		/*!
			Number of flowfields allocated.
		*/
		int maxFlowfields;

		/*!
			Node list for the loaded tree.
		*/
		Node* nodes;

		/*!
			current number of nodes reciding under the nodes-pointer.
		*/
		int nrNodes;
	};

	/*!
		Get a pointer to the current instance of the navigaion mesh. Will return nullprt if no mesh is loaded.
	*/
	NavigationMesh* GetNavigationMesh();

	/*!
		will set the Core::GetNavigationMesh pointer to point to this object.
	*/
	void SetNavigationMesh( Core::NavigationMesh* mesh );

	/*!
		Will allocate said number of flowfiled instances from the level allocator.
	*/
	void InitFlowfieldInstances( Core::NavigationMesh* mesh, int nrFlowfieldInstances = 0 );
	
}

std::fstream& operator>> ( std::fstream& ff, Core::NavigationMesh::Node& node );


#endif
