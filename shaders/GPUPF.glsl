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

struct DataIN
{
	vec4 position_unitType;
	vec4 direction_speed;
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

//vec4 curves[1][2];
shared ChargeCurve curves[1][2];

float GetAgentChargeAt(int unitType, float distSqr)
{
	//ChargeCurve curves[1][2];
	//curves[0][0].ch_cu_re_dec = vec4(1.0f, 15.0f, 2.0f, 1.0f / (15.0f - 2.0f));
	//curves[0][1].ch_cu_re_dec = vec4(-100.0f, 15.0f, 1.0f, -100.0f / (15.0f - 1.0f));

	//if (distSqr > 0.001f && distSqr < curves[0][unitType].z)
	//	return -100 + distSqr * (curves[0][unitType].z / 100);
	//else if (distSqr <= curves[0][unitType].y)
	//	return curves[0][unitType].x - distSqr * curves[0][unitType].w;
	//ChargeCurve c = curves[0][unitType];
	//
	//if(distSqr <= c.ch_cu_re_dec.y)
	//{
	//	if (distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z)
	//		return (-MINIMUM_CHARGE + distSqr * (c.ch_cu_re_dec.z / MINIMUM_CHARGE));
	//
	//	return (c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w);
	//}


	//if (distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z)
	//	return -100 + distSqr * (c.ch_cu_re_dec.z / 100);
	//else if (distSqr <= c.ch_cu_re_dec.y)
	//	return c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w;

	//if (distSqr > 0.001f && distSqr < curves[0][unitType].ch_cu_re_dec.z)
	//	return -100 + distSqr * (curves[0][unitType].ch_cu_re_dec.z / 100);
	//else if (distSqr <= curves[0][unitType].ch_cu_re_dec.y)
	//	return curves[0][unitType].ch_cu_re_dec.x - distSqr * curves[0][unitType].ch_cu_re_dec.w;
	ChargeCurve c = curves[0][unitType];
	
	return float(distSqr < c.ch_cu_re_dec.z) * (-100 + distSqr * (c.ch_cu_re_dec.z / 100)) + 
	float(distSqr <= c.ch_cu_re_dec.y) * ( c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w);
	
	
	
	//return 0.0f;
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
	
		if (distSqr >= curves[0][0].ch_cu_re_dec.z && (gInput[i].position_unitType.w == RIOTER_TYPE && matchID != groupID) )
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
		curves[0][0].ch_cu_re_dec = vec4(0.0f, 5.0f, 1.0f, 0.0f / (5.0f));
		curves[0][1].ch_cu_re_dec = vec4(-5000.0f, 25.0f, 1.0f, -5000.0f / (25.0f));
	}
	
	barrier();
	
	uint passCount = 0;
	passCount = ( gEntityCount + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
	
	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index > gEntityCount)
			break;
	
		//gOutput[index].newDirection_speed = gInput[index].newDirection_speed;
		//gOutput[index].goal_maxSpeed = gInput[index].goal_maxSpeed;

		if (int(gInput[index].position_unitType.w) == RIOTER_TYPE)
		{
			vec2 bestIndex = vec2(0.0f, 0.0f);

			vec3 offsetPos = vec3(
			gInput[index].position_unitType.x + gInput[index].newDirection_speed.x / 2.0f,
			gInput[index].position_unitType.y + gInput[index].newDirection_speed.y / 2.0f, 
			gInput[index].position_unitType.z + gInput[index].newDirection_speed.z / 2.0f);
		
			float highestSum = GetEffectOnAgentAt(vec2(offsetPos.x, offsetPos.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			float staySum = highestSum;

			

			/*
			for(int i = -1; i < 2; ++i)
			{
				for(int j = -1; j < 2; ++j)
				{
					if(i == 0 && j == 0)
						continue;
			
					float chargeSum = GetEffectOnAgentAt(vec2(gInput[index].position_unitType.x + i, gInput[index].position_unitType.z + j),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
					
					if(chargeSum > highestSum)
					{
						highestSum = chargeSum;
						bestIndex.x = i;
						bestIndex.y = j;
					}
				}
			}
			*/
			
			CalculatedCharge chargeSums[8];
			
			// ---------------------------------------- -1, 0 ----------------------------------------
			float chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 0),  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			
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
			
			for (int i = 0; i < 8; i++)
			{
				if (chargeSums[i].chargeSum > highestSum)
				{
					highestSum = chargeSums[i].chargeSum;
					bestIndex.x = chargeSums[i].x;
					bestIndex.y = chargeSums[i].y;
				}
			}

			//barrier();

			vec3 pfVector = vec3(0.0f);
			
			if (highestSum - staySum > STAY_LIMIT)
			{
				pfVector = vec3( bestIndex.x, 0, bestIndex.y );

				if (length(pfVector) > 0.0f)
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
			
			//barrier();
			if (length(newDir) > 0.1f)
				newDir = normalize(newDir);

			gOutput[index].newDirection_speed = vec4(newDir, gInput[index].newDirection_speed.w);
		}											
	}												
	
	//barrier();
}