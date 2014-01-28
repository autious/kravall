#version 430 core

#define MAX_LIGHTS 1024
#define MAX_LIGHTS_PER_TILE 256
#define WORK_GROUP_SIZE 16
//#define SHOW_LIGHT_TILES

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
	vec3 spec_color;
	float spot_penumbra;
	vec3 orientation;
	float spot_angle;
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

vec4 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = invProjView * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
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


		specColor = surface.specular.xyz * intensity * light.spec_color * light.intensity * attenuation;
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
	float att = 1.0f / dist - 1.0f / light.radius_length;// More attenuations to chose from at the bottom of this file

	vec3 eyeDir = normalize(eyePosition - wPos);
	return vec4(BlinnPhong(light, surface, eyeDir, lightDir, att), 0.0f);
}

vec4 CalculateSpotlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition)
{
	vec3 lightDir = light.position - wPos;
	
	float cosAngle = dot(normalize(light.orientation.xyz), normalize(-lightDir));
	float cosOuterAngle = cos(light.spot_angle);
	float cosInnerAngle = cosOuterAngle+light.spot_penumbra;
	float cosDelta = cosInnerAngle - cosOuterAngle;

	float spot = clamp((cosAngle - cosOuterAngle) / cosDelta,0.0f, 1.0f);

	if((cosAngle > cos(light.spot_angle)) && (length(lightDir) <= light.radius_length))
	{
		// Calculate attenuation
		float dist = length( lightDir );
		float att = spot * (1.0f / dist - 1.0f / light.radius_length);// More attenuations to chose from at the bottom of this file

		vec3 eyeDir = normalize(eyePosition - wPos);
		return vec4(BlinnPhong(light, surface, eyeDir, lightDir, att), 0.0f);
	}
	else
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
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

		vec2 uv = vec2(pixel.x / framebufferDim.x, pixel.y / framebufferDim.y);
		vec4 wPos = reconstruct_pos(normalColor.w , uv);
		
		float zNear = proj[3][2] / (proj[2][2] - 1.0);
		float zFar	= proj[3][2] / (proj[2][2] + 1.0);

		float clipDelta = zFar - zNear;

		float d = 0.5*(normalColor.w + 1.0);

		float viewDepth = (normalColor.w/wPos.w+zNear)/zFar;

		uint depth = uint(viewDepth * uint(0xFFFFFFFF));

		atomicMin(minDepth, depth);
		atomicMax(maxDepth, depth);

		barrier();

		float minDepthZ =  clipDelta*(float(minDepth) / float(uint(0xFFFFFFFF)));
		float maxDepthZ =  clipDelta*(float(maxDepth) / float(uint(0xFFFFFFFF)));

		vec2 tileScale = framebufferDim * (1.0f / float( 2 * WORK_GROUP_SIZE));
		vec2 tileBias = tileScale - vec2(gl_WorkGroupID.xy);

		vec4 col1 = vec4(-proj[0][0]  * tileScale.x		,0.0							,tileBias.x			,0.0);
		
		vec4 col2 = vec4(0.0							,-proj[1][1] * tileScale.y		,tileBias.y			,0.0);

		vec4 col4 = vec4(0.0							,0.0							,-1.0f				,0.0);

		vec4 frustumPlanes[6];

		//Left plane
		frustumPlanes[0] = col4 + col1;

		//right plane
		frustumPlanes[1] = col4 - col1;

		//top plane
		frustumPlanes[2] = col4 - col2;

		//bottom plane
		frustumPlanes[3] = col4 + col2;

		//far
		frustumPlanes[4] = vec4(0.0f, 0.0f, -1.0f, -minDepthZ);

		//near
		frustumPlanes[5] = vec4(0.0f, 0.0f, 1.0f, maxDepthZ);

		for(int i = 0; i < 4; i++)
		{
			frustumPlanes[i].xyz *= 1.0f / length(frustumPlanes[i].xyz);
		}
		
		LightData p;
		
		float dist;
		uint id;
		vec4 pos;
		float rad;
		bool inFrustum = false;

		uint threadCount = WORK_GROUP_SIZE * WORK_GROUP_SIZE;
		uint passCount;
		// Cull Point lights
		passCount = (numPointLights + threadCount - 1) / threadCount;
		for (uint passIt = 0; passIt < passCount; ++passIt)
		{
			uint lightIndex =  passIt * threadCount + gl_LocalInvocationIndex;
			if(lightIndex >= numPointLights)
				break;
		
			lightIndex = min(lightIndex, numPointLights-1); // Dont want this because of break above?
		
			p = lights[lightIndex];
			pos = view * vec4(p.position, 1.0f);
			rad = p.radius_length;
		
			if (pointLightCount < MAX_LIGHTS_PER_TILE)
			{
				inFrustum = true;
				for (uint i = 5; i >= 0 && inFrustum; i--)
				{
					dist = dot(frustumPlanes[i], pos);
					inFrustum = (-dist <= rad);
				}
			
				if (inFrustum)
				{
					id = atomicAdd(pointLightCount, 1);
					pointLightIndex[id] = lightIndex;
				}
			}
		}

		inFrustum = false;
		// Cull Spotlights
		passCount = (numSpotLights + threadCount - 1) / threadCount;
		for (uint passIt = 0; passIt < passCount; ++passIt)
		{
			uint lightIndex =  passIt * threadCount + gl_LocalInvocationIndex;
			if(lightIndex >= numSpotLights)
				break;

			lightIndex += numPointLights;
		
			lightIndex = min(lightIndex, numPointLights+numSpotLights-1); // Dont want this because of break above?
		
			p = lights[lightIndex];
			pos = view * vec4(p.position, 1.0f);
			rad = p.radius_length;
		
			if (spotLightCount < MAX_LIGHTS_PER_TILE)
			{
				inFrustum = true;
				for (uint i = 3; i >= 0 && inFrustum; i--)
				{
					dist = dot(frustumPlanes[i], pos);
					inFrustum = (dist > -rad);
				}
			
				if (inFrustum)
				{
					id = atomicAdd(spotLightCount, 1);
					spotLightIndex[id] = lightIndex;
				}
			}
		}

		barrier();
		
		SurfaceData surface;
		surface.normalDepth = normalColor;
		surface.diffuse = imageLoad(diffuse, pixel);
		surface.specular = imageLoad(specular, pixel);
		surface.glow = imageLoad(glowMatID, pixel);
		
		
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
			color += CalculateSpotlight(lights[spotLightIndex[i]], surface, wPos.xyz, eyePos);
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

		color += surface.glow;
		
		
		//if (gl_LocalInvocationID.x == 0 || gl_LocalInvocationID.y == 0)
		//{
		//	imageStore(outTexture, pixel, vec4(.2f, .2f, .2f, 1.0f));
		//}
		//else
		{
			//color += min(wPos, 1.0f)*0.15f;
			#ifdef SHOW_LIGHT_TILES
			color += vec4(0.0f, (pointLightCount+spotLightCount)/float(max(1,numActiveLights)), 0.0f, 0.0f);
			#endif
			//color = vec4(0.0f, (viewDepth), 0.0f, 0.0f);
			//color += vec4(float(minDepthZ<(2*d-1.0 + 0.00001f) && minDepthZ>(2*d-1.0 - 0.00001f))*0.5f, 0.0f, 0.0f, 0.0f);
			imageStore(outTexture, pixel, color);
			//imageStore(outTexture, pixel, vec4(minDepthZ));
			//imageStore(outTexture, pixel, vec4(pointLightCount / 25.0f));
			//imageStore(outTexture, pixel, vec4(vec2(tilePos.xy), 0.0f, 1.0f));
		}

}

// Some different attenuations

// invsquare attenuation which will be 0 at light radius
//att = 1/dist^2 - 1/radius^2
//float att = 1.0f / (dist * dist) - 1.0f / (light.radius_length * light.radius_length); 

// A more "deferred friendly" attenuation, slower cutoff than invsquare
//float att = 1.0f / dist - 1.0f / light.radius_length;

// Traditional point light attenuation as used in most forward shaders
//float constantAttenuation = 1.0;
//float linearAttenuation = 0.12;
//float quadraticAttenuation = 0.10;
//float att = constantAttenuation / ((1+linearAttenuation*dist)*(1+quadraticAttenuation*dist*dist));
