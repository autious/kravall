#ifndef CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_NAVIGATION_MESH_HPP
#define CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_NAVIGATION_MESH_HPP

#include <fstream>
#include <glm/glm.hpp>

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
					Normal for the respective edge.
				*/
				float normal[3];
				
			} corners[4];
		};

		/*!
			Struct containing a list of direction vectors with the same size as Core::NavigationMesh::nrNodes.
		*/
		struct Flowfield
		{
			int* edges;
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


		/*!
			Will allocate said number of flowfiled instances from the level allocator.
		*/
		void InitFlowfieldInstances( int nrFlowfieldInstances = 0 );

		/*!
			Will calculate the linksToEdge values for the nodes.
		*/
		void CalculateLinks();

		// NavigationMesh utility...
		/*!
			Returns true if point is inside node.
		*/
		bool CheckPointInsideNode( glm::vec2 point, int node );


        /*!
            Function for checking if a point is on the navigation mesh.
            \param point The point to check if inside the navigation mesh.
            \return Returns true if the given point is inside the navigation mesh, otherwise returns false.    
        */
        bool CheckPointInsideNavigationMesh( glm::vec2 point );
       

		/*!
			If point is inside a node the flowfield to get to that node will be calculated for the respective group.
			Returns false if no field is calculated.
		*/
		bool CalculateFlowfieldForGroup( glm::vec2 point, int group );

		/*!
			Uses the GFX debug system to draw the outlines of the navigation mesh.
		*/
		void DrawDebug();
	};


}

std::fstream& operator>> ( std::fstream& ff, Core::NavigationMesh::Node& node );


#endif
