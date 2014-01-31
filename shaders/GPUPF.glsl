#version 430 core

#define WORK_GROUP_SIZE 1024
#define MAXIMUM_ENTITIES 1024
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
	float range;
	float moraleFactor;
	float pressureFactor;
	float rageFactor;
};

struct DataIN
{
	vec4 position_unitType;
	vec4 newDirection_speed;
	vec4 health_stamina_morale_stance;
	vec4 groupSquadID_defenseRage_mobilityPressure_empty;
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

uniform uint gEntityCount;

shared ChargeCurve gChargeCurves[1][2];
shared MoodCurve gMoodCurves[6][13];
shared int gMoodSenderIndices[2][6];
shared int gMoodReceiverIndices[2][6];

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

// Xorshift algorithm from George Marsaglia's paper
float rand( uint seed )
{
	uint rng_state = seed;
    rng_state ^= (rng_state << 13);
    rng_state ^= (rng_state >> 17);
    rng_state ^= (rng_state << 5);
    return float( rng_state ) * (1.0 / 4294967296.0);
}


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
	
	for (int i = 0; i < gInput.length(); ++i)
	{
		if (i > gEntityCount)
			break;
		
		float dx = gInput[i].position_unitType.x - queryPosition.x;
		float dz = gInput[i].position_unitType.z - queryPosition.y;
		float distSqr = dx * dx + dz * dz;
	
		int matchID = -1;
	
		if (groupID >= 0)
		{
			matchID = int(gInput[i].groupSquadID_defenseRage_mobilityPressure_empty.x);
		}
	
		if (distSqr >= gChargeCurves[0][0].ch_cu_re_dec.z && (gInput[i].position_unitType.w == RIOTER_TYPE && matchID != groupID) )
			continue;
	
		currentSum = GetAgentChargeAt(int(gInput[i].position_unitType.w), distSqr);
	
		if (currentSum >= 0)
			positiveSum += currentSum;
		else
			negativeSum += currentSum;
	
	}

	return positiveSum + negativeSum;
}

//r-prefix = receiever
//s-prefix = sender
vec3 MoodProp(int rType, int rState, float sMorale, float sPressure, float sRage, int sType, int sState, float dist)
{
	int senderIndex = gMoodSenderIndices[sType][sState];
	int receieverIndex = gMoodReceiverIndices[rType][rState];

	MoodCurve moodCurve = gMoodCurves[receieverIndex][senderIndex];

	float rMorale = 0;
	float rPressure = 0;
	float rRage = 0;

	if (dist > moodCurve.range)
		return vec3(0, 0, 0);
	
	float attenuation = 1.0f / (dist * dist) - 1.0f / (moodCurve.range * moodCurve.range); 

	rMorale = moodCurve.moraleFactor * attenuation * sMorale;
	rPressure = moodCurve.pressureFactor * attenuation * sPressure;
	rRage = moodCurve.rageFactor * attenuation * sRage;

	return vec3(rMorale, rRage, rPressure);
}


vec3 GetMoodOnAgent(vec2 queryPosition, int groupID, int index)
{
	float morale = 0;
	float pressure = 0;
	float rage = 0;

	for (int i = 0; i < gInput.length(); ++i)
	{
		if (i >= gEntityCount)
			break;

		if (i == index)
			continue;
		
		float dist = distance(gInput[i].position_unitType.xyz, vec3(queryPosition.x, 0, queryPosition.y));

		//float rMorale = gInput[index].health_stamina_morale_stance.z;
		//float rRage = gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.y;
		//float rPressure = gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.z;
		int rType = int(gInput[index].position_unitType.w);
		int rState = int(gInput[index].health_stamina_morale_stance.w);

		float sMorale = gInput[i].health_stamina_morale_stance.z;
		float sRage = gInput[i].groupSquadID_defenseRage_mobilityPressure_empty.y;
		float sPressure = gInput[i].groupSquadID_defenseRage_mobilityPressure_empty.z;
		int sType = int(gInput[i].position_unitType.w);
		int sState = int(gInput[i].health_stamina_morale_stance.w);

		vec3 moods = MoodProp(rType, rState, sMorale, sPressure, sRage, sType, sState, dist);
		
		morale += moods.x;
		rage += moods.y;
		pressure += moods.z;
	}

	return vec3(morale, rage, pressure);
}

