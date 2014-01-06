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

struct SurfaceData
{
	vec4 normalDepth;
	vec4 diffuse;
	vec4 specular;
	vec4 glow;
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

uniform vec3 eyePos;
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

vec3 BlinnPhong( LightData light, SurfaceData surface, vec3 eyeDirection, vec3 lightDirection )
{
	float dist = length( lightDirection );
	lightDirection = normalize(lightDirection); // Normalize direction vector
	//dist = dist * dist;
	//att = 1/dist^2 - 1/radius^2
	//float att = 1.0f / (dist * dist) - 1.0f / (light.radius_length * light.radius_length);
	float att = 1.0f / dist - 1.0f / light.radius_length;
	//float att = 1.0f -  dist / light.radius_length;
	//float constantAttenuation = 1.0;
    //float linearAttenuation = 0.12;
    //float quadraticAttenuation = 0.10;
	//float att = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));

	float intensity = 0.0f;

	// Diffuse
	vec3 diffuseColor = vec3(0.0f, 0.0f, 0.0f);
	float df = dot(surface.normalDepth.xyz, lightDirection);
	intensity = max( 0.0f, df );

	diffuseColor = surface.diffuse.xyz * intensity * light.color * light.intensity * att;

	// Specular
	vec3 specColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 H = normalize( lightDirection + eyeDirection );
	float NdotH = dot( surface.normalDepth.xyz, H );
	intensity = pow( clamp( NdotH, 0.0f, 1.0f ), 20.0f );
	
	// Temp vars, need materials with these channels
	float lightSpecIntensity = 1.5f;
	vec3 lightSpecColor = light.color;
	vec3 surfaceSpecColor = surface.diffuse.xyz;


	specColor = surfaceSpecColor * intensity * lightSpecColor * lightSpecIntensity * att;

	return diffuseColor + specColor;
}

vec4 CalculatePointlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition)
{
	vec3 lightDir = light.position - wPos;
	
	if(length(lightDir) > light.radius_length)
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	vec3 eyeDir = eyePosition - wPos;
	return vec4(BlinnPhong(light, surface, eyeDir, lightDir), 1.0f);

	//float attenuation = 1.0f - length(lightDir) / (p.radius_length);
	//lightDir = normalize(lightDir);
	//float diffuseFactor = max(0.0f, dot(lightDir, wNormal)) * attenuation;
	//return vec4(p.color.xyz, 1.0f) * diffuseFactor * p.intensity;
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
		
		SurfaceData surface;
		surface.normalDepth = normalColor;
		surface.diffuse = imageLoad(diffuse, pixel);
		surface.specular = imageLoad(specular, pixel);
		surface.glow = imageLoad(glowMatID, pixel);
		
		vec2 uv = vec2(pixel.x / framebufferDim.x, pixel.y / framebufferDim.y);
		vec4 wPos = vec4(reconstruct_pos(normalColor.w, uv),1.0f);
		
		vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);


		//point lights
		for(int i = 0; i < pointLightCount; i++)
		{
			color += CalculatePointlight(lights[pointLightIndex[i]], surface, wPos.xyz, eyePos);
		}

		// ambient
		color += vec4(0.05f, 0.05f, 0.05f, 0.0f) * surface.diffuse;
		
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
