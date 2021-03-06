#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;

uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

uniform float gGamma;

layout ( location = 1 ) out vec4 normalDepthRT;
layout ( location = 2 ) out vec4 diffuseRT;
layout ( location = 3 ) out vec4 specularRT;
layout ( location = 4 ) out vec4 glowMatIDRT;

void main()
{
	vec4 diffuse = texture2D(gDiffuse, uvFS);
	diffuse.xyz = pow(diffuse.xyz, vec3(gGamma));

	vec4 spec = texture2D(gSpecular, uvFS);
	spec.xyz = pow(spec.xyz, vec3(gGamma));

	vec4 glow = texture2D(gGlow, uvFS);
	glow.xyz = pow(glow.xyz, vec3(gGamma));

	normalDepthRT = vec4(normalize(normalFS.xyz), posFS.z / posFS.w);
	diffuseRT = diffuse;
	specularRT = spec;
	glowMatIDRT = glow;
}
