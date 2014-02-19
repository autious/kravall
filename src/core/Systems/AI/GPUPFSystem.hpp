#ifndef SRC_CORE_SYSTEMS_GPUPF_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_GPUPF_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Core
{
	/*!
	System for handling the movement of moving objects. Moving objects are defined by a WorldPositionComponent and
	a MovementComponent.
	*/
	class GPUPFSystem : public BaseSystem
	{

	public:
		/*!
		System update function. All logic for the system is placed here.
		\param delta The delta time for the frame.
		*/
		virtual void Update(float delta) override;

		/*!
		Default constructor creating a system. The system works on entities containing a WorldPositioncomponent and
		a MovementComponent.
		*/
		GPUPFSystem();

		/*!
		The name of the system for printing.
		\return The system's name.
		*/
		virtual const char* GetHumanName() { return "GPUPFSystem"; }

	private:
		
		struct DataIN
		{
			glm::vec4 position_unitType;
			glm::vec4 newDirection_speed;
			glm::vec4 health_stamina_morale_stancealignment;
			glm::vec4 groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance;
		};

		struct DataOUT
		{
			glm::vec4 newDirection_speed;
			glm::vec4 morale_rage_pressure_empty;
		};

		GLuint m_shaderID;
		GLuint m_inDataBuffer;
		GLuint m_outDataBuffer;

		const unsigned int MAXIMUM_ENTITIES = 2048;

		DataIN* m_inData;
		DataOUT* m_outData;
		GLint m_entityCount;

		bool m_foundShader;
		bool m_readBack;
		bool m_dispatch;

		GLsync m_sync;
	};
}

#endif
