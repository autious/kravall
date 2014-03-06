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
#pragma optionNV(unroll none)


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
	vec4 newDirection_team;
	vec4 health_stamina_morale_stance;
	//vec4 groupSquadID_defenseRage_mobilityPressure_empty;
	vec4 groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance;
};

struct DataOUT
{
	vec4 newDirection_team;
	vec4 morale_rage_pressure_empty;
};

layout(std430, binding = 0) coherent readonly buffer InputBuffer
{
	DataIN gInput[];
};

layout (std430, binding = 1) coherent writeonly buffer OutputBuffer
{
	DataOUT gOutput[];
};

//layout (std430, binding = 2) restrict readonly buffer MoodCurveBuffer
//{
//	MoodCurve gMoodCurves[];
//}

uniform uint gEntityCount;

//shared ChargeCurve gChargeCurves[1][2];

//x = charge
//y = cutoff
//z = repel
//w = decline

uniform vec4 gChargeCurves[6][13]= {{ vec4(-5000.0f, 40.0f, 40.0f, -5000.0f / (40.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 300.0f, 1.0f, -5000.0f / (300.0f)),vec4(0.0f),vec4(0.0f)},
									
								   { vec4(0.0f),vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)),  vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 20.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 300.0f, 1.0f, -5000.0f / (300.0f)),vec4(0.0f),vec4(0.0f) },

								   { vec4(0.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f / (1.0f)), vec4(0.0f, 0.0f, 1.5f, 0.0f / (1.0f)), vec4(10.0f, 0.0f, 0.0f, 10.0f / (100.0f)), vec4(0.0f, 0.0f, 0.0f, 0.0f / (1.0f)), vec4(0.0f, 0.0f, 0.0f, 0.0f / (1.0f)), vec4(-5000.0f, 0.0f, 0.0f, -5000.0f / (30.0f)), vec4(-5000.0f, 0.0f, 0.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 0.0f, 0.0f, -5000.0f / (30.0f)), vec4(-5000.0f, 0.0f, 0.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 0.0f, 0.0f, -5000.0f / (300.0f)), vec4(0.0f),vec4(0.0f)  }, 								   
								  
								   { vec4(0.0f),vec4(3.0f, 5.0f, 0.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 0.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 0.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 0.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 0.5f, 3.0f / (5.0f)),  vec4(-5000.0f, 0.0f, 0.5f, -5000.0f / (30.0f)),  vec4(-5000.0f, 0.0f, 0.5f, -5000.0f / (30.0f)),  vec4(-5000.0f, 0.0f, 0.5f, -5000.0f / (30.0f)),  vec4(-5000.0f, 0.0f, 0.5f, -5000.0f / (30.0f)),  vec4(-5000.0f, 0.0f, 0.5f, -5000.0f / (300.0f)),vec4(0.0f),vec4(0.0f)  },

								   { vec4(0.0f),vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)), vec4(3.0f, 5.0f, 1.5f, 3.0f / (5.0f)),  vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 30.0f, 1.0f, -5000.0f / (30.0f)),  vec4(-5000.0f, 300.0f, 1.0f, -5000.0f / (300.0f)),vec4(0.0f),vec4(0.0f)  },

								   { vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f), vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) }};


uniform int gChargeSenderIndices[3][6] = {{ 1, 2, 3, 4, 5, 0 },
										  { 6, 7, 8, 9, 0, 0  },
										  { 10, 11, 0, 0, 0, 0  }};

uniform int gChargeReceiverIndices[3][6] = {{ 0, 1, 2, 3, 4, 5 },
										  { 5, 5, 5, 5, 5, 5  },
										  { 0, 0, 0, 0, 0, 0  }};

//Row indices in order
//Normal
//Agitated
//Attacking
//Retreating
//Civilian

//Column indices in order
//reserved
//Normal rioter
//Agitated rioter
//Attacking rioter
//retreating rioter
//civilian rioter
//passive police
//defensive police
//Aggressive police
//Attacking police
//Tear gas
//Molotov



