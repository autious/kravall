#version 430

struct InstanceData
{
	mat4 mm;
	vec4 outlineColor;
	uint animationIndex;
	uint frameOffset;
	uint rnd_seed;
	uint pad2;
};

layout (std140, binding = 4) readonly buffer instanceBuffer
{
    InstanceData gInstances[];
};

layout (std140, binding = 6) readonly buffer animationBuffer
{
	mat4x4 gBones[];
};


layout (shared) uniform PerFrameBlock
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

mat4x4 GetBoneMatrix(InstanceData instanceData, int boneIndex)
{
	return gBones[instanceData.animationIndex + instanceData.frameOffset + boneIndex];
}

void main()
{
	InstanceData instance = gInstances[gl_InstanceID];

	// Apply animations
	vec4 posA = vec4(0.0f);
	mat4x4 boneMat;

	boneMat = GetBoneMatrix(instance, boneIndices[0]);
	posA		+= boneWeights[0]*( boneMat * positionIN	);
	
	boneMat = GetBoneMatrix(instance, boneIndices[1]);
	posA		+= boneWeights[1]*( boneMat * positionIN	);
	
	boneMat = GetBoneMatrix(instance, boneIndices[2]);
	posA		+= boneWeights[2]*( boneMat * positionIN	);
	
	boneMat = GetBoneMatrix(instance, boneIndices[3]);
	posA		+= boneWeights[3]*( boneMat * positionIN	);

	//Move position to clip space
	posFS = gProjection * gView * instance.mm * posA;

	gl_Position = posFS;
}
