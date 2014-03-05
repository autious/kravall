#version 430
#extension GL_ARB_texture_rectangle : enable

in vec4 posFS;
in vec4 posW;
in vec3 orientationFS;
in vec2 uvFS;

layout (shared) uniform PerFrameBlock
{
	mat4 gView;
	mat4 gProjection;
};

uniform sampler2D gNormalDepth;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

uniform float gGamma;

uniform mat4 invProjView;
uniform mat4 invModelMatrix;

vec4 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = invProjView * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}


layout ( location = 1 ) out vec4 diffuseRT;
layout ( location = 2 ) out vec4 specularRT;
layout ( location = 3 ) out vec4 glowMatIDRT;

void main()
{
	vec2 screenPosition = posFS.xy / posFS.w;

	vec2 depthUV = screenPosition * 0.5f + 0.5f;
	depthUV += vec2(0.5f / 1280.0f, 0.5f / 720.0f);
	vec4 normal_depth = texture2D(gNormalDepth, depthUV);
	float depth = normal_depth.w;

	vec4 worldPos = reconstruct_pos(depth, depthUV);
	worldPos.w = 1.0f;
	vec4 localPos =  invModelMatrix * worldPos;

	float dist =  0.5f - abs(localPos.xyz);

	float normalThreshold = dot(normal_depth.xyz, orientationFS) - cos(3.14f / 2.0f);

	if (dist >= 0.0f && normalThreshold > 0.0f)
	{
		vec2 uv = vec2(localPos.x, localPos.z) + 0.5f;
		vec4 diffuseColor = texture2D(gDiffuse, uv);
		diffuseColor.xyz = pow(diffuseColor.xyz, vec3(gGamma));

		vec4 glowColor = texture2D(gGlow, uv);
		glowColor.xyz = pow(glowColor.xyz, vec3(gGamma));

		vec4 specColor = texture2D(gSpecular, uv);
		specColor.xyz = pow(specColor.xyz, vec3(gGamma));

		diffuseRT = diffuseColor;
		glowMatIDRT = glowColor;
		specularRT = specColor;
	}
	else
	{
		diffuseRT = vec4(0.0f);
		glowMatIDRT = vec4(0.0f);
		specularRT = vec4(0.0f);
	}
}