//x = range
//y = morale
//z = pressure
//w = rage
uniform vec4 gMoodCurves[6][13] = {{ vec4(20.0f, 0.01f, 0.5f, 0.5f), vec4(0.0f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, -0.001f, 0.001f, 0.001f),vec4(0.0f), vec4(20.0f, 0.0f, 0.7f, 0.0f), vec4(10.0f, 0.0f, 0.3f, 0.3f), vec4(20.0f, 0.1f, 1.0f, 1.0f), vec4(0.0f), vec4(20.0f, -0.04f, 10.01f, 10.01f), vec4(0.0f),vec4(0.0f) },

																																																																							   
								   { vec4(20.0f, 0.01f, 0.01f, 0.01f), vec4(5.0f, -0.001f, -0.001f, -0.001f), vec4(5.0f, 0.0f, 0.0001f, 0.0001f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, -0.001f, 0.001f, 0.001f),vec4(0.0f), vec4(20.0f, 0.0f, 1.1f, 1.1f), vec4(10.0f, 0.0f, 1.1f, 1.1f),  vec4(20.0f, 0.0f, 2.5f, 2.5f), vec4(0.0f), vec4(20.0f, -0.04f, 10.01f, 10.01f), vec4(0.0f),vec4(0.0f) },
																																																																							   
								   { vec4(20.0f, 0.01f, 0.01f, 0.01f), vec4(5.0f, 0.001f, -0.01f, -0.01f), vec4(5.0f, 0.001f, -0.005f, -0.005f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, -0.001f, 0.001f, 0.001f),vec4(0.0f), vec4(10.0f, 0.0f, 1.1f, 1.1f), vec4(10.0f, 0.0f, 1.1f, 1.1f),  vec4(10.0f, -0.01f, 2.5f, 2.5f), vec4(0.0f), vec4(20.0f, -0.04f, 10.01f, 10.01f), vec4(0.0f),vec4(0.0f) },
																																																																							   
								   { vec4(20.0f, 0.01f, 0.01f, 0.01f), vec4(0.0f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, -0.001f, 0.001f, 0.001f),vec4(0.0f),vec4(10.0f, 0.0f, 1.1f, 1.1f), vec4(10.0f, 0.0f, 1.1f, 1.1f),  vec4(10.0f, 0.1f, 1.0f, 1.0f), vec4(0.0f), vec4(20.0f, 10.01f, 10.01f, 10.01f), vec4(0.0f),vec4(0.0f)  },
																																																																							   
								   { vec4(20.0f, 0.01f, 0.01f, 0.01f), vec4(0.0f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, 0.001f, 0.001f, 0.001f), vec4(5.0f, -0.001f, 0.001f, 0.001f),vec4(0.0f), vec4(10.0f, 0.0f, 0.1f, 0.1f), vec4(10.0f, 0.0f, 0.1f, 0.1f),  vec4(10.0f, 0.1f, 1.0f, 1.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f),vec4(0.0f)  },

								   { vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f),vec4(0.0f) }};


uniform int gMoodSenderIndices[3][6] = {{ 1, 2, 3, 4, 5, 0 },
										{ 6, 7, 8, 9, 0, 0  },
										{ 10, 11, 0, 0, 0, 0  }};

uniform int gMoodReceiverIndices[3][6] = {{ 0, 1, 2, 3, 4, 5 },
										  {	5, 5, 5, 5, 0, 0 },
										  {	0, 0, 0, 0, 0, 0 }};


//Row indices in order
//Normal
//Agitated
//Attacking
//Retreating
//Civilian

//Column indices in order
//Different rioter teams 
//Normal rioter
//Agitated rioter
//Attacking rioter
//retreating rioter
//civilian rioter
//passive police
//defensive police
//Aggressive police
//Attacking police
//Tear gas
//Molotov



//uniform vec4 gMoodCurves[6][13] = {{ vec4(5, -0.001f, 0.01f, 0.01f), vec4(5, 0.0f, 0.0f, 0.01f),vec4(15, 0.0f, 0.5f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(3, 0.0f, 0.0001f, 0.00001f),vec4(5, 0.0f, 0.0002f, 0.0002f),vec4(3, -0.0001f, 0.0001f, 0.0001f),vec4(0.0f), vec4(30, -0.1f, 0.1f, 0.1f),vec4(0.0f) },
//									
//								   { vec4(5, 0.0f, 0.001f, 0.1f),vec4(5, 0.0f, 0.001f, 0.1f),vec4(15, 0.0f, 0.0005f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(3, 0.0f, 0.00005f, 0.001f),vec4(3, 0.0f, 0.00005f, 0.001f), vec4(5, 0.0f, 0.0001f, 0.0002f),vec4(3, -0.0001f, 0.0001f, 0.0002f),vec4(0.0f), vec4(30, -0.1f, 0.1f, 0.1f),vec4(0.0f) },
//
//								   { vec4(5, 0.0f, 0.01f, 0.1f), vec4(5, 0.0, 0.0, 0.1f), vec4(15, 0.0f, 0.5f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(5, 0.0001f, 0.0001f, 0.0002f), vec4(3, -0.0001f, 0.0001f, 0.0f),vec4(0.0f), vec4(30, -0.1f, 0.1f, 0.1f),vec4(0.0f) },
//
//								   { vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },
//
//								   { vec4(0.0f),vec4(0.0f),vec4(15, 0.0f, 0.5f, 0.1f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(5, 0.0f, 0.005f, 0.001f),vec4(5, 0.0f, 0.001f, 0.002f), vec4(3, -0.0001f, 0.0f, 0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) },
//
//								   { vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f),vec4(1, -0.0001f, 0.0f, 0.0f),vec4(15, -0.001f, 0.0f, 0.0f),vec4(15, -0.001f, 0.0f, 0.0f),vec4(0.0f),vec4(0.0f),vec4(0.0f) }};
//
////shared MoodCurve gMoodCurves[6][13];
////int gMoodSenderIndices[2][6];
////int gMoodReceiverIndices[2][6];
//
//uniform int gMoodSenderIndices[3][6] = {{ 6, 7, 8, 9, 10, 0 },
//										{ 0, 1, 2, 3, 0, 0	},
//										{ 11, 0, 0, 0, 0, 0	}};
//
//uniform int gMoodReceiverIndices[3][6] = {{ 0, 1, 2, 3, 4, 5 },
//										  {	5, 5, 5, 5, 0, 0 },
//										  {	0, 0, 0, 0, 0, 0 }};


