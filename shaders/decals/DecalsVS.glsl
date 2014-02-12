#version 430

layout (shared) uniform PerFrameBlock
{
	mat4 gView;
	mat4 gProjection;
};

uniform mat4 modelMatrix;
uniform vec3 decalSize;

layout ( location = 0 ) in vec4 positionIN;
layout ( location = 1 ) in vec4 normalIN;
layout ( location = 2 ) in vec4 tangentIN;
layout ( location = 3 ) in ivec4 boneIndices;
layout ( location = 4 ) in vec4 boneWeights;
layout ( location = 5 ) in vec2 uvIN;

out vec4 posFS;
out vec2 uvFS;
void main()
{
	//Move position to clip space
	posFS = gProjection * gView * modelMatrix * vec4(positionIN.xyz * decalSize, positionIN.w);

	gl_Position = posFS;
}