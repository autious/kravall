#version 430 core

#define WORK_GROUP_SIZE 1024
#define MAXIMUM_ENTITIES 2048
#define MAX_CURVES 1024;

#define RIOTER_TYPE 0
#define POLICE_TYPE 1

#define PF_DRAW_DIVIDE_FACTOR 0.0001f
#define PF_FACTOR 0.5f
#define FF_FACTOR 0.5f
#define CAP_POSITIVE 1000000.0f
#define CAP_NEGATIVE -1000000.0f
#define MINIMUM_CHARGE 100.0f  // Is negated later!

#define STAY_LIMIT 0.1f

#pragma optionNV(fastmath on) 
#pragma optionNV(ifcvt none) 
#pragma optionNV(inline all) 
#pragma optionNV(strict on) 
#pragma optionNV(unroll all)


layout (local_size_x = WORK_GROUP_SIZE) in;

struct CalculatedCharge
{
	float chargeSum;
	int x;
	int y;
};

struct ChargeCurve
{
	vec4 ch_cu_re_dec;
};

struct MoodCurve
{
	vec4 range_mo_pre_rage;
};

struct DataIN
{
	vec4 position_unitType;
	vec4 newDirection_speed;
	vec4 health_stamina_morale_stance;
	//vec4 groupSquadID_defenseRage_mobilityPressure_empty;
	vec4 groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance;
};

struct DataOUT
{
	vec4 newDirection_speed;
	vec4 morale_rage_pressure_empty;
};

layout(std430, binding = 0) restrict readonly buffer InputBuffer
{
	DataIN gInput[];
};

layout (std430, binding = 1) restrict writeonly buffer OutputBuffer
{
	DataOUT gOutput[];
};

//layout (std430, binding = 2) restrict readonly buffer MoodCurveBuffer
//{
//	MoodCurve gMoodCurves[];
//}

uniform uint gEntityCount;

