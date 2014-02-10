#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;

uniform sampler2D gDiffuse;
uniform sampler2D gNormalDepth;

uniform float gGamma;

layout ( location = 1 ) out vec4 diffuseRT;
layout ( location = 2 ) out vec4 specularRT;
layout ( location = 3 ) out vec4 glowMatIDRT;

void main()
{

	//DO MAGIC TO GET UV FOR DECAL
	vec4 diffuse = texture2D(gDiffuse, uvFS);
	diffuse.xyz = pow(diffuse.xyz, vec3(gGamma));
}
