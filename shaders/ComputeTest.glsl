#version 430 core

#define MAX_LIGHTS 4096
#define MAX_LIGHTS_PER_TILE 1024
#define WORK_GROUP_SIZE 16

#pragma optionNV(fastmath on) 
#pragma optionNV(ifcvt none) 
#pragma optionNV(inline all) 
#pragma optionNV(strict on) 
#pragma optionNV(unroll all)

layout (local_size_x = WORK_GROUP_SIZE, local_size_y = WORK_GROUP_SIZE) in;

struct LightData
{
	vec3 position;
	float radius_length;
	vec3 color;
	float intensity;
	vec3 pad;
	float spot_angle;
	vec4 orientation;
};

layout (binding = 0, rgba32f) uniform writeonly image2D outTexture;
layout (binding = 1, rgba32f) uniform readonly image2D normalDepth;
layout (binding = 2, rgba32f) uniform readonly image2D diffuse;
layout (binding = 3, rgba32f) uniform readonly image2D specular;
layout (binding = 4, rgba32f) uniform readonly image2D glowMatID;

layout (std430, binding = 5) readonly buffer BufferObject
{
    LightData lights[];
};

uniform mat4 view;
uniform mat4 proj;
uniform mat4 invProjView;
uniform vec2 framebufferDim;

uniform uint numActiveLights;
uniform uint numPointLights;
uniform uint numSpotLights;
uniform uint numDirLights;

shared uint minDepth = 0xFFFFFFFF;
shared uint maxDepth = 0;

shared uint pointLightIndex[MAX_LIGHTS];
shared uint pointLightCount = 0;

shared uint spotLightIndex[MAX_LIGHTS];
shared uint spotLightCount = 0;

vec3 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = invProjView * sPos;
     
    return (sPos.xyz / sPos.w);
}

vec4 CalculateLighting( LightData p, vec3 wPos, vec3 wNormal, vec4 wSpec, vec4 wGlow)
{
	//vec4 outColor = vec4(0.0f);
	//
	//vec4 ambient = vec4(0.0f);
	//vec4 diffuse = vec4(0.0f);
	//vec4 spec = vec4(0.0f);
	//
	//vec3 lightVec = p.position - wPos;
	//float dist = length(lightVec);
	//
	//if (d > p.range_attenuation)
	//	return vec4(0.0f);
	//
	//lightVec /= d;

	vec3 direction = p.position - wPos;
	
	if(length(direction) > p.radius_length)
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float attenuation = 1.0f - length(direction) / (p.radius_length);
	direction = normalize(direction);
	float diffuseFactor = max(0.0f, dot(direction, wNormal)) * attenuation;
	return vec4(p.color.xyz, 1.0f) * diffuseFactor * p.intensity;
}

