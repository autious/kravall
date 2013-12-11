#version 430

uniform float roll;

uniform writeonly image2D outTexture;
//uniform sampler2D diffuse;
uniform sampler2D normal;
//uniform sampler2D specular;
//uniform sampler2D glowMatID;

layout (local_size_x = 16, local_size_y = 16) in;

void main()
{
	vec2 storePos = vec2(gl_GlobalInvocationID.xy);

	vec4 normalColor = texture2D(normal, vec2(storePos.x / 1280.0f, storePos.y / 720.0f));
	imageStore(outTexture, ivec2(gl_GlobalInvocationID.xy), vec4(storePos.x, storePos.y, 0.0, 1.0f));



	//imageStore(outTexture, ivec2(gl_GlobalInvocationID.xy), vec4(normalColor.xyz, 1.0f));
	//float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy) - 8) / 8.0f);
	//float globalCoef = sin(float(gl_WorkGroupID.x + gl_WorkGroupID.y) * 0.1f + roll) * 0.5f;

	//imageStore(outTexture, storePos, vec4(1.0f - globalCoef * localCoef, 0.0f, 0.0f, 1.0f));
}