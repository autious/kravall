#version 430

in vec2 uv;

uniform sampler2D gNormals_depth;
uniform sampler2D gDiffuse;

uniform mat4 gInvProjView;

uniform vec4 gLightDirection;
uniform vec4 gLightDiffuse;

out vec4 resultColor;

vec4 ReconstructPosition(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = gInvProjView * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}

void main()
{
	vec4 normals_depth = texture2D(gNormals_depth, uv);
	vec4 position = ReconstructPosition(normals_depth.w, uv);
	vec4 diffuse = texture2D(gDiffuse, uv);

	normals_depth.w = 0.0f;
	normals_depth = normalize(normals_depth);
	float cosAlpha = max(0.0f, dot(gLightDirection.xyz, normals_depth.xyz));

	vec4 diffuseColor = vec4(cosAlpha) * diffuse;

	resultColor = diffuseColor;
	resultColor.a = 1.0f;
}