#version 430

in vec2 uv;

uniform sampler2D gSourceTexture;
uniform sampler2D gBloomTextures[8];
uniform vec3 gWhitePoint;
uniform float gExposure;
uniform float gGamma;

out vec4 result;

vec3 Uncharted2Tonemap(vec3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}


void main()
{

	vec4 finalBloom = vec4(0);

	for (int i = 0; i < 8; i++)
	{
		finalBloom += texture(gBloomTextures[i], uv);
	}

	vec4 sourceColor = texture(gSourceTexture, uv);
	sourceColor += finalBloom;

	sourceColor.xyz = Uncharted2Tonemap(sourceColor.xyz * gExposure) / Uncharted2Tonemap(gWhitePoint);

	//Gamma correct
	sourceColor.xyz = pow(sourceColor.xyz, vec3(1.0f / gGamma));
		

	result = vec4(sourceColor.xyz, 1);
}