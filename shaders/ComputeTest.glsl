#version 430

uniform float roll;

layout (binding = 0, rgba32f) uniform writeonly image2D outTexture;
//uniform sampler2D diffuse;
layout (binding = 1, rgba32f) uniform readonly image2D normal;
//uniform sampler2D specular;
//uniform sampler2D glowMatID;

layout (local_size_x = 16, local_size_y = 16) in;

void main()
{
	const ivec2 tile_xy = ivec2(gl_WorkGroupID);
	const ivec2 thread_xy = ivec2(gl_LocalInvocationID);
	const ivec2 pixel_xy = tile_xy * ivec2(16,16) + thread_xy;

	vec4 pixel = imageLoad(normal, pixel_xy);
	//pixel = vec4(pixel.x * 255.0f, pixel.y * 255.0f, pixel.z * 255.0f, 1.0f);
	imageStore(outTexture, pixel_xy, pixel);


	//vec2 storePos = vec2(gl_GlobalInvocationID.xy);

	//vec4 normalColor = texture2D(normal, vec2(storePos.x / 1280.0f, storePos.y / 720.0f));
	//vec4 normalColor = imageLoad(normal, ivec2(gl_GlobalInvocationID.xy));
	
	//imageStore(outTexture, ivec2(gl_GlobalInvocationID.xy), vec4(storePos.x, storePos.y, 0.0, 1.0f));

	//imageStore(outTexture, ivec2(gl_GlobalInvocationID.xy), vec4(normalColor.xyz, 1.0f));

	//float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy) - 8) / 8.0f);
	//float globalCoef = sin(float(gl_WorkGroupID.x + gl_WorkGroupID.y) * 0.1f + roll) * 0.5f;

	//imageStore(outTexture, storePos, vec4(1.0f - globalCoef * localCoef, 0.0f, 0.0f, 1.0f));
}