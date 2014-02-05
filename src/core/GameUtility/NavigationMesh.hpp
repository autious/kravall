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

	struct PathData
	{
		PathData() { node = -1; }
		PathData( int node, int entryEdge, glm::vec3 point )
		: node(node), entryEdge(entryEdge), point(point) {}

		int node;
		int entryEdge;
		glm::vec3 point;
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
				if above zero the respective edge cannot be traversed in the navmesh.
			*/
			float blocked[4];

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


			/*!
				Will return a vector to the mid-point of the respective edge.
			*/
			glm::vec3 GetMidPoint( int edge )
			{
				int ii = edge * 2;
				int oo = (ii + 2) % 8;
				return glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] ) + 
					(glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] ) - 
					glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] )) * 0.5f;
			}
		};

		/*!
			Struct containing a list of direction vectors and from-goal entry edges with the same size as Core::NavigationMesh::nrNodes.
			Temporarily contains group metadata as well.
		*/
		struct Flowfield
		{
			/* Group Data */

			float timeSinceLastCheck;
			float recordedPosition[2];
			


			/* ********* */


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
		bool CheckPointInsideNode( const glm::vec3& point, int node );

        /*!
            Gets the closest point on the navmesh on the line between origin and point. The origin has to be inside the mesh 
            \param point InOut parameter, will be changed to the closest point inside the navmesh.
            \param origin The start point from where the nearest point will be found.
            \param node The index of the node that origin is inside of.
			\param goalNode The index of the node that the origin is residing in. Will be set to -1 if point is outside mesh.
			\param fromBorder The fudge factor, positon will be offsetted this far from the edge of the navigation mesh.
            \return True if a new closest point is found. False if origin is not inside the mesh or if a closer point was not found.
        */
        bool GetClosestPointInsideMesh( glm::vec3& point, const glm::vec3& origin, int& goalNode, float fromBorder ); 

        /*!
            Gets the node that the speciefied point is inside of. If multiple nodes contain the point the first one found will be returned.
            \param node Out parameter, the node that the point is inside of will be returned through this parameter.
            \param point The point to retrieve the containing node.
            \returns Returns true if a containing node is found, false otherwise. 
        */
        bool GetNodeForPoint(int& node, const glm::vec3& point);
       

        /*!
            Function for checking if a point is on the navigation mesh.
            \param point The point to check if inside the navigation mesh.
            \return Returns true if the given point is inside the navigation mesh, otherwise returns false.    
        */
        bool CheckPointInsideNavigationMesh( const glm::vec3& point );

		/*!
			If point is inside a node the flowfield to get to that node will be calculated for the respective group.
			Returns false if no field is calculated.
		*/
		bool CalculateFlowfieldForGroup(const glm::vec3& point, int group );


		/*!
			Will allocate frame memory for Astar algorithm. Must be run before use of CalculateShortPath function.
			Will reserve memory according to the currently loaded navigation mesh and will return false if heap allocation fails.
		*/
		bool AllocateFrameMemoryForAstar();

		/*!
			Used for single unit pathfinding. AllocateFrameMemoryForAstar() must be run once before any calls to this function. 
		*/
		PathData CalculateShortPath( int ownNode, glm::vec3 ownPosition, int otherNode, glm::vec3 otherPosition );

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

		private : 

			Flowfield tempField;
			bool* visited;
			float* distances;
			glm::vec3* points;
	};
}

std::fstream& operator>> ( std::fstream& ff, Core::NavigationMesh::Node& node );



#endif