void main()
{
        ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

        vec4 normalColor = imageLoad(normalDepth, pixel);

		float d = normalColor.w;

		uint depth = uint(d * 0xFFFFFFFF);

		atomicMin(minDepth, depth);
		atomicMax(maxDepth, depth);

		barrier();

		float minDepthZ = float(minDepth / float(0xFFFFFFFF));
		float maxDepthZ = float(maxDepth / float(0xFFFFFFFF));

		vec2 tileScale = framebufferDim * (1.0f / float( 2 * WORK_GROUP_SIZE));
		vec2 tileBias = tileScale - vec2(gl_WorkGroupID.xy);

		vec4 col1 = vec4(-proj[0][0]  * tileScale.x, proj[0][1], tileBias.x, proj[0][3]); 
		
		vec4 col2 = vec4(proj[1][0], -proj[1][1] * tileScale.y, tileBias.y, proj[1][3]);

		vec4 col4 = vec4(proj[3][0], proj[3][1],  -1.0f, proj[3][3]); 

		vec4 frustumPlanes[6];

		//Left plane
		frustumPlanes[0] = col4 + col1;

		//right plane
		frustumPlanes[1] = col4 - col1;

		//top plane
		frustumPlanes[2] = col4 - col2;

		//bottom plane
		frustumPlanes[3] = col4 + col2;

		//near
		frustumPlanes[4] =vec4(0.0f, 0.0f, -1.0f,  -minDepthZ);

		//far
		frustumPlanes[5] = vec4(0.0f, 0.0f, -1.0f,  maxDepthZ);

		for(int i = 0; i < 4; i++)
		{
			frustumPlanes[i] *= 1.0f / length(frustumPlanes[i].xyz);
		}
		
		LightData p;
		
		float dist;
		uint id;
		vec4 pos;
		float rad;
		bool inFrustum;

		uint threadCount = WORK_GROUP_SIZE * WORK_GROUP_SIZE;
		uint passCount = (numActiveLights + threadCount - 1) /threadCount;

		for (uint passIt = 0; passIt < passCount; ++passIt)
		{
			uint lightIndex =  passIt * threadCount + gl_LocalInvocationIndex;

			lightIndex = min(lightIndex, numActiveLights);

			p = lights[lightIndex];
			pos = view * vec4(p.position, 1.0f);
			rad = p.radius_length;

			if (pointLightCount < MAX_LIGHTS_PER_TILE)
			{
				inFrustum = true;
				for (uint i = 3; i >= 0 && inFrustum; i--)
				{
					dist = dot(frustumPlanes[i], pos);
					inFrustum = (-rad <= dist);
				}
			
				if (inFrustum)
				{
					id = atomicAdd(pointLightCount, 1);
					pointLightIndex[id] = lightIndex;
				}
			}
		}


		//for (uint lightIndex = gl_LocalInvocationIndex; lightIndex < numActiveLights; lightIndex += WORK_GROUP_SIZE)
		//{
		//	p = pointLights[lightIndex];
		//	pos = view * vec4(p.position, 1.0f);
		//	rad = p.radius_length;
		//
		//	if (pointLightCount < MAX_LIGHTS_PER_TILE)
		//	{
		//		bool inFrustum = true;
		//		for (uint i = 3; i >= 0 && inFrustum; i--)
		//		{
		//			dist = dot(frustumPlanes[i], pos);
		//			//inFrustum = ((i % lightIndex) == 0);
		//			inFrustum = (-rad <= dist);
		//		}
		//
		//		if (inFrustum)
		//		{
		//			id = atomicAdd(pointLightCount, 1);
		//			pointLightIndex[id] = lightIndex;
		//		}
		//	}
		//}

		barrier();

		vec4 diffuseColor = imageLoad(diffuse, pixel);
		vec4 specular = imageLoad(specular, pixel);
		vec4 glow = imageLoad(glowMatID, pixel);
		
		vec2 uv = vec2(pixel.x / framebufferDim.x, pixel.y / framebufferDim.y);
		vec4 wPos = vec4(reconstruct_pos(normalColor.w, uv),1.0f);
		
		vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		//point lights
		for(int i = 0; i < pointLightCount; i++)
		{
			color += CalculateLighting(lights[pointLightIndex[i]], wPos.xyz, 2 * normalColor.xyz - 1.0f, specular, glow) * diffuseColor;
		}

		// ambient
		color += vec4(0.05f, 0.05f, 0.05f, 0.0f) * diffuseColor;
		
		//if (gl_LocalInvocationID.x == 0 || gl_LocalInvocationID.y == 0)
		//	imageStore(outTexture, pixel, vec4(.2f, .2f, .2f, 1.0f));
		//else
		{
			imageStore(outTexture, pixel, color);
			
			//imageStore(outTexture, pixel, vec4(minDepthZ));
			//imageStore(outTexture, pixel, vec4(pointLightCount / 25.0f));
			//imageStore(outTexture, pixel, vec4(vec2(tilePos.xy), 0.0f, 1.0f));
		}

}
