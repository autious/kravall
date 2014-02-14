#version 430
#extension GL_ARB_texture_rectangle : enable

in vec4 posFS;
in vec4 posW;
in vec2 uvFS;

layout (shared) uniform PerFrameBlock
{
	mat4 gView;
	mat4 gProjection;
};

uniform sampler2D gNormalDepth;
uniform sampler2D gDiffuse;

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
	float depth = texture2D(gNormalDepth, depthUV).w;

	vec4 worldPos = reconstruct_pos(depth, depthUV);
	vec4 localPos = invModelMatrix * worldPos;

	float dist = 0.5f - abs(localPos.y);
	float dist2 = 0.5f - abs(localPos.x);

	if (dist > 0.0f && dist2 > 0.0f)
	{
		vec2 uv = vec2(localPos.x, localPos.y) + 0.5f;// * 0.5f + 0.5f;
		vec4 diffuseColor = texture2D(gDiffuse, uv);
		//diffuseRT = vec4(worldPos.xyz, 1.0f);
		//diffuseRT = vec4(uv, 0, 1);
		//diffuseRT = vec4(screenPosition, 0, 1);
		//diffuseRT = diffuseColor;
	}
	else
		diffuseRT = vec4(localPos.xyz, 1.0f);

	//diffuseRT = vec4(depthUV, 0, 1);
	//diffuseRT = vec4(depth, 0, 0, 1.0f);
	//diffuseRT = vec4(localPos.y, localPos.z, 0, 1);
}


/*

vec2 screenPosition = posFS.xy / posFS.w;

	vec2 depthUV = screenPosition * 0.5f + 0.5f;
	depthUV += vec2(0.5f / 1280.0f, 0.5f / 720.0f);
	float depth = texture2D(gNormalDepth, depthUV).w;

	vec4 worldPos = reconstruct_pos(depth, depthUV);
	vec4 localPos = invModelMatrix * worldPos;

	float dist = 0.5f - abs(localPos.y);
	float dist2 = 0.5f - abs(localPos.x);

	if (dist2 > 0.0f && dist2 > 0.0f)
	{
		vec2 uv = vec2(localPos.x, localPos.y) + 0.5f;// * 0.5f + 0.5f;
		vec4 diffuseColor = texture2D(gDiffuse, uv);
		diffuseRT = vec4(worldPos.xyz, 1.0f);
		//diffuseRT = vec4(uv, 0, 1);
		//diffuseRT = vec4(screenPosition, 0, 1);
		diffuseRT = diffuseColor;
	}
	else
		diffuseRT = vec4(1.0f);

	//diffuseRT = vec4(depthUV, 0, 1);
	//diffuseRT = vec4(depth, 0, 0, 1.0f);
	//diffuseRT = vec4(localPos.y, localPos.z, 0, 1);

*/

/*
vec2 screenPosition = posFS.xy;// / posFS.w;

	//vec2 depthUV = screenPosition * 0.5f + 0.5f;
	vec2 depthUV = screenPosition * 0.5f + 0.5f;
	depthUV += vec2(0.5f / 1280.0f, 0.5f / 720.0f);
	//depthUV += vec2(1.0f / 1280.0f, 1.0f / 720.0f);
	//float depth = texture2D(gNormalDepth, depthUV).w;
	float depth = texture2D(gNormalDepth, depthUV).w;

	//vec4 scenePosView = vec4(posFS.xy * depth / (vec2(gProjection[0][0], gProjection[1][1] * posFS.w)), -depth, 1);
	//vec4 worldPos = reconstruct_pos(depth, depthUV);
	vec4 worldPos = reconstruct_pos(depth, depthUV);
	vec4 localPos = invModelMatrix * worldPos;
	//localPos.xyz /= localPos.w;

	float dist = 0.5f - abs(localPos.xyz);
	if (dist > 0.0f)
	{
		vec2 uv = vec2(localPos.x, localPos.y) + 0.5f;// * 0.5f + 0.5f;
		vec4 diffuseColor = texture2D(gDiffuse, uv);
		diffuseRT = vec4(worldPos.xyz, 1.0f);
		//diffuseRT = vec4(uv, 0, 1);
		diffuseRT = vec4(screenPosition, 0, 1);
		//diffuseRT = diffuseColor;
	}
	else
		diffuseRT = vec4(1.0f);

	//diffuseRT = vec4(depthUV, 0, 1);
	//diffuseRT = vec4(localPos.x, 0, 0, 1.0f);
	//diffuseRT = vec4(dist, 0, 0, 1);

*/

