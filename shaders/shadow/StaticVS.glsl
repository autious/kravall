#version 430
struct InstanceData
{
	mat4 mm;
	uint animationIndex;
	uint frameOffset;
	uint rnd_seed;
	uint pad2;
};

layout (std140, binding = 4) readonly buffer instanceBuffer
{
    InstanceData gInstances[];
};

layout (std140, binding = 0) uniform Matrices
{
	mat4 gView;
	mat4 gProjection;
};

layout ( location = 0 ) in vec4 positionIN;
layout ( location = 1 ) in vec4 normalIN;
layout ( location = 2 ) in vec4 tangentIN;
layout ( location = 3 ) in ivec4 boneIndices;
layout ( location = 4 ) in vec4 boneWeights;
layout ( location = 5 ) in vec2 uvIN;

out vec4 posFS;

void main()
{
	//Move position to clip space
	posFS = gProjection * gView * gInstances[gl_InstanceID].mm * positionIN;
	vec2 Tex = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    vec4 Pos = vec4(Tex * vec2(2,-2) + vec2(-1,1), 0, 1);
	gl_Position = Pos;//posFS;
}
