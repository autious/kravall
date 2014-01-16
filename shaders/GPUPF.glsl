#version 430 core

#define WORK_GROUP_SIZE 1280
#define MAXIMUM_ENTITIES 1024
#define MAX_CURVES 1024;

#define RIOTER_TYPE 0
#define POLICE_TYPE 1

#pragma optionNV(fastmath on) 
#pragma optionNV(ifcvt none) 
#pragma optionNV(inline all) 
#pragma optionNV(strict on) 
#pragma optionNV(unroll all)


layout (local_size_x = WORK_GROUP_SIZE) in;

struct ChargeCurve
{
	vec4 ch_cu_re_dec;
};

struct DataIN
{
	vec4 position_unitType;
	vec4 direction_speed;
	vec4 goal_maxSpeed;
	vec4 health_stamina_morale_stancealignment;
	vec4 groupSquadID_defenseRage_mobilityPressure_empty;
};

struct DataOUT
{
	vec4 direction_speed;
	vec4 goal_maxSpeed;
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
	ChargeCurve c = curves[0][unitType];

	if(distSqr <= c.ch_cu_re_dec.y)
	{
		if (distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z)
			return (-100 + distSqr * (c.ch_cu_re_dec.z / 100));

		return (c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w);
	}


	//if (distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z)
	//	return -100 + distSqr * (c.ch_cu_re_dec.z / 100);
	//else if (distSqr <= c.ch_cu_re_dec.y)
	//	return c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w;

	//if (distSqr > 0.001f && distSqr < curves[0][unitType].ch_cu_re_dec.z)
	//	return -100 + distSqr * (curves[0][unitType].ch_cu_re_dec.z / 100);
	//else if (distSqr <= curves[0][unitType].ch_cu_re_dec.y)
	//	return curves[0][unitType].ch_cu_re_dec.x - distSqr * curves[0][unitType].ch_cu_re_dec.w;

	//if (distSqr < curves[0][unitType].ch_cu_re_dec.z)
	//	return -100 + distSqr * (curves[0][unitType].ch_cu_re_dec.z / 100);
	//else if (distSqr <= curves[0][unitType].ch_cu_re_dec.y)
	//	return curves[0][unitType].ch_cu_re_dec.x - distSqr * curves[0][unitType].ch_cu_re_dec.w;
	//
	return 0.0f;
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
	
		//if (groupID >= 0)
		{
			matchID = int(gInput[i].groupSquadID_defenseRage_mobilityPressure_empty.x);
		}
	
		//if (distSqr >= curves[0][0].ch_cu_re_dec.z && matchID != groupID)
		//	continue;
	
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
		curves[0][0].ch_cu_re_dec = vec4(1.0f, 15.0f, 2.0f, 1.0f / (15.0f - 2.0f));
		curves[0][1].ch_cu_re_dec = vec4(-100.0f, 15.0f, 1.0f, -100.0f / (15.0f - 1.0f));
	}
	
	barrier();
	
	uint passCount = 0;
	passCount = ( gEntityCount + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
	
	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index > gEntityCount)
			break;
	
		gOutput[index].direction_speed = gInput[index].direction_speed;
		gOutput[index].goal_maxSpeed = gInput[index].goal_maxSpeed;

		if (int(gInput[index].position_unitType.w) == RIOTER_TYPE)
		{
			vec2 bestIndex = vec2(0.0f, 0.0f);
		
			float highestSum = GetEffectOnAgentAt(vec2(gInput[index].position_unitType.x, gInput[index].position_unitType.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_empty.x));
			float staySum = highestSum;
		
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
		
			float FF_VS_PF_FACTOR = 1.0f;
			vec3 pfVector;

			if (highestSum - staySum > 0.1f)
			{
				//if (bestIndex.x == 0.0f || bestIndex.y == 0.0f)
				//	pfVector = vec3( bestIndex.x, 0.0f, bestIndex.y);
				//else
				{
					pfVector = normalize( vec3( bestIndex.x, 0, bestIndex.y ) );
				}

				gOutput[index].direction_speed.xyz = normalize( gInput[index].direction_speed.xyz + pfVector * FF_VS_PF_FACTOR );	
			}
			//else
			//{
			//	pfVector = vec3(0.0f);
			//	//MovementComponent::InterpolateToDirection(mc->direction, 0.0f, 0.0f, 0.0f);
			//}
		}
	}
	
	barrier();
}