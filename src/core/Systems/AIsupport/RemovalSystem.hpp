#ifndef SRC_CORE_SYSTEMS_REMOVAL_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_REMOVAL_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

#include <gfx/BitmaskDefinitions.hpp>
#include <glm/glm.hpp>
namespace Core
{
	class RemovalSystem : public BaseSystem
	{
	public:
		RemovalSystem();

		virtual void Update(float delta) override;


		virtual const char* GetHumanName(){ return "RemovalSystem"; }

	private:
		bool m_foundAssets;
		int m_mesh;
		int m_material;
		int m_materialTwo;
		GFX::GFXBitmask m_bloodBitmask;
		GFX::GFXBitmask m_bloodBitmaskTwo;

		void CreateBloodDecal(glm::vec3 position);

		std::vector<Entity> m_bloodEntities;

		inline float rand_FloatRange(float a, float b)
		{
			return ((b - a)*((float)rand() / RAND_MAX)) + a;
		}
	};
}

#endif
