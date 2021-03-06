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

layout (std140) uniform instanceBufferOffset
{
    uint gInstanceOffset;
	uint pad0;
	uint pad1;
	uint pad2;
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
out vec4 outlineColor;

void main()
{
	uint instanceID = gInstanceOffset + gl_InstanceID;
	//Move position to clip space
	posFS = gProjection * gView * gInstances[instanceID].mm * positionIN;
	
	outlineColor = gInstances[instanceID].outlineColor;

	gl_Position = posFS;
}
