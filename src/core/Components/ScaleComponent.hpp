#ifndef SRC_CORE_COMPONENTS_SCALE_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_SCALE_COMPONENT_HPP

namespace Core
{
	struct ScaleComponent
	{
		float scale;

		ScaleComponent() : scale(1.0f)
		{}

		ScaleComponent(float initScale) : scale(initScale)
		{}
	};
}

#endif