shared ChargeCurve gChargeCurves[1][2];
uniform vec4 gMoodCurves[6][13] = {{ vec4(5, -0.001f, 0.01f, 0.01f), vec4(5, 0.0f, 0.0f, 0.01f),vec4(15, 0.0f, 0.5f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(3, 0.0f, 0.0001f, 0.00001f),vec4(5, 0.0f, 0.0002f, 0.0002f),vec4(3, -0.0001f, 0.0001f, 0.0001f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },
									
								   { vec4(5, 0.0f, 0.001f, 0.1f),vec4(5, 0.0f, 0.001f, 0.1f),vec4(15, 0.0f, 0.0005f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(3, 0.0f, 0.00005f, 0.001f),vec4(3, 0.0f, 0.00005f, 0.001f), vec4(5, 0.0f, 0.0001f, 0.0002f),vec4(3, -0.0001f, 0.0001f, 0.0002f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },

								   { vec4(5, 0.0f, 0.01f, 0.1f), vec4(5, 0.0, 0.0, 0.1f), vec4(15, 0.0f, 0.5f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(5, 0.0001f, 0.0001f, 0.0002f), vec4(3, -0.0001f, 0.0001f, 0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },

								   { vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },

								   { vec4(0.0f),vec4(0.0f),vec4(15, 0.0f, 0.5f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(5, 0.0f, 0.005f, 0.001f),vec4(5, 0.0f, 0.001f, 0.002f), vec4(3, -0.0001f, 0.0f, 0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },

								   { vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(1, -0.0001f, 0.0f, 0.0f),vec4(15, -0.001f, 0.0f, 0.0f),vec4(15, -0.001f, 0.0f, 0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) }};

//shared MoodCurve gMoodCurves[6][13];
//int gMoodSenderIndices[2][6];
//int gMoodReceiverIndices[2][6];

uniform int gMoodSenderIndices[2][6] = {{ 6, 7, 8, 9, 10, 0 },
										{ 0, 1, 2, 3, 0, 0	}};

uniform int gMoodReceiverIndices[2][6] = {{ 0, 1, 2, 3, 4, 5 },
										  {	5, 5, 5, 5, 0, 0 }};


float GetAgentChargeAt(int unitType, float distSqr)
{
	ChargeCurve c = gChargeCurves[0][unitType];
	
	return float(distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z) * (-MINIMUM_CHARGE + distSqr * (c.ch_cu_re_dec.z / MINIMUM_CHARGE)) + 
	float(distSqr <= c.ch_cu_re_dec.y) * ( c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w);
}


float GetEffectOnAgentAt(vec2 queryPosition, int groupID)
{
	float positiveSum = 0.0f;
	float negativeSum = 0.0f;
	float currentSum = 0.0f;
	DataIN indata;
	for (int i = 0; i < gInput.length(); ++i)
	{
		if (i > gEntityCount)
			break;
		indata = gInput[i];

		//float dx = gInput[i].position_unitType.x - queryPosition.x;
		//float dz = gInput[i].position_unitType.z - queryPosition.y;
		float dx = indata.position_unitType.x - queryPosition.x;
		float dz = indata.position_unitType.z - queryPosition.y;
		float distSqr = dx * dx + dz * dz;
	
		int matchID = -1;
	
		//if (groupID >= 0)
		{
			matchID = int(indata.groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.x);
		}
	
		if (distSqr >= gChargeCurves[0][0].ch_cu_re_dec.z && (indata.position_unitType.w == RIOTER_TYPE && matchID != groupID) )
			continue;
	
		currentSum = GetAgentChargeAt(int(indata.position_unitType.w), distSqr);
	
		//if(currentSum >= 0)
			positiveSum += currentSum * float((currentSum >= 0));
		//else
			negativeSum += currentSum * float(!(currentSum >= 0));
	
	}

	return positiveSum + negativeSum;
}

//r-prefix = receiever
//s-prefix = sender
vec3 MoodProp(int rType, int rState, float sMorale, float sPressure, float sRage, int sType, int sState, float dist)
{
	int senderIndex = gMoodSenderIndices[sType][sState];
	int receieverIndex = gMoodReceiverIndices[rType][rState];

	vec4 moodCurve = gMoodCurves[receieverIndex][senderIndex];

	//float rMorale = 0;
	//float rPressure = 0;
	//float rRage = 0;

	
	if (dist > moodCurve.x)
		return vec3(0, 0, 0);
	
	float attenuation = 1.0f / (dist * dist) - 1.0f / (moodCurve.x * moodCurve.x); 
	
	//rMorale = moodCurve.y * attenuation * sMorale;
	//rPressure = moodCurve.z * attenuation * sPressure;
	//rRage = moodCurve.w * attenuation * sRage;

	//return vec3(rMorale, rRage, rPressure);
	return vec3( moodCurve.y * attenuation * sMorale, moodCurve.w * attenuation * sRage, moodCurve.z * attenuation * sPressure);
}


vec3 GetMoodOnAgent(vec2 queryPosition, int groupID, int index)
{
	float morale = 0;
	float pressure = 0;
	float rage = 0;

	//float rMorale = gInput[index].health_stamina_morale_stance.z;
	//float rRage = gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.y;
	//float rPressure = gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.z;
	int rType = int(gInput[index].position_unitType.w);
	int rState = int(gInput[index].health_stamina_morale_stance.w);
	//int receieverIndex = gMoodReceiverIndices[rType][rState];

	float sMorale;
	float sRage;
	float sPressure;
	int sType;
	int sState;

	float dist;
	DataIN indata;
	vec3 moods;
	for (int i = 0; i < gInput.length(); ++i)
	{
		if (i >= gEntityCount)
			break;

		if (i == index)
			continue;
		
		indata = gInput[i];

		dist = distance(indata.position_unitType.xyz, vec3(queryPosition.x, 0, queryPosition.y));
		
		
		sMorale = indata.health_stamina_morale_stance.z;
		sRage = indata.groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.y;
		sPressure = indata.groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.z;
		sType = int(indata.position_unitType.w);
		sState = int(indata.health_stamina_morale_stance.w);

		//float dist = distance(gInput[i].position_unitType.xyz, vec3(queryPosition.x, 0, queryPosition.y));
		//
		//
		//float sMorale = gInput[i].health_stamina_morale_stance.z;
		//float sRage = gInput[i].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.y;
		//float sPressure = gInput[i].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.z;
		//int sType = int(gInput[i].position_unitType.w);
		//int sState = int(gInput[i].health_stamina_morale_stance.w);

		moods = MoodProp(rType, rState, sMorale, sPressure, sRage, sType, sState, dist);
		//vec3 moods = MoodProp(receieverIndex, sMorale, sPressure, sRage, sType, sState, dist);
		morale += moods.x;
		rage += moods.y;
		pressure += moods.z;
	}

	return vec3(morale, rage, pressure);
}

void main()
{
/*
	//normal
	gMoodSenderIndices[0][0] = 6;
	gMoodReceiverIndices[RIOTER_TYPE][0] = 0;

	//agitated
	gMoodSenderIndices[RIOTER_TYPE][1] = 7;
	gMoodReceiverIndices[RIOTER_TYPE][1] = 1;
	
	//attacking
	gMoodSenderIndices[RIOTER_TYPE][2] = 8;
	gMoodReceiverIndices[RIOTER_TYPE][2] = 2;
	
	//retreating
	gMoodSenderIndices[RIOTER_TYPE][3] = 9;
	gMoodReceiverIndices[RIOTER_TYPE][3] = 3;
	
	//civilian
	gMoodSenderIndices[RIOTER_TYPE][4] = 10;
	gMoodReceiverIndices[RIOTER_TYPE][4] = 4;
	
	//passive
	gMoodSenderIndices[POLICE_TYPE][0] = 0;
	gMoodReceiverIndices[POLICE_TYPE][0] = 5;
	
	//defensive
	gMoodSenderIndices[POLICE_TYPE][1] = 1;
	gMoodReceiverIndices[POLICE_TYPE][1] = 5;
	
	//aggresive
	gMoodSenderIndices[POLICE_TYPE][2] = 2;
	gMoodReceiverIndices[POLICE_TYPE][2] = 5;
	
	//attacking
	gMoodSenderIndices[POLICE_TYPE][3] = 3;
	gMoodReceiverIndices[POLICE_TYPE][3] = 5;
	*/
	//barrier();
	
	if (gl_LocalInvocationIndex == 0)
	{
		gChargeCurves[0][0].ch_cu_re_dec = vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)); //0.3 makes them stick into a huge blob
		gChargeCurves[0][1].ch_cu_re_dec = vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f));

		//MOOD CURVE INDICES/PLACEMENT CAN BE FOUND AT THE BOTTOM OF THIS DOCUMENT
		/*
		//Init all curves to no effect
		for (int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 13; j++)
			{
				gMoodCurves[i][j].range_mo_pre_rage = vec4(0);
			}
		}

		//barrier();
		//normal rioter vs passive police
		gMoodCurves[0][0].range_mo_pre_rage = vec4(5, -0.001f, 0.01f, 0.01f);

		//normal rioter vs defensive police
		gMoodCurves[0][1].range_mo_pre_rage = vec4(5, 0.0f, 0.0f, 0.01f);

		//normal rioter vs agggresive police
		gMoodCurves[0][2].range_mo_pre_rage = vec4(15, 0.0f, 0.5f, 0.1f);

		//normal rioter vs Attacking police
		gMoodCurves[0][3].range_mo_pre_rage = vec4(15, -0.5f, 5000.0f, 5000.0f);//Attacking state doesn't actually exist yet, fix this

		//normal rioter vs normal rioter
		//No effect
		
		//normal rioter vs rioter agitated
		gMoodCurves[0][7].range_mo_pre_rage = vec4(3, 0.0f, 0.0001f, 0.00001f);
		
		//normal rioter vs rioter attacking
		gMoodCurves[0][8].range_mo_pre_rage = vec4(5, 0.0f, 0.0002f, 0.0002f);
		
		//normal rioter vs rioter retreat
		gMoodCurves[0][9].range_mo_pre_rage = vec4(3, -0.0001f, 0.0001f, 0.0001f);
		
		//normal rioter vs rioter civilian
		//No effect

		//agitated rioter vs passive police
		gMoodCurves[1][0].range_mo_pre_rage = vec4(5, 0.0f, 0.01f, 0.1f);

		//agitated rioter vs defensive police
		gMoodCurves[1][1].range_mo_pre_rage = vec4(5, 0.0f, 0.01f, 0.1f);

		//agitated rioter vs agggresive police
		gMoodCurves[1][2].range_mo_pre_rage = vec4(15, 0.0f, 0.5f, 0.1f);

		//agitated rioter vs normal rioter
		gMoodCurves[1][6].range_mo_pre_rage = vec4(15, 0.0f, 0.5f, 0.1f);

		//agitated rioter vs agitated rioter
		gMoodCurves[1][7].range_mo_pre_rage = vec4(15, 0.0f, 0.005f, 0.1f);

		//agitated rioter vs rioter attacking
		gMoodCurves[1][8].range_mo_pre_rage = vec4(5, 0.0f, 0.0001f, 0.0002f);

		//agitated rioter vs rioter retreat
		gMoodCurves[1][9].range_mo_pre_rage = vec4(3, -0.0001f, 0.0001f, 0.0002f);
			
		//agitated rioter vs rioter civilian
		//No effect
		
		//barrier();

		//attacking rioter vs passive police
		gMoodCurves[2][0].range_mo_pre_rage = vec4(5, 0.0f, 0.01f, 0.1f);

		//attacking rioter vs defensive police
		gMoodCurves[2][1].range_mo_pre_rage = vec4(5, 0.0, 0.0, 0.1f);

		//attacking rioter vs agggresive police
		gMoodCurves[2][2].range_mo_pre_rage = vec4(15, 0.0f, 0.5f, 0.1f);

		//attacking rioter vs normal rioter
		//No effect

		//attacking rioter vs agitated rioter
		//No effect

		//attacking rioter vs rioter attacking
		gMoodCurves[2][8].range_mo_pre_rage = vec4(5, 0.0001f, 0.0001f, 0.0002f);

		//attacking rioter vs rioter retreat
		gMoodCurves[2][9].range_mo_pre_rage = vec4(3, -0.0001f, 0.0001f, 0.0f);
			
		//agitated rioter vs rioter civilian
		//No effect

		//barrier();

		//retreating rioter
		//No effects


		//civilian rioter vs passive police
		//No effect

		//civilian rioter vs defensive police
		//No effect

		// civilian rioter vs agggresive police
		gMoodCurves[4][2].range_mo_pre_rage = vec4(15, 0.0f, 0.5f, 0.1f);

		//civilian rioter vs normal rioter
		//No effect

		//civilian rioter vs agitated rioter
		gMoodCurves[4][7].range_mo_pre_rage = vec4(5, 0.0f, 0.005f, 0.001f);

		//civilian rioter vs rioter attacking
		gMoodCurves[4][8].range_mo_pre_rage = vec4(5, 0.0f, 0.001f, 0.002f);

		//civilian rioter vs rioter retreat
		gMoodCurves[4][9].range_mo_pre_rage = vec4(3, -0.0001f, 0.0f, 0.0f);

		//civilian rioter vs rioter civilian
		//No effect


		//police vs rioter neut 
		//No effect

		//police vs rioter agi
		gMoodCurves[5][7].range_mo_pre_rage = vec4(1, -0.0001f, 0.0f, 0.0f);

		//police vs rioter att
		gMoodCurves[5][8].range_mo_pre_rage = vec4(15, -0.001f, 0.0f, 0.0f);

		//police vs rioter ret
		gMoodCurves[5][9].range_mo_pre_rage = vec4(15, -0.001f, 0.0f, 0.0f);

		//pol vs rioter civ
		//No effect
		*/
	}
	
	barrier();
	
	uint passCount = 0;
	passCount = ( gEntityCount + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
	
	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index >= gEntityCount)
			break;

		gOutput[index].newDirection_speed = gInput[index].newDirection_speed;
		
		if (int(gInput[index].position_unitType.w) == RIOTER_TYPE)
		{
			vec2 bestIndex = vec2(0.0f, 0.0f);

			vec3 offsetPos = vec3(
			gInput[index].position_unitType.x + 0 * gInput[index].newDirection_speed.x / 2.0f,
			gInput[index].position_unitType.y + 0 * gInput[index].newDirection_speed.y / 2.0f, 
			gInput[index].position_unitType.z + 0 * gInput[index].newDirection_speed.z / 2.0f);
		
			float highestSum = GetEffectOnAgentAt(vec2(offsetPos.x, offsetPos.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.x));
			float staySum = highestSum;

			CalculatedCharge chargeSums[8];

			float chargeSum = 0;
			int groupID =  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.x);

			#define navMeshWallRepellVal -1000.0

			vec3 navMeshWallVector;
			navMeshWallVector.x = int(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.w * 0.001);
			navMeshWallVector.y = 0.01;
			navMeshWallVector.z = abs( int((gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.w - navMeshWallVector.x * 1000) * 0.01) ) - 2;
			float navMeshWallDistance = abs( gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.w - navMeshWallVector.x * 1000 ) - abs( navMeshWallVector.z * 100 ) - 200;
			navMeshWallVector = normalize( navMeshWallVector ) * navMeshWallDistance;

			// ---------------------------------------- -1, 0 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 0), groupID) + dot( navMeshWallVector, normalize( vec3( -1, 0, 0))) * navMeshWallRepellVal;
			chargeSums[0].x = -1;																									    
			chargeSums[0].y = 0;																									    
			chargeSums[0].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 1, 0 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 0), groupID) + dot( navMeshWallVector, normalize( vec3( 1, 0, 0))) * navMeshWallRepellVal;
			chargeSums[1].x = 1;																									    
			chargeSums[1].y = 0;																									    
			chargeSums[1].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 0, -1 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z - 1), groupID) + dot( navMeshWallVector, normalize( vec3( 0, 0, 1))) * navMeshWallRepellVal;
			chargeSums[2].x = 0;																									    
			chargeSums[2].y = -1;																									    
			chargeSums[2].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 0, 1 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z + 1), groupID) + dot( navMeshWallVector, normalize( vec3( 0, 0, 1))) * navMeshWallRepellVal;
			chargeSums[3].x = 0;																									    
			chargeSums[3].y = 1;																									    
			chargeSums[3].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- -1, -1 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z - 1), groupID) + dot( navMeshWallVector, normalize( vec3( -1, 0, 1))) * navMeshWallRepellVal;
			chargeSums[4].x = -1;																									    
			chargeSums[4].y = -1;																									    
			chargeSums[4].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 1, -1 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z - 1), groupID) + dot( navMeshWallVector, normalize( vec3( 1, 0, 1))) * navMeshWallRepellVal;
			chargeSums[5].x = 1;																									    
			chargeSums[5].y = -1;																									    
			chargeSums[5].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- -1, 1 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 1), groupID) + dot( navMeshWallVector, normalize( vec3( -1, 0, 1))) * navMeshWallRepellVal;
			chargeSums[6].x = -1;																									    
			chargeSums[6].y = 1;																									    
			chargeSums[6].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 1, 1 ----------------------------------------								    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 1), groupID) + dot( navMeshWallVector, normalize( vec3( 1, 0, 1))) * navMeshWallRepellVal;
			chargeSums[7].x = 1;
			chargeSums[7].y = 1;
			chargeSums[7].chargeSum = chargeSum;

			bool set = false;
			for (int i = 0; i < 8; i++)
			{
				if (chargeSums[i].chargeSum > highestSum)
				{
					highestSum = chargeSums[i].chargeSum;
					bestIndex.x = chargeSums[i].x;
					bestIndex.y = chargeSums[i].y;
				}
			}

			vec3 pfVector = vec3(0.0f);
			
			if (highestSum - staySum > STAY_LIMIT)
			{
				pfVector = vec3( bestIndex.x, 0, bestIndex.y );
			
				if (length(pfVector) > 0.1f)
					pfVector = normalize(pfVector);
			}

			gOutput[index].newDirection_speed = vec4(pfVector.x, 0, pfVector.z, gInput[index].newDirection_speed.w );
			
			//float dirDot = dot(vec2(gInput[index].newDirection_speed.x, gInput[index].newDirection_speed.z), vec2(pfVector.x, pfVector.z));
			//
			//vec3 FFDirection = gInput[index].newDirection_speed.xyz;
			//FFDirection = FFDirection * float(!(dirDot < 0.0f));
			//
			//vec3 newDir = vec3(
			//FFDirection.x * FF_FACTOR + pfVector.x * PF_FACTOR, 
			//FFDirection.y * FF_FACTOR + pfVector.y * PF_FACTOR,
			//FFDirection.z * FF_FACTOR + pfVector.z * PF_FACTOR);
			//
			//if (length(newDir) > 0.1f)
			//	newDir = normalize(newDir);
			//
			//gOutput[index].newDirection_speed = vec4(newDir, gInput[index].newDirection_speed.w);
		}											
	}												
	
	barrier();
	
	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index > gEntityCount)
			break;

		float morale = gInput[index].health_stamina_morale_stance.z;
		float pressure = gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.z;
		float rage = gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.y;

		vec3 moods = GetMoodOnAgent(vec2(gInput[index].position_unitType.x, gInput[index].position_unitType.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.x), int(index));

		gOutput[index].morale_rage_pressure_empty = vec4(morale + moods.x, rage + moods.y, pressure + moods.z, 0);
	}
	
	//barrier();
}

