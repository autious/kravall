#version 430

in vec2 uv;

uniform sampler2D gDiffuse;
uniform sampler2D gNormals_depth;

out vec4 outputColor;

uniform vec4 gLightDir;
uniform vec4 gLightDiffuse;

uniform mat4 gInvViewProj;

vec4 ReconstructPosition(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = gInvViewProj * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}

void main()
{
	vec4 color = texture2D(gDiffuse, uv);
	vec4 normal_depth = texture2D(gNormals_depth, uv);

	vec4 wPos = ReconstructPosition(normal_depth.w, uv);

	vec4 normal = normalize(vec4(normal_depth.xyz, 0.0f));
	float cosAlpha = max(0.0f, dot(gLightDir.xyz, normal.xyz));

	outputColor = vec4(cosAlpha) * color;
	outputColor.w = 1.0f;
}