float GetAgentChargeAt(int unitType, float distSqr, vec4 chargeCurve)
{
	//ChargeCurve c = gChargeCurves[0][unitType];
	//
	//return float(distSqr > 0.001f && distSqr < c.ch_cu_re_dec.z) * (-MINIMUM_CHARGE + distSqr * (c.ch_cu_re_dec.z / MINIMUM_CHARGE)) + 
	//float(distSqr <= c.ch_cu_re_dec.y) * ( c.ch_cu_re_dec.x - distSqr * c.ch_cu_re_dec.w);


	//int senderIndex = 1;//gChargeSenderIndices[sType][sState];
	//int receieverIndex = 0;//gChargeReceiverIndices[rType][rState];
	//
	//vec4 chargeCurve = gChargeCurves[receieverIndex][senderIndex];
	
	return float(distSqr > 0.001f && distSqr < chargeCurve.z) * (-MINIMUM_CHARGE + distSqr * (chargeCurve.z / MINIMUM_CHARGE)) + 
	float(distSqr <= chargeCurve.y) * ( chargeCurve.x - distSqr * chargeCurve.w);
}


float GetEffectOnAgentAt(vec2 queryPosition, int groupID, uint index)
{
	float positiveSum = 0.0f;
	float negativeSum = 0.0f;
	float currentSum = 0.0f;
	DataIN indata;
	DataIN receiver = gInput[index];

	int receieverIndex = gChargeReceiverIndices[int(receiver.position_unitType.w)][int(receiver.health_stamina_morale_stance.w)];

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

		int senderIndex = gChargeSenderIndices[int(indata.position_unitType.w)][int(indata.health_stamina_morale_stance.w)];

		int multiplier = int(indata.newDirection_team.w == receiver.newDirection_team.w || indata.newDirection_team.w  == 0 || receiver.health_stamina_morale_stance.w == 2);
		receieverIndex = receieverIndex * multiplier;
		senderIndex = senderIndex * multiplier;
		vec4 chargeCurve = gChargeCurves[receieverIndex][senderIndex];
	
		if (distSqr >= chargeCurve.y && (indata.position_unitType.w == RIOTER_TYPE))// && matchID != groupID) )
			continue;
	
		currentSum = GetAgentChargeAt(int(indata.position_unitType.w), distSqr, chargeCurve);
	
		//if(currentSum >= 0)
			positiveSum += currentSum * float((currentSum >= 0));
		//else
			negativeSum += currentSum * float(!(currentSum >= 0));
	
	}

	return positiveSum + negativeSum;
}

//r-prefix = receiever
//s-prefix = sender
vec3 MoodProp(int rType, int rState, float sMorale, float sPressure, float sRage, int sType, int sState, float dist, int multiplier)
{
	int senderIndex = gMoodSenderIndices[sType][sState] * multiplier;
	int receieverIndex = gMoodReceiverIndices[rType][rState] * multiplier;

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
	DataIN receiver = gInput[index];
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

		int multiplier = int(indata.newDirection_team.w == receiver.newDirection_team.w || indata.newDirection_team.w  == 0);
		
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

		moods = MoodProp(rType, rState, sMorale, sPressure, sRage, sType, sState, dist, multiplier);
		//vec3 moods = MoodProp(receieverIndex, sMorale, sPressure, sRage, sType, sState, dist);
		morale += moods.x;
		rage += moods.y;
		pressure += moods.z;
	}

	return vec3(morale, rage, pressure);
}

