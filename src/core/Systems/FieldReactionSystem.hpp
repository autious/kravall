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
		/*
			Struct defining a curve for an agent's charge with the charge magnitude, the maximum radius for the charge,
			the radius of the repelling field very close to the agent and the decline rate of the charge based on the
			maximum radius.
		*/
		struct ChargeCurve
		{
			float charge;
			float cutoff;
			float repelRadius;
			float decline;

			ChargeCurve() : charge(0.0f), cutoff(0.0f), repelRadius(0.0f), decline(0.0f) {}

			ChargeCurve(float peakCharge, float cutoffLimit, float repelDistance) : charge(peakCharge),
				cutoff(cutoffLimit), repelRadius(repelDistance), decline(peakCharge / (cutoffLimit - repelDistance)) {}
		};

		static const float STAY_LIMIT;

		static const int FIELD_SIDE_LENGTH = 24;
		static const int FIELD_SIDE_CELL_COUNT = 60;
		static const float FIELD_CELL_SIDE_SIZE;
		static const int FIELD_UPDATE_FRAME_COUNT = 5;
		static const int FIELD_UPDATE_ROW_COUNT = FIELD_SIDE_CELL_COUNT / FIELD_UPDATE_FRAME_COUNT;
		static const ChargeCurve CURVE[1][2];
		
		float m_field[FIELD_SIDE_CELL_COUNT][FIELD_SIDE_CELL_COUNT];
		float m_calculatingField[FIELD_SIDE_CELL_COUNT][FIELD_SIDE_CELL_COUNT];
		bool m_showPF;
		int m_updateCounter;

		void UpdateAgents();
		float GetEffectOnAgentAt(Entity* queryAgent, WorldPositionComponent* queryPosition);
		float GetAgentsChargeAt(Entity chargedAgent, glm::vec3 queryPosition);

		void UpdateDebugField();
		void DrawDebugField();
		void CommitDebugField();

		void ToggleDrawingOfPF();

		glm::vec3 GetPositionFromFieldIndex(int xIndex, int zIndex, float yPos = 0.0f);
	};
}

#endif