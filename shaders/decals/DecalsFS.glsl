#version 430
#extension GL_ARB_texture_rectangle : enable
in vec4 posFS;
in vec2 uvFS;

uniform sampler2DRect gNormalDepth;
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
	vec2 uv = posFS.xy;// * 0.5f + 0.5f;
	float pixelDepth = texture2DRect(gNormalDepth, gl_FragCoord.xy).w;
	vec4 worldPos = reconstruct_pos(pixelDepth, uv.xy);

	diffuseRT = vec4(worldPos.xyz, 1);//vec4(pixelDepth, 0, 0, 1);//vec4(pixelDepth, 0, 0, 1);
}


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