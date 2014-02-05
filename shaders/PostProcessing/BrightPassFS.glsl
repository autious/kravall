#version 430
#define BRIGHT_PASS_THRESHOLD 0.9
#define BRIGHT_PASS_OFFSET 1.0

in vec2 uv;

uniform sampler2D gSourceTexture;
uniform vec3 gWhitePoint;
uniform float gExposure;

out vec4 outputColor;

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
	vec4 source = texture2D(gSourceTexture, uv);
	outputColor = vec4(0, 0, 0, 1);

	vec3 c = Uncharted2Tonemap(source.xyz * gExposure) / Uncharted2Tonemap(gWhitePoint);
	vec3 c2 = vec3(1.0f);


	c -= BRIGHT_PASS_THRESHOLD;
	c = max(c, vec3(0));
	
	c /= (BRIGHT_PASS_OFFSET + c);
	outputColor.xyz = c;
}