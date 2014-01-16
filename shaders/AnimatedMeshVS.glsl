#version 430

#define MAX_INSTANCES 1000


layout (shared) uniform PerFrameBlock
{
	mat4 gView;
	mat4 gProjection;
};

layout uniform PerInstanceBlock
{
	PerInstanceData gInstances[MAX_INSTANCES];
};

struct PerInstanceData
{
	mat4 modelMatrix;
	int animFrameID;
	int animFrameOffset;
	int pad1;
	int pad2;
};

layout ( location = 0 ) in vec4 positionIN;
layout ( location = 1 ) in vec4 normalIN;
layout ( location = 2 ) in vec4 tangentIN;
layout ( location = 3 ) in vec4 boneIndexIN;
layout ( location = 4 ) in vec4 boneWeightsIN;
layout ( location = 5 ) in vec2 uvIN;

out vec4 posFS;
out vec4 posW;
out vec4 normalFS;
out vec4 tangentFS;
out vec4 binormalFS;
out vec2 uvFS;

mat4 LoadBoneMatrix(vec3 animationData, float bone)
{
}

void main()
{
	posW = modelMatrix * positionIN;
	posFS = gProjection * gView * modelMatrix * positionIN;

	//Transform normal with model matrix
	normalFS = modelMatrix * normalIN;
	tangentFS = modelMatrix * tangentIN;
	binormalFS = modelMatrix * binormalIN;

	uvFS = uvIN;

	gl_Position = posFS;
}