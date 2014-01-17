#ifndef SRC_CORE_COMPONENTS_COLLISIONCULLING_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_COLLISIONCULLING_COMPONENT_HPP



namespace Core
{
	/*!
		Component holding meta for the entity's relation to the navigation mesh.
	*/
	struct CollisionCullingComponent
	{
		/*!
			The gridIndex
		*/
		int gridIndex;


		CollisionCullingComponent()
			: gridIndex(0)
		{
		}

		inline static const char* GetName()
        {
            return "CollisionCullingComponent";
        }

	};
}
#endif
