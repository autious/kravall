#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;
flat in uint rnd_seed;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap; // Used here as tone map
uniform sampler2D specularMap;
uniform sampler2D glowMap;

layout ( location = 1 ) out vec4 normalDepthRT;
layout ( location = 2 ) out vec4 diffuseRT;
layout ( location = 3 ) out vec4 specularRT;
layout ( location = 4 ) out vec4 glowMatIDRT;

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
	float tr = rand(wang_hash(seed+1));
	float tg = rand(wang_hash(seed+2));
	float tb = rand(wang_hash(seed+3));
	return vec4( 
					0.3 + 0.4 * tr, 
					0.3 + 0.4 * tg, 
					0.3 + 0.4 * tb, 
					1.0f 
				);
}

void main()
{
	vec4 diffuseColor = vec4(1);//texture2D(diffuseMap, uvFS);
	vec4 blendMap = texture2D(normalMap, uvFS);
	vec4 specColor = texture2D(specularMap, uvFS);
	vec4 glowColor = texture2D(glowMap, uvFS);

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
	result = mix(result, diffuseColor*colorR, blendMap.r); // Skin
	result = mix(result, diffuseColor*colorG, blendMap.g); // Shirt
	result = mix(result, diffuseColor*colorB, blendMap.b); // Pants
	//result = mix(result, diffuseColor*colorA, blendMap.a); // Extra

	// Save to render targets
	normalDepthRT = vec4(normalize(normalFS.xyz), posFS.z / posFS.w);
	diffuseRT = result;
	specularRT = specColor;
	glowMatIDRT = glowColor;
}