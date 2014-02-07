#version 430

in vec4 posFS;

out vec4 varianceMap;

void main()
{
	float depth = posFS.z/posFS.w;
	varianceMap = vec4(1, 0, 0, 1);
}