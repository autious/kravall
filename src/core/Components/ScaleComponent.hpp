#ifndef SRC_CORE_COMPONENTS_SCALE_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_SCALE_COMPONENT_HPP

namespace Core
{
	struct ScaleComponent
	{
		float scale[3];

		ScaleComponent()
		{
            scale[0] = 1.0f;
            scale[1] = 1.0f;
            scale[2] = 1.0f;
        }

		ScaleComponent(float initScale)
		{
            scale[0] = initScale;
            scale[1] = initScale;
            scale[2] = initScale;
        }

        inline static const char* GetName()
        {
            return "ScaleComponent";
        }
	};
}

#endif
