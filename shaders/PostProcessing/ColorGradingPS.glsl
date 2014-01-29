#version 430

in vec2 uv;

uniform sampler2D gSourceTexture;
uniform sampler3D gLut;

out vec4 outputColor;

void main()
{
	outputColor = vec4(1.0f);
	vec3 lutSize = vec3(32.0f);

	vec3 sourceColor = texture2D(gSourceTexture, uv).xyz;
	vec3 scale = (lutSize - 1.0f) / lutSize;
	vec3 offset = 1.0f / (2.0f * lutSize);

	sourceColor = clamp(sourceColor, vec3(0.0f), vec3(1.0f));

	float g = sourceColor.y;
	float b = sourceColor.z;

	sourceColor.y = b;
	sourceColor.z = g;

	vec3 newUV = scale * sourceColor + offset;

	//outputColor = texture2D(gSourceTexture, uv);
	outputColor = texture(gLut, (newUV));

	outputColor = vec4(outputColor.xyz, 1.0f);
}