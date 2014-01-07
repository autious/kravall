#version 430 core

#define MAX_LIGHTS 1024
#define MAX_LIGHTS_PER_TILE 256
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

struct Tile
{
	uint x;
	uint y;
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
uniform uint numAmbientLights;

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

vec3 BlinnPhong( LightData light, SurfaceData surface, vec3 eyeDirection, vec3 lightDirection, float attenuation )
{
	lightDirection = normalize(lightDirection);

	float intensity = 0.0f;

	// Diffuse
	vec3 diffuseColor = vec3(0.0f, 0.0f, 0.0f);
	float df =  max( 0.0f, dot(surface.normalDepth.xyz, lightDirection));
	intensity = df;

	diffuseColor = surface.diffuse.xyz * intensity * light.color * light.intensity * attenuation;

	// Specular
	vec3 specColor = vec3(0.0f, 0.0f, 0.0f);
	if(df > 0.0f)
	{
		vec3 H = normalize( lightDirection + eyeDirection );
		float NdotH = dot( surface.normalDepth.xyz, H );
		intensity = pow( clamp( NdotH, 0.0f, 1.0f ), surface.specular.w ) * df;
	
		// Temp vars, need materials with these channels
		float lightSpecIntensity = 1.0f;
		vec3 lightSpecColor = light.color;


		specColor = surface.specular.xyz * intensity * lightSpecColor * lightSpecIntensity * attenuation;
	}
	return diffuseColor + specColor;
}

vec4 CalculatePointlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition)
{
	vec3 lightDir = light.position - wPos;
	
	if(length(lightDir) > light.radius_length)
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Calculate attenuation
	float dist = length( lightDir );
	float att = 1.0f -  dist / light.radius_length;
	//dist = dist * dist;
	//att = 1/dist^2 - 1/radius^2
	//float att = 1.0f / (dist * dist) - 1.0f / (light.radius_length * light.radius_length);
	//float att = 1.0f / dist - 1.0f / light.radius_length;
	//float constantAttenuation = 1.0;
    //float linearAttenuation = 0.12;
    //float quadraticAttenuation = 0.10;
	//float att = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));

	vec3 eyeDir = normalize(eyePosition - wPos);
	return vec4(BlinnPhong(light, surface, eyeDir, lightDir, att), 0.0f);
}

vec4 CalculateDirlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition)
{
	vec3 lightDir = normalize(-light.orientation.xyz);
	vec3 eyeDir = normalize(eyePosition - wPos);
	float df =  max( 0.0f, dot(surface.normalDepth.xyz, lightDir));
	return vec4(BlinnPhong(light, surface, eyeDir, lightDir, 1.0f), 0.0f);
}

void main()
{
		if (gl_LocalInvocationIndex == 0)
		{
			
			minDepth = 0xFFFFFFFF;
			maxDepth = 0;

			pointLightCount = 0;

			spotLightCount = 0;
		}

		barrier();

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
		uint passCount = (numPointLights + threadCount - 1) / threadCount;

		for (uint passIt = 0; passIt < passCount; ++passIt)
		{
			uint lightIndex =  passIt * threadCount + gl_LocalInvocationIndex;

			lightIndex = min(lightIndex, numPointLights);

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
		uint i;
		for(i = 0; i < pointLightCount; i++)
		{
			color += CalculatePointlight(lights[pointLightIndex[i]], surface, wPos.xyz, eyePos);
		}
		
		//spot lights
		for(i = 0; i < spotLightCount; i++)
		{
			
		}
		uint ofst;
		//Directional lights
		ofst = numPointLights + numSpotLights;
		for(i = ofst; i < numDirLights + ofst; i++)
		{
			color += CalculateDirlight(lights[i], surface, wPos.xyz, eyePos);
		}
		
		ofst = numPointLights + numSpotLights + numDirLights;
		// Ambient lights
		for(i = ofst; i < numAmbientLights + ofst; i++)
		{
			color += vec4(lights[i].color*lights[i].intensity, 0.0f) * surface.diffuse;
		}
		
		
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
