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


		FlowfieldComponent()
			: node(0)
		{
		}

		inline static const char* GetName()
        {
            return "FlowfieldComponent";
        }

	};
}
#endif
