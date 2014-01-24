#ifndef CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_NAVIGATION_MESH_HPP

#define CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_NAVIGATION_MESH_HPP

#include <fstream>
#include <glm/glm.hpp>
#include "Console/CLOP.hpp"

#define MAX_NUMBER_OF_FLOWFIELDS 1000
#define NAVMESH_NO_CONNECTING_CORNERS -1
#define NAVMESH_CONCAVE_CORNER_NODE -2


namespace Core
{
	struct NavigationMesh;

	/*!
		Get a pointer to the current instance of the navigaion mesh. Will return nullprt if no mesh is loaded.
	*/
	NavigationMesh* GetNavigationMesh();

	/*!
		will set the Core::GetNavigationMesh pointer to point to this object.
	*/
	void SetNavigationMesh( Core::NavigationMesh* mesh );

	/*!
		Will toggle drawing of the navigation mesh.
	*/
	void ToggleDrawOfNavigationMesh(clop::ArgList args);

	/*!
		Will use the GFX debug system to draw the navigation mesh if loaded.
	*/
	void DrawToggledNavigationMesh();

	/*!
		Data structure for the navigation mesh. 
	*/
	struct NavigationMesh
	{	

		NavigationMesh()
		{
		}

		~NavigationMesh()
		{
			Core::SetNavigationMesh( nullptr );
		}

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
					The respective edge of the node the current edge leads to.
				*/
				int linksToEdge;
				/*!
					The distance from this corner to the n + 1 corner. Loops around for the last index.
					Negative if edge is not valid ( last corner if triangle ).
				*/
				float length;
				/*!
					Inverse of the distance from this corner to the n + 1 corner.
					Loops around for the last index.
				*/
				float inverseLength;
				/*!
					2D Normal for the respective edge.
				*/
				float normal[2];

				/*
					This corner links to the following edge and node.
				*/
				int cornerConnectsToNode;

				/*
					This corner links to the following edge and node.
				*/
				int cornerConnectsToCorner;
				
			} corners[4];
		};

		/*!
			Struct containing a list of direction vectors and from-goal entry edges with the same size as Core::NavigationMesh::nrNodes.
		*/
		struct Flowfield
		{
			float goal[2];
			int* edges;
			glm::vec3* list;
		};

        /*!
            Creates a group and allocates memory for flowfields for the group.
        */
        int CreateGroup();

		/*!
			Will allocate flowfields from the level allocator.
		*/
		void InitFlowfieldInstances();

		/*!
			Will calculate the linksToEdge values for the nodes.
		*/
		void CalculateLinks();

		// NavigationMesh utility...
		/*!
			Returns true if point is inside node.
		*/
		bool CheckPointInsideNode( glm::vec3 point, int node );

        /*!
            Function for checking if a point is on the navigation mesh.
            \param point The point to check if inside the navigation mesh.
            \return Returns true if the given point is inside the navigation mesh, otherwise returns false.    
        */
        bool CheckPointInsideNavigationMesh( glm::vec3 point );

		/*!
			If point is inside a node the flowfield to get to that node will be calculated for the respective group.
			Returns false if no field is calculated.
		*/
		bool CalculateFlowfieldForGroup( glm::vec3 point, int group );

		/*!
			Uses the GFX debug system to draw the outlines of the navigation mesh.
		*/
		void DrawDebug();

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
}

std::fstream& operator>> ( std::fstream& ff, Core::NavigationMesh::Node& node );



#endif
