#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;
//in uint rnd_seed;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap; // Used here as tone map
uniform sampler2D specularMap;
uniform sampler2D glowMap;

layout ( location = 1 ) out vec4 normalDepthRT;
layout ( location = 2 ) out vec4 diffuseRT;
layout ( location = 3 ) out vec4 specularRT;
layout ( location = 4 ) out vec4 glowMatIDRT;



void main()
{
	vec4 diffuseColor = texture2D(diffuseMap, uvFS);
	vec4 blendMap = texture2D(normalMap, uvFS);
	vec4 specColor = texture2D(specularMap, uvFS);
	vec4 glowColor = texture2D(glowMap, uvFS);

	// Blend colors
	vec4 colorR = vec4( 0.18, 0.13, 0.12, 1.0f ); // Dark skin tone
	vec4 colorG = vec4( 1.0f, 0.0f, 0.0f, 1.0f ); // Shirt
	vec4 colorB = vec4( 0.09, 0.39, 0.88, 1.0f ); // Pants
	//vec4 colorA = vec4( 1.0f, 0.0f, 1.0f, 1.0f ); // Extra
	
	diffuseColor = mix(diffuseColor, diffuseColor*colorR, blendMap.r);
	diffuseColor = mix(diffuseColor, diffuseColor*colorG, blendMap.g);
	diffuseColor = mix(diffuseColor, diffuseColor*colorB, blendMap.b);
	//diffuseColor = mix(diffuseColor, colorA, blendMap.a);

	// Save to render targets
	normalDepthRT = vec4(normalize(normalFS.xyz), posFS.z / posFS.w);
	diffuseRT = diffuseColor;
	specularRT = specColor;
	glowMatIDRT = glowColor;
}