#version 430

struct InstanceData
{
	mat4 mm;
	vec4 outlineColor;
	uint animationFrame;
	uint animationOffset;
	uint rnd_seed;
	uint pad2;
};

layout (std140, binding = 4) readonly buffer buffah
{
    InstanceData gInstances[];
};

layout (shared) uniform PerFrameBlock
{
	mat4 gView;
	mat4 gProjection;
};

uniform mat4 modelMatrix;

layout ( location = 0 ) in vec4 positionIN;
layout ( location = 1 ) in vec4 normalIN;
layout ( location = 2 ) in vec4 tangentIN;
layout ( location = 3 ) in ivec4 boneIndices;
layout ( location = 4 ) in vec4 boneWeights;
layout ( location = 5 ) in vec2 uvIN;

out vec4 posFS;
out vec4 posW;
out vec4 normalFS;
out vec4 tangentFS;
out vec2 uvFS;

void main()
{
#define INSTANCED
#ifdef INSTANCED
	//Move position to clip space
	posW = gInstances[gl_InstanceID].mm * positionIN;
	posFS = gProjection * gView * gInstances[gl_InstanceID].mm * positionIN;
	
	//Transform normal with model matrix
	normalFS = gInstances[gl_InstanceID].mm * normalIN;
	tangentFS = gInstances[gl_InstanceID].mm * tangentIN;
	//binormalFS = gInstances[gl_InstanceID].mm * binormalIN;

#else

	//Move position to clip space
	posW = modelMatrix * positionIN;
	posFS = gProjection * gView * modelMatrix * positionIN;
	
	//Transform normal with model matrix
	normalFS = modelMatrix * normalIN;
	tangentFS = modelMatrix * tangentIN;
	//binormalFS = modelMatrix * binormalIN;

#endif

	uvFS = uvIN;

	gl_Position = posFS;
}