//Rioter == 0
//Rioter stance indices
//Normal = 0
//Agitated = 1
//Attacking = 2
//Retreating = 3
//Civilian = 4


//MOOD CURVES
//x vs passive police
//gMoodCurves[x][0]

//x vs defensive police
//gMoodCurves[x][1]

//x vs aggresive police
//gMoodCurves[x][2]

//x vs attacking police
//gMoodCurves[x][3]

//x vs weapons police
//gMoodCurves[x][4]

//x vs police car
//gMoodCurves[x][5]

//x vs rioter neutral
//gMoodCurves[x][6]

//x vs rioter agitated
//gMoodCurves[x][7]

//x vs rioter attacking
//gMoodCurves[x][8]

//x vs rioter retreat
//gMoodCurves[x][9]

//x vs rioter civilian
//gMoodCurves[x][10]

//x vs weapon rioter
//gMoodCurves[x][11]

//x vs megaphone
//gMoodCurves[x][12]

//Rioter neutral vs x
//gMoodCurves[0][x]

//Rioter agitated vs x
//gMoodCurves[1][x]

//Rioter attack vs x
//gMoodCurves[2][x]

//Rioter retreating vs x
//gMoodCurves[3][x]

//Rioter civilian vs x
//gMoodCurves[4][x]

//Police all states vs x
//gMoodCurves[5][x]