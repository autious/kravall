#ifndef SRC_CORE_COMPONENTS_FLOWFIELDCOMPONENT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_FLOWFIELDCOMPONENT_COMPONENT_HPP



namespace Core
{
	/*!
		Component holding meta for the entity's relation to the navigation mesh.
	*/
	struct FlowfieldComponent
	{
		/*!
			The node the entity is currently colliding with.
		*/
		int node;

		/*!
			The direction index to the closest wall in the navigation mesh. -1 to 1.
		*/
		int wallDirX;

		/*!
			The direction index to the closest wall in the navigation mesh. -1 to 1.
		*/
		int wallDirZ;

		/*!
			The projected distance to the closest wall in the navigation mesh for use in the pf system. This value is recalculated and should not be used as a actual distance.
		*/
		float distance;

		FlowfieldComponent()
			: node(0)
		{
			wallDirX = 0;
			wallDirZ = 0;
			distance = 0;
		}

		inline static const char* GetName()
        {
            return "FlowfieldComponent";
        }

	};
}
#endif
