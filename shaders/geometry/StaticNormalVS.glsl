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

layout (std140, binding = 3) uniform instanceBufferOffset
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
out vec4 posW;
out vec4 normalFS;
out vec4 tangentFS;
out vec2 uvFS;

void main()
{
	uint instanceID = gInstanceOffset + gl_InstanceID;
	//Move position to clip space
	posW = gInstances[instanceID].mm * positionIN;
	//posFS = gProjection * gView * gInstances[instanceID].mm * (positionIN+vec4(50*gInstanceOffset,0,0,0));
	posFS = gProjection * gView * (positionIN+vec4(50*gInstanceOffset,0,0,0));
	
	//Transform normal with model matrix
	normalFS = gInstances[instanceID].mm * normalIN;
	tangentFS = gInstances[instanceID].mm * tangentIN;
	//binormalFS = gInstances[instanceID].mm * binormalIN;

	uvFS = uvIN;

	gl_Position = posFS;
}
