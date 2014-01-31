#ifndef CORE_COMPONENTS_FORMATION_COMPONENT_HPP
#define CORE_COMPONENTS_FORMATION_COMPONENT_HPP

namespace Core
{
	struct FormationComponent
	{
		float relativePosition[2];
		bool isStraggler = false;

		FormationComponent()
		{
			relativePosition[0] = 0.0f;
			relativePosition[1] = 0.0f;
		}

		FormationComponent(float relativeX, float relativeY)
		{
			relativePosition[0] = relativeX;
			relativePosition[1] = relativeY;
		}

		inline static const char* GetName()
		{
			return "FormationComponent";
		}
	};
}

#endif
