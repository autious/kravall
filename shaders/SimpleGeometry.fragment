#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D glowMap;

//out vec4 color;

layout ( location = 1 ) out vec4 normalDepthRT;
layout ( location = 2 ) out vec4 diffuseRT;
layout ( location = 3 ) out vec4 specularRT;
layout ( location = 4 ) out vec4 glowMatIDRT;

void main()
{
	normalDepthRT = vec4(normalize(normalFS.xyz), posFS.z / posFS.w);
	diffuseRT = texture2D(diffuseMap, uvFS);
	specularRT = vec4(tangentFS.xyz, 1.0f);
	glowMatIDRT = vec4(1, 0, 1, posFS.z);
}
