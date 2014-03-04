#ifndef SRC_CORE_COMPONENTS_BLOOD_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_BLOOD_COMPONENT_HPP

namespace Core
{
	struct BloodComponent
	{
		bool expand;
		float scaleSpeed[2];
		float maxSize;

		static BloodComponent BC()
		{
			BloodComponent bc;
			bc.expand = true;
			bc.scaleSpeed[0] = 0;
			bc.scaleSpeed[1] = 0;
			bc.maxSize = 0;
			return bc;
		}

		inline static const char* GetName()
		{
			return "BloodComponent";
		}
	};
}

#endif