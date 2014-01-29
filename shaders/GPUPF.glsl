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
	vec4 health_stamina_morale_stancealignment;
	vec4 groupSquadID_defenseRage_mobilityPressure_empty;
};

struct DataOUT
{
	vec4 newDirection_speed;
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

//r-prefix = receiever
//s-prefix = sender
void MoodProp(inout float rMorale, inout float rPressure, inout float rRage, in int rType, in int rState, 
in float sMorale, in float sPressure, in float sRage, in int sType, in int sState, in float dist)
{
	int senderIndex = gMoodSenderIndices[sType][sState];
	int receieverIndex = gMoodReceiverIndices[rType][rState];

	MoodCurve moodCurve = gMoodCurves[receieverIndex][senderIndex];

	float attenuation = 1.0f / dist - 1.0f / moodCurve.range;

	//float moraleFactor;
	//float pressureFactor;
	//float rageFactor;

	rMorale += moodCurve.moraleFactor * attenuation * sMorale;
	rPressure += moodCurve.pressureFactor * attenuation * sPressure;
	rRage += moodCurve.rageFactor * attenuation * sRage;
}

float GetAgentChargeAt(int unitType, float distSqr)
{
	ChargeCurve c = gChargeCurves[0][unitType];
	
	return float(distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z) * (-MINIMUM_CHARGE + distSqr * (c.ch_cu_re_dec.z / MINIMUM_CHARGE)) + 
	float(distSqr <= c.ch_cu_re_dec.y) * ( c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w);
}


float GetEffectOnAgentAt(vec2 queryPosition, int groupID, int index)
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

		float murre = 0;
		float ragge = 0;
		float presse = 0;

		if (index > 0)
			MoodProp(murre, ragge, presse, 0, 0, 0, 0, 0, 0, 0, 0);
	
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

		//Police all states vs x
		
		//gMoodCurves[5][x]


		//curve index array
		//muhIndex[unitType][unitStance] = colIndex


		//Mood curve factors
		//float range;
		//float moraleFactor;
		//float pressureFactor;
		//float rageFactor;

	}
	
	barrier();
	
	uint passCount = 0;
	passCount = ( gEntityCount + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
	
	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index > gEntityCount)
			break;

		gOutput[index].newDirection_speed = gInput[index].newDirection_speed;

		if (int(gInput[index].position_unitType.w) == RIOTER_TYPE)
		{
			vec2 bestIndex = vec2(0.0f, 0.0f);

			vec3 offsetPos = vec3(
			gInput[index].position_unitType.x + 0 * gInput[index].newDirection_speed.x / 2.0f,
			gInput[index].position_unitType.y + 0 * gInput[index].newDirection_speed.y / 2.0f, 
			gInput[index].position_unitType.z + 0 * gInput[index].newDirection_speed.z / 2.0f);
		
			float highestSum = GetEffectOnAgentAt(vec2(offsetPos.x, offsetPos.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), int(index));
			float staySum = highestSum;

			CalculatedCharge chargeSums[8];

			float chargeSum = 0;

			// ---------------------------------------- -1, 0 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 0),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[0].x = -1;
			chargeSums[0].y = 0;
			chargeSums[0].chargeSum = chargeSum;

			// ---------------------------------------- 1, 0 ----------------------------------------
			 chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 0),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[1].x = 1;
			chargeSums[1].y = 0;
			chargeSums[1].chargeSum = chargeSum;
			
			// ---------------------------------------- 0, -1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z - 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[2].x = 0;
			chargeSums[2].y = -1;
			chargeSums[2].chargeSum = chargeSum;
			
			// ---------------------------------------- 0, 1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z + 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[3].x = 0;
			chargeSums[3].y = 1;
			chargeSums[3].chargeSum = chargeSum;
			
			// ---------------------------------------- -1, -1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z - 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[4].x = -1;
			chargeSums[4].y = -1;
			chargeSums[4].chargeSum = chargeSum;
			
			// ---------------------------------------- 1, -1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z - 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[5].x = 1;
			chargeSums[5].y = -1;
			chargeSums[5].chargeSum = chargeSum;
			
			// ---------------------------------------- -1, 1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
			chargeSums[6].x = -1;
			chargeSums[6].y = 1;
			chargeSums[6].chargeSum = chargeSum;
			
			// ---------------------------------------- 1, 1 ----------------------------------------
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 1),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x), -1);
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