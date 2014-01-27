#version 430

#pragma optionNV(fastmath on)
#pragma optionNV(fastprecision on)
#pragma optionNV(ifcvt none)
#pragma optionNV(inline all)
#pragma optionNV(strict on)
#pragma optionNV(unroll all)

in vec2 uv;

uniform float gMaxOcclusionDistance;
uniform float gOcclusionRadius;

uniform vec2 gScreenSize;
uniform vec3 gEyePosition;

uniform mat4 gInvProjView;

uniform sampler2D gNormals_depth;
uniform sampler2D gNoiseTexture;

out vec4 resultColor;

vec4 ReconstructPosition(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = gInvProjView * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}


void main()
{
	resultColor = vec4(0.0f);

	const int num_points = 16;
    const vec3 points[32] = vec3[32]
    (
        vec3(-0.134f,  0.044f, -0.825f),
        vec3( 0.045f, -0.431f, -0.529f),
        vec3(-0.537f,  0.195f, -0.371f),
        vec3( 0.525f, -0.397f,  0.713f),
        vec3( 0.895f,  0.302f,  0.139f),
        vec3(-0.613f, -0.408f, -0.141f),
        vec3( 0.307f,  0.822f,  0.169f),
        vec3(-0.819f,  0.037f, -0.388f),
        vec3( 0.376f,  0.009f,  0.193f),
        vec3(-0.006f, -0.103f, -0.035f),
        vec3( 0.098f,  0.393f,  0.019f),
        vec3( 0.542f, -0.218f, -0.593f),
        vec3( 0.526f, -0.183f,  0.424f),
        vec3(-0.529f, -0.178f,  0.684f),
        vec3 (0.066f, -0.657f, -0.570f),
        vec3(-0.214f,  0.288f,  0.188f),
        vec3(-0.689f, -0.222f, -0.192f),
        vec3(-0.008f, -0.212f, -0.721f),
        vec3( 0.053f, -0.863f,  0.054f),
        vec3( 0.639f, -0.558f,  0.289f),
        vec3(-0.255f,  0.958f,  0.099f),
        vec3(-0.488f,  0.473f, -0.381f),
        vec3(-0.592f, -0.332f,  0.137f),
        vec3( 0.080f,  0.756f, -0.494f),
        vec3(-0.638f,  0.319f,  0.686f),
        vec3(-0.663f,  0.230f, -0.634f),
        vec3( 0.235f, -0.547f,  0.664f),
        vec3( 0.164f, -0.710f,  0.086f),
        vec3(-0.009f,  0.493f, -0.038f),
        vec3(-0.322f,  0.147f, -0.105f),
        vec3(-0.554f, -0.725f,  0.289f),
        vec3( 0.534f,  0.157f, -0.250f)
    );

	const int num_samples = 32;

	vec2 noiseTextureSize = vec2(4,4);
	vec4 normals_depth = texture2D(gNormals_depth, uv);
	vec4 position = ReconstructPosition(normals_depth.w, uv);

	float center_depth = distance(vec3(-gEyePosition.x, -gEyePosition.y, -gEyePosition.z) , position.xyz);
	float radius = gOcclusionRadius / center_depth;
	float maxDistanceInv = 1.0f / gMaxOcclusionDistance;
	float attenuation_angle_threshold = 0.1f;

	vec3 noise = texture2D(gNoiseTexture,  uv * vec2(1280, 720)  / noiseTextureSize).xyz * 2.0f - 1.0f;

	vec3 center_normal = normals_depth.xyz;
	vec4 occlusion_sh2 = vec4(0.0f);

	//These can probably tweaked for more awzum
	const float fudge_factor_l0 = 2.0f;
	const float fudge_factor_l1 = 10.0f;

	const float sh2_weight_l0 = fudge_factor_l0 * 0.28209; //0.5*sqrt(1.0/pi);
    const vec3 sh2_weight_l1 = vec3(fudge_factor_l1 * 0.48860); //0.5*sqrt(3.0/pi);

	const vec4 sh2_weight = vec4(sh2_weight_l1, sh2_weight_l0) / num_samples;

	for( int i=0; i < num_samples; ++i )
	{
		vec2 uvOffset = reflect(points[i].xy, noise.xy).xy * radius;
		vec2 sample_tex = uv + uvOffset;

		vec4 sample_normal_depth = texture2D(gNormals_depth, sample_tex);
		vec4 samplePos = ReconstructPosition(sample_normal_depth.w, sample_tex);

		vec3 center_to_sample = samplePos.xyz - position.xyz;

		float dist = length(center_to_sample);

		vec3 center_to_sample_normalized = center_to_sample / dist;

		float attenuation = 1 - clamp(dist * maxDistanceInv, 0.0, 1.0f);

		float dp = dot (center_normal, center_to_sample_normalized);

		attenuation = attenuation * attenuation * step(attenuation_angle_threshold, dp);

        occlusion_sh2 += attenuation * sh2_weight * vec4(center_to_sample_normalized , 1);
	}

	resultColor = (occlusion_sh2) * 0.5f + 0.5f;
}