/*

vec2 uv = (posFS.xy / posFS.w) * 0.5f + 0.5f;

	float pixelDepth = texture2D(gNormalDepth, uv.xy).w;
	vec4 worldPos = reconstruct_pos(pixelDepth, uv.xy);
	worldPos = invModelMatrix * worldPos;

	float dist = abs(worldPos.z);
	float scaledDistance = max(dist * (1.0f / 1.0f) * 2.0f, 1.0f);
	float fadeOut = 1.0f - scaledDistance;

	vec2 texCoord = worldPos.xy * 1.0f * 0.5f + 0.5f;

	//diffuseRT = vec4(worldPos.xyz, 1);

	dist = 0.5f - abs(worldPos.x);

	if (dist > 0.0f)
	{
		vec4 diffuseColor = texture2D(gDiffuse, texCoord);
		diffuseRT = vec4(diffuseColor.rgb, diffuseColor.a * fadeOut);
	}
	else
	{
		diffuseRT = vec4(0.0f);
	}

	
	//diffuseRT = vec4(fadeOut,0,0,1);

*/


/*

void main()
{
	vec2 screenPosition = posFS.xy / posFS.w;

	vec2 depthUV = screenPosition * 0.5f + 0.5f;
	//depthUV += vec2(0.5f / 1280.0f, 0.5f / 720.0f);
	depthUV += vec2(1.0f / 1280.0f, 1.0f / 720.0f);
	float depth = texture2D(gNormalDepth, depthUV).w;

	//vec4 scenePosView = vec4(posFS.xy * depth / (vec2(gProjection[0][0], gProjection[1][1] * posFS.w)), -depth, 1);
	//vec4 worldPos = reconstruct_pos(depth, depthUV);
	vec4 worldPos = reconstruct_pos(depth, depthUV);
	vec4 localPos = invModelMatrix * worldPos;
	float dist = 0.5f - abs(localPos.xyz);
	if (dist >= 0.0f)
	{
		vec2 uv = vec2(localPos.x, localPos.y) + vec2(0.5f);
		vec4 diffuseColor = texture2D(gDiffuse, uv);
		diffuseRT = vec4(uv, 0, 1);
		diffuseRT = diffuseColor;
	}
	else
		diffuseRT = vec4(0.0f);
}

*/

/*

vec2 uv = posFS.xy * 0.5f + 0.5f;

	float pixelDepth = texture2D(gNormalDepth, uv.xy).w;
	vec4 worldPos = reconstruct_pos(pixelDepth, uv.xy);
	worldPos = invModelMatrix * worldPos;

	vec2 texCoord = worldPos.xy * 1.0f / 1.0f * 0.5f + 0.5f;

	//diffuseRT = vec4(worldPos.xyz, 1);
	diffuseRT = texture2D(gDiffuse, texCoord);

*/


/*

//vec4 normal_depth = texture2D(gNormalDepth, uvFS);
	//vec4 gBufferPos = reconstruct_pos(normal_depth.w, uvFS);
	
	//vec2 uv = posFS.xy * vec2(1.0f / 1280.0f, 1.0f / 720.0f);
	vec2 uv = posFS.xy  * 0.5 + 0.5f;
	//vec4 normal_depth = texture2D(gNormalDepth, uv.xy);
	//vec4 gBufferPos = reconstruct_pos(normal_depth.w,  uv.xy);

	vec4 normal_depth = texture2D(gNormalDepth, uv);
	vec4 gBufferPos = reconstruct_pos(normal_depth.w, uv);

	//Transform the gBufferPos into decals local space
	vec4 decalPos = invModelMatrix  * gBufferPos;
	//decalPos.xyz /= decalPos.w;

	//if (distance(decalPos.xy, posFS.xy) > 1.5)
	//	diffuseRT = vec4(0);
	//else
	//{
	
		float dist = abs(decalPos.z);
		float scaledDistance = max(dist * (1.0f / 1.0f) * 2.0f, 1.0f);
		float fadeOut = 1.0f - scaledDistance;

		//use the xy position of the position for the texture lookup
		//vec2 texcoord = decalPos.xy * (1.0f / textureSize(gDiffuse, 0)) * 0.5 + 0.5;
		vec2 texcoord = decalPos.xy * (1.0f / 1.0f) * 0.5 + 0.5;

		vec4 diff = texture2D(gDiffuse, texcoord);

		diffuseRT = vec4(diff.xyz, diff.w * fadeOut);
	//}


*/