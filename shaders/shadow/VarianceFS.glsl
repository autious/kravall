#version 430

in vec4 posFS;

layout ( location = 1 ) out vec2 varianceMap;

void main()
{
	float depth = posFS.z;
	varianceMap.x = depth;
	varianceMap.y = depth * depth;
}