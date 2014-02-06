#version 430

struct InstanceData
{
	mat4 mm;
	uint animationIndex;
	uint frameOffset;
	uint pad1;
	uint pad2;
	vec4 outlineColor;
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

mat4x4 GetBoneMatrix(InstanceData instanceData, int boneIndex)
{
	return gBones[instanceData.animationIndex + instanceData.frameOffset + boneIndex];
	//return mat4x4(1.0f);
}

void main()
{
	InstanceData instance = gInstances[gl_InstanceID];
	// Apply animations
	vec4 posA = vec4(0.0f);
	vec4 normalA = vec4(0.0f);
	vec4 tangentA = vec4(0.0f);
	mat4x4 boneMat;

	boneMat = GetBoneMatrix(instance, boneIndices[0]);
	posA		+= boneWeights[0]*( boneMat * positionIN	);
	normalA		+= boneWeights[0]*( boneMat * normalIN		);
	tangentA	+= boneWeights[0]*( boneMat * tangentIN		);
	
	boneMat = GetBoneMatrix(instance, boneIndices[1]);
	posA		+= boneWeights[1]*( boneMat * positionIN	);
	normalA		+= boneWeights[1]*( boneMat * normalIN		);
	tangentA	+= boneWeights[1]*( boneMat * tangentIN		);
	
	boneMat = GetBoneMatrix(instance, boneIndices[2]);
	posA		+= boneWeights[2]*( boneMat * positionIN	);
	normalA		+= boneWeights[2]*( boneMat * normalIN		);
	tangentA	+= boneWeights[2]*( boneMat * tangentIN		);
	
	boneMat = GetBoneMatrix(instance, boneIndices[3]);
	posA		+= boneWeights[3]*( boneMat * positionIN	);
	normalA		+= boneWeights[3]*( boneMat * normalIN		);
	tangentA	+= boneWeights[3]*( boneMat * tangentIN		);

	//Move position to clip space
	posW = instance.mm * posA;
	posFS = gProjection * gView * instance.mm * posA;
	
	//Transform normal with model matrix
	normalFS = instance.mm * normalA;
	tangentFS = instance.mm * tangentA;

	uvFS = uvIN;

	gl_Position = posFS;
}