void main()
{
	if (gl_LocalInvocationIndex == 0)
	{
		gChargeCurves[0][0].ch_cu_re_dec = vec4(1.0f, 5.0f, 1.0f, 1.0f / (5.0f)); //0.3 makes them stick into a huge blob
		gChargeCurves[0][1].ch_cu_re_dec = vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f));

		//Rioter == 0
		//Rioter stance indices
		//Normal = 0
		//Agitated = 1
		//Attacking = 2
		//Retreating = 3
		//Civilian = 4

		//MOOD CURVE INDICES/PLACEMENT CAN BE FOUND AT THE BOTTOM OF THIS DOCUMENT

		//normal
		gMoodSenderIndices[RIOTER_TYPE][0] = 6;
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

		//Police == 1
		//Police stance indices
		//Passive = 0
		//Defensive = 1
		//Aggressive = 2
		//Attacking = 3

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

		//Mood curve factors
		//float range;
		//float moraleFactor;
		//float pressureFactor;
		//float rageFactor;

		//receiver first, sender second
		//normal rioter vs passive police
		

		//Init all curves to no effect
		for (int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 13; j++)
			{
				gMoodCurves[i][j].range = 0;
				gMoodCurves[i][j].moraleFactor = 0;
				gMoodCurves[i][j].pressureFactor = 0;
				gMoodCurves[i][j].rageFactor = 0;
			}
		}

		/************************************************************************************/
		//passive police vs normal rioter
		gMoodCurves[0][0].range = 5.0f;
		gMoodCurves[0][0].moraleFactor = -0.001f;
		gMoodCurves[0][0].pressureFactor = 0.01f;
		gMoodCurves[0][0].rageFactor = 0.01f;

		//defensive police vs normal rioter
		gMoodCurves[0][1].range = 5.0f;
		gMoodCurves[0][1].moraleFactor = 0.0f;
		gMoodCurves[0][1].pressureFactor = 0.0f;
		gMoodCurves[0][1].rageFactor = 0.01f;

		//agggresive police vs normal rioter
		gMoodCurves[0][2].range = 15.0f;
		gMoodCurves[0][2].moraleFactor = 0.0f;
		gMoodCurves[0][2].pressureFactor = 0.5f;
		gMoodCurves[0][2].rageFactor = 0.1f;

		//Attacking police vs normal rioter
		gMoodCurves[0][3].range = 15.0f; //Attacking state doesn't actually exist yet, fix this
		gMoodCurves[0][3].moraleFactor = -0.5f;
		gMoodCurves[0][3].pressureFactor = 5000.0f;
		gMoodCurves[0][3].rageFactor = 5000.0f;

		/************************************************************************************/
		//passive police vs agitated rioter
		gMoodCurves[1][0].range = 5.0f;
		gMoodCurves[1][0].moraleFactor = 0.0f;
		gMoodCurves[1][0].pressureFactor = 0.01f;
		gMoodCurves[1][0].rageFactor = 0.1f;

		//defensive police vs agitated rioter
		gMoodCurves[1][1].range = 5.0f;
		gMoodCurves[1][1].moraleFactor = 0.0f;
		gMoodCurves[1][1].pressureFactor = 0.0f;
		gMoodCurves[1][1].rageFactor = 0.1f;

		//agggresive police vs agitated rioter
		gMoodCurves[1][2].range = 15.0f;
		gMoodCurves[1][2].moraleFactor = 0.0f;
		gMoodCurves[1][2].pressureFactor = 0.5f;
		gMoodCurves[1][2].rageFactor = 0.1f;

		/************************************************************************************/

		//passive police vs attacking rioter
		gMoodCurves[2][0].range = 5.0f;
		gMoodCurves[2][0].moraleFactor = 0.0f;
		gMoodCurves[2][0].pressureFactor = 0.01f;
		gMoodCurves[2][0].rageFactor = 0.1f;

		//defensive police vs attacking rioter
		gMoodCurves[2][1].range = 5.0f;
		gMoodCurves[2][1].moraleFactor = 0.0f;
		gMoodCurves[2][1].pressureFactor = 0.0f;
		gMoodCurves[2][1].rageFactor = 0.1f;

		//agggresive police vs attacking rioter
		gMoodCurves[2][2].range = 15.0f;
		gMoodCurves[2][2].moraleFactor = 0.0f;
		gMoodCurves[2][2].pressureFactor = 0.5f;
		gMoodCurves[2][2].rageFactor = 0.1f;

		/************************************************************************************/

		//passive police vs retreating rioter
		gMoodCurves[3][0].range = 5.0f;
		gMoodCurves[3][0].moraleFactor = 0.0f;
		gMoodCurves[3][0].pressureFactor = 0.01f;
		gMoodCurves[3][0].rageFactor = 0.1f;

		//defensive police vs retreating rioter
		gMoodCurves[3][1].range = 5.0f;
		gMoodCurves[3][1].moraleFactor = 0.0f;
		gMoodCurves[3][1].pressureFactor = 0.0f;
		gMoodCurves[3][1].rageFactor = 0.1f;

		//agggresive police vs retreating rioter
		gMoodCurves[3][2].range = 15.0f;
		gMoodCurves[3][2].moraleFactor = 0.0f;
		gMoodCurves[3][2].pressureFactor = 0.5f;
		gMoodCurves[3][2].rageFactor = 0.1f;
		
		/************************************************************************************/

		//passive police vs civilian rioter
		gMoodCurves[4][0].range = 0.0f;
		gMoodCurves[4][0].moraleFactor = 0.0f;
		gMoodCurves[4][0].pressureFactor = 0.0f;
		gMoodCurves[4][0].rageFactor = 0.0f;

		//defensive police vs civilian rioter
		gMoodCurves[4][1].range = 0.0f;
		gMoodCurves[4][1].moraleFactor = 0.0f;
		gMoodCurves[4][1].pressureFactor = 0.0f;
		gMoodCurves[4][1].rageFactor = 0.0f;

		//agggresive police vs civilian rioter
		gMoodCurves[4][2].range = 15.0f;
		gMoodCurves[4][2].moraleFactor = 0.0f;
		gMoodCurves[4][2].pressureFactor = 0.5f;
		gMoodCurves[4][2].rageFactor = 0.1f;

		/************************************************************************************/

		//rioter neut vs police
		gMoodCurves[5][6].range = 1;
		gMoodCurves[5][6].moraleFactor = 0;
		gMoodCurves[5][6].pressureFactor = 0;
		gMoodCurves[5][6].rageFactor = 0;

		//rioter agi vs police
		gMoodCurves[5][7].range = 1;
		gMoodCurves[5][7].moraleFactor = -0.0001;
		gMoodCurves[5][7].pressureFactor = 0;
		gMoodCurves[5][7].rageFactor = 0;

		//rioter att vs police
		gMoodCurves[5][8].range = 15;
		gMoodCurves[5][8].moraleFactor = -0.001f;
		gMoodCurves[5][8].pressureFactor = 0;
		gMoodCurves[5][8].rageFactor = 0;

		//rioter ret vs police
		gMoodCurves[5][9].range = 15;
		gMoodCurves[5][9].moraleFactor = 0.1f;
		gMoodCurves[5][9].pressureFactor = 0;
		gMoodCurves[5][9].rageFactor = 0;

		//rioter civ vs pol
		gMoodCurves[5][10].range = 1;
		gMoodCurves[5][10].moraleFactor = 0;
		gMoodCurves[5][10].pressureFactor = 0;
		gMoodCurves[5][10].rageFactor = 0;

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
		
			float highestSum = GetEffectOnAgentAt(vec2(offsetPos.x, offsetPos.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			float staySum = highestSum;

			CalculatedCharge chargeSums[8];

			float chargeSum = 0;

			// ---------------------------------------- -1, 0 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 0),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[0].x = -1;
			chargeSums[0].y = 0;
			chargeSums[0].chargeSum = chargeSum;

			// ---------------------------------------- 1, 0 ----------------------------------------
			 chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 0),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[1].x = 1;
			chargeSums[1].y = 0;
			chargeSums[1].chargeSum = chargeSum;
			
			// ---------------------------------------- 0, -1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z - 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[2].x = 0;
			chargeSums[2].y = -1;
			chargeSums[2].chargeSum = chargeSum;
			
			// ---------------------------------------- 0, 1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z + 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[3].x = 0;
			chargeSums[3].y = 1;
			chargeSums[3].chargeSum = chargeSum;
			
			// ---------------------------------------- -1, -1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z - 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[4].x = -1;
			chargeSums[4].y = -1;
			chargeSums[4].chargeSum = chargeSum;
			
			// ---------------------------------------- 1, -1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z - 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[5].x = 1;
			chargeSums[5].y = -1;
			chargeSums[5].chargeSum = chargeSum;
			
			// ---------------------------------------- -1, 1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			chargeSums[6].x = -1;
			chargeSums[6].y = 1;
			chargeSums[6].chargeSum = chargeSum;
			
			// ---------------------------------------- 1, 1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
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
				//else if (chargeSums[i].chargeSum == highestSum)
				//{
				//	highestSum = chargeSums[i].chargeSum;
				//	bestIndex.x = mix(bestIndex.x, chargeSums[i].x, 0.1f);
				//	bestIndex.y = mix(bestIndex.y, chargeSums[i].y, 0.1f);
				//}
			}

			vec3 pfVector = vec3(0.0f);
			
			if (highestSum - staySum > STAY_LIMIT)
			{
				pfVector = vec3( bestIndex.x, 0, bestIndex.y );

				if (length(pfVector) > 0.1f)
					pfVector = normalize(pfVector);
			}
			
			float dirDot = dot(vec2(gInput[index].newDirection_speed.x, gInput[index].newDirection_speed.z), vec2(pfVector.x, pfVector.z));
			
			vec3 FFDirection = gInput[index].newDirection_speed.xyz;
			
			if (dirDot < 0.0f)
			{
				FFDirection = vec3(0.0f);
			}
			
			vec3 newDir = vec3(
			FFDirection.x * FF_FACTOR + pfVector.x * PF_FACTOR, 
			FFDirection.y * FF_FACTOR + pfVector.y * PF_FACTOR,
			FFDirection.z * FF_FACTOR + pfVector.z * PF_FACTOR);
			
			if (length(newDir) > 0.1f)
				newDir = normalize(newDir);

			gOutput[index].newDirection_speed = vec4(newDir, gInput[index].newDirection_speed.w);
		}											
	}												
	
	barrier();

	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index > gEntityCount)
			break;

		float morale = gInput[index].health_stamina_morale_stance.z;
		float pressure = gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.z;
		float rage = gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.y;

		vec3 moods = GetMoodOnAgent(vec2(gInput[index].position_unitType.x, gInput[index].position_unitType.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), int(index));

		gOutput[index].morale_rage_pressure_empty = vec4(morale + moods.x, rage + moods.y, pressure + moods.z, 0);
		//gOutput[index].morale_rage_pressure_empty = vec4(1, 1, 1, 0);
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

//x vs rioter agitated
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