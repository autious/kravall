#ifndef SRC_CORE_SYSTEMS_FIELD_REACTION_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_FIELD_REACTION_SYSTEM_HPP

#include <SystemDef.hpp>

namespace Core
{
	/*!
		System for handling the movement of moving objects. Moving objects are defined by a WorldPositionComponent and
		a MovementComponent.
	*/
	class FieldReactionSystem : public BaseSystem
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
		FieldReactionSystem();

		/*!
			The name of the system for printing.
			\return The system's name.
		*/
		virtual const char* GetHumanName() { return "FieldReactionSystem"; }

	private:
		static const int FIELD_SIDE_LENGTH = 512;
		static const int FIELD_SIDE_CELL_COUNT = 32;

		float m_field[FIELD_SIDE_CELL_COUNT][FIELD_SIDE_CELL_COUNT];
		bool m_showPF;

		float GetEffectOnAgent(WorldPositionComponent* agentPos);
		float GetChargeAt(Entity chargedAgent, glm::vec3 queryPosition);
	};
}

#endif