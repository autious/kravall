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
		static const int FIELD_SIDE_LENGTH = 24;
		static const int FIELD_SIDE_CELL_COUNT = 60;
		static const float FIELD_CELL_SIDE_SIZE;

		float m_field[FIELD_SIDE_CELL_COUNT][FIELD_SIDE_CELL_COUNT];
		bool m_showPF;

		void UpdateAgents();
		float GetEffectOnAgent(WorldPositionComponent* agentPos);
		float GetChargeAt(Entity chargedAgent, glm::vec3 queryPosition);

		void UpdateDebugField(Entity selectedAgent);
		void DrawDebugField();

		glm::vec3 GetPositionFromFieldIndex(int xIndex, int zIndex, int yPos = 0.0f);
	};
}

#endif