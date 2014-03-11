#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;
flat in uint rnd_seed;

uniform sampler2D gDiffuse;
uniform sampler2D gNormal; // Used here as tone map
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

uniform float gGamma;

layout ( location = 1 ) out vec4 gNormalDepthRT;
layout ( location = 2 ) out vec4 gDiffuseRT;
layout ( location = 3 ) out vec4 gSpecularRT;
layout ( location = 4 ) out vec4 gGlowMatIDRT;

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

// Xorshift algorithm from George Marsaglia's paper
float rand( uint seed )
{
	uint rng_state = seed;
    rng_state ^= (rng_state << 13);
    rng_state ^= (rng_state >> 17);
    rng_state ^= (rng_state << 5);
    return float( rng_state ) * (1.0 / 4294967296.0);
}

vec4 randColor( uint seed )
{
	float tr = (rand(wang_hash(seed+1)) + 1.0)/(2.0);
	float tg = (rand(wang_hash(seed+2)) + 1.0)/(2.0);
	float tb = (rand(wang_hash(seed+3)) + 1.0)/(2.0);
	return vec4( 
					tr, 
					tg, 
					tb, 
					1.0f 
				);
}

void main()
{
	vec4 diffuseColor = texture2D(gDiffuse, uvFS);
	if( diffuseColor.w < 0.5 )
		discard;
	vec4 blendMap = texture2D(gNormal, uvFS);
	vec4 specColor = texture2D(gSpecular, uvFS);
	vec4 glowColor = texture2D(gGlow, uvFS);

	diffuseColor.xyz = pow(diffuseColor.xyz, vec3(gGamma));

	// interpolate skin color from light to dark
	float factor = rand(wang_hash(rnd_seed+4));
	vec4 colorR = mix(vec4( 1.0f, 0.89, 0.78, 1.0f ), vec4( 0.09, 0.07, 0.06, 1.0f ), factor);  // Dark skin tone
	
	// Create random colors for shirts
	vec4 colorG = randColor(rnd_seed); // Shirt

	// Create random colors for pants
	vec4 colorB = randColor(2*rnd_seed);// 0.5 * (vec4(0.1f, 0.5f, 0.8f, 1.0f) + randColor(rnd_seed+52)); // Pants
	
	// Create random colors for Accessories/Extra
	//vec4 colorA = randColor(4*rnd_seed); // Extra
	
	vec4 result = diffuseColor;
	result = mix(result, diffuseColor * colorR, blendMap.r); // Skin
	result = mix(result, diffuseColor * colorG, blendMap.g); // Shirt
	result = mix(result, diffuseColor * colorB, blendMap.b); // Pants
	//result = mix(result, diffuseColor*colorA, blendMap.a); // Extra

	//result.xyz = pow(result.xyz, vec3(gGamma));

	//result.xyz = pow(result.xyz, vec3(gGamma));
	specColor.xyz = pow(specColor.xyz, vec3(gGamma));
	glowColor.xyz = pow(glowColor.xyz, vec3(gGamma));

	// Save to render targets
	gNormalDepthRT = vec4(normalize(normalFS.xyz), posFS.z / posFS.w);
	gDiffuseRT = result;
	gSpecularRT = specColor;
	gGlowMatIDRT = glowColor;
}