void main()
{
	uint passCount = 0;
	passCount = ( gEntityCount + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
	
	for (uint passIt = 0; passIt < passCount; passIt++)
	{
		uint index = passIt * WORK_GROUP_SIZE + gl_LocalInvocationIndex;
		
		if (index >= gEntityCount)
			break;

		gOutput[index].newDirection_team = gInput[index].newDirection_team;
		
		if (int(gInput[index].position_unitType.w) == RIOTER_TYPE)
		{
			vec2 bestIndex = vec2(0.0f, 0.0f);

			vec3 offsetPos = vec3(
			gInput[index].position_unitType.x + 0 * gInput[index].newDirection_team.x / 2.0f,
			gInput[index].position_unitType.y + 0 * gInput[index].newDirection_team.y / 2.0f, 
			gInput[index].position_unitType.z + 0 * gInput[index].newDirection_team.z / 2.0f);
		
			float highestSum = GetEffectOnAgentAt(vec2(offsetPos.x, offsetPos.z), int(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.x), index);
			float staySum = highestSum;

			CalculatedCharge chargeSums[8];

			float chargeSum = 0;
			int groupID =  int(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.x);

			#define navMeshWallRepellVal -1000.0

			#define navMeshPF
			
			#ifdef navMeshPF
			// y = 0.01 for normalizing when the other two are zero
			vec3 navMeshWallVector = 
				vec3(	(uint(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.w) >> 30) - 1, 
						0.01, 
						((uint(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.w) >> 28) & 0x3) - 1 );			
			navMeshWallVector = normalize( navMeshWallVector ) * 
						float((uint(gInput[index].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance.w) & 0xFFFFFFF)) * 0.0001 * navMeshWallRepellVal;
			#endif


			// --------------------------------------- -1, 0 ----------------------------------------
			#ifdef navMeshPF
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 0), groupID, index) + dot( navMeshWallVector, normalize( vec3( 1, 0, 0)));
			#endif
			chargeSums[0].x = -1;																									    
			chargeSums[0].y = 0;																									    
			chargeSums[0].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 1, 0 ----------------------------------------								    
			#ifdef navMeshPF
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 0), groupID, index) + dot( navMeshWallVector, normalize( vec3( 1, 0, 0)));
			#endif
			chargeSums[1].x = 1;																									    
			chargeSums[1].y = 0;																									    
			chargeSums[1].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 0, -1 ----------------------------------------								    
			#ifdef navMeshPF
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z - 1), groupID, index) + dot( navMeshWallVector, normalize( vec3( 0, 0, 1)));
			#endif
			chargeSums[2].x = 0;																									    
			chargeSums[2].y = -1;																									    
			chargeSums[2].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 0, 1 ----------------------------------------			
			#ifdef navMeshPF					    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 0, offsetPos.z + 1), groupID, index) + dot( navMeshWallVector, normalize( vec3( 0, 0, 1)));
			#endif
			chargeSums[3].x = 0;																									    
			chargeSums[3].y = 1;																									    
			chargeSums[3].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- -1, -1 ----------------------------------------				
			#ifdef navMeshPF				    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z - 1), groupID, index) + dot( navMeshWallVector, normalize( vec3( 1, 0, 1)));
			#endif
			chargeSums[4].x = -1;																									    
			chargeSums[4].y = -1;																									    
			chargeSums[4].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 1, -1 ----------------------------------------					
			#ifdef navMeshPF			    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z - 1), groupID, index) + dot( navMeshWallVector, normalize( vec3( 1, 0, 1)));
			#endif
			chargeSums[5].x = 1;																									    
			chargeSums[5].y = -1;																									    
			chargeSums[5].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- -1, 1 ----------------------------------------					
			#ifdef navMeshPF			    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x - 1, offsetPos.z + 1), groupID, index) + dot( navMeshWallVector, normalize( vec3( 1, 0, 1)));
			#endif
			chargeSums[6].x = -1;																									    
			chargeSums[6].y = 1;																									    
			chargeSums[6].chargeSum = chargeSum;																					    
																																	    
			// ---------------------------------------- 1, 1 ----------------------------------------						
			#ifdef navMeshPF		    
			chargeSum = GetEffectOnAgentAt(vec2(offsetPos.x + 1, offsetPos.z + 1), groupID, index) + dot( navMeshWallVector, normalize( vec3( 1, 0, 1)));
			#endif
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

			gOutput[index].newDirection_team = vec4(pfVector.x, 0, pfVector.z, gInput[index].newDirection_team.w );
			
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
	
}
