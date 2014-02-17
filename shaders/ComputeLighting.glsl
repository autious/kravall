#version 430 core

#define MAX_LIGHTS 1024
#define MAX_SHADOWCASTERS 16
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

struct ShadowData
{
	mat4x4 lightMatrix1;
	mat4x4 lightMatrix2;
	mat4x4 lightMatrix3;
	mat4x4 lightMatrix4;
	vec4 reserved_for_atlas_coordinates;
};


struct SurfaceData
{
	vec4 normalDepth;
	vec4 diffuse;
	vec4 specular;
	vec4 glow;
	vec4 occlusion;
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
layout (binding = 5, rgba32f) uniform readonly image2D occlusion;

//layout (binding = 6, rg32f) uniform readonly image2D shadowMap;

uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;
uniform sampler2D shadowMap4;

layout (std430, binding = 5) readonly buffer LightBuffer
{
    LightData lights[];
};

layout (std140, binding = 8) readonly buffer ShadowDataBuffer
{
    ShadowData shadowData[];
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

uniform uint numPointShadows;
uniform uint numSpotShadows;
uniform uint numDirShadows;

uniform vec3 gWhitePoint;
uniform float gExposure;
uniform float gGamma;

shared uint minDepth;
shared uint maxDepth;

shared uint pointLightIndex[MAX_LIGHTS];
shared uint pointLightCount;

shared uint spotLightIndex[MAX_LIGHTS];
shared uint spotLightCount;

vec4 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = invProjView * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}

vec3 BlinnPhong( LightData light, SurfaceData surface, vec3 eyeDirection, vec3 lightDirection, float attenuation, inout float occlusion)
{
	lightDirection = normalize(lightDirection);

	float intensity = 0.0f;

	// Diffuse
	vec3 diffuseColor = vec3(0.0f, 0.0f, 0.0f);
	float df =  max( 0.0f, dot(surface.normalDepth.xyz, lightDirection));
	intensity = df;

	occlusion = 1 - clamp(dot(vec4(lightDirection , 1.0f), surface.occlusion), 0.0f, 1.0f);
	occlusion *= attenuation;

	diffuseColor = surface.diffuse.xyz * intensity * light.color * light.intensity * attenuation;

	// Specular
	vec3 specColor = vec3(0.0f, 0.0f, 0.0f);
	if(df > 0.0f)
	{
		vec3 H = normalize( lightDirection + eyeDirection );
		float NdotH = dot( surface.normalDepth.xyz, H );
		intensity = pow( clamp( NdotH, 0.0f, 1.0f ), surface.specular.w  * 256) * df;
	
		// Temp vars, need materials with these channels

		specColor = surface.specular.xyz * intensity * light.spec_color * light.intensity * attenuation;
	}


	return diffuseColor + specColor;
}

vec4 CalculatePointlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition, inout float occlusion)
{
	vec3 lightDir = light.position - wPos;
	
	if(length(lightDir) > light.radius_length)
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Calculate attenuation
	float dist = length( lightDir );
	float att = (pow(clamp( 1 - pow(dist / light.radius_length, 4.0f), 0.0f, 1.0f), 2.0f)) / (pow(dist, 2.0f) + 1);// More attenuations to chose from at the bottom of this file
	//float att = 1.0f / dist - 1.0f / light.radius_length;

	vec3 eyeDir = normalize(eyePosition - wPos);
	return vec4(BlinnPhong(light, surface, eyeDir, lightDir, att, occlusion), 0.0f);
}

vec4 CalculateSpotlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition, inout float occlusion)
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
		float att = spot * ((pow(clamp( 1 - pow(dist / light.radius_length, 4.0f), 0.0f, 1.0f), 2.0f)) / (pow(dist, 2.0f) + 1));// More attenuations to chose from at the bottom of this file
		//float att =  spot * (1.0f / dist - 1.0f / light.radius_length);

		vec3 eyeDir = normalize(eyePosition - wPos);
		return vec4(BlinnPhong(light, surface, eyeDir, lightDir, att, occlusion), 0.0f);
	}
	else
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
}

vec4 CalculateDirlight( LightData light, SurfaceData surface, vec3 wPos, vec3 eyePosition, inout float occlusion)
{
	vec3 lightDir = normalize(-light.orientation.xyz);
	vec3 eyeDir = normalize(eyePosition - wPos);
	float df =  max( 0.0f, dot(surface.normalDepth.xyz, lightDir));
	return vec4(BlinnPhong(light, surface, eyeDir, lightDir, 1.0f, occlusion), 0.0f);
}

float linstep(float low, float high, float v)
{
	return clamp((v-low)/(high-low), 0.0, 1.0);
}

vec3 GetDepthData(ShadowData shadowData, float dist, vec4 wPos, float farZ)
{
	vec2 moments;
	float depth;
	if(dist < farZ/10.0f)
	{
		vec4 shadowCoords = shadowData.lightMatrix1 * vec4(wPos.xyz, 1.0);
		shadowCoords.xyz /= shadowCoords.w;
		shadowCoords.xyz = shadowCoords.xyz * 0.5 + 0.5;
		vec2 uv = shadowCoords.xy;
		moments = texture2D(shadowMap1, uv).xy;
		depth = (shadowCoords.z);
		//return vec3(1.0, 0.0, 0.0);
	}
	else if(dist < farZ/5.0f)
	{
		vec4 shadowCoords = shadowData.lightMatrix2 * vec4(wPos.xyz, 1.0);
		shadowCoords.xyz /= shadowCoords.w;
		shadowCoords.xyz = shadowCoords.xyz * 0.5 + 0.5;
		vec2 uv = shadowCoords.xy;
		moments = texture2D(shadowMap2, uv).xy;
		depth = (shadowCoords.z);
		//return vec3(0.0, 1.0, 0.0);
	}
	else if(dist < farZ/2.0f)
	{
		vec4 shadowCoords = shadowData.lightMatrix3 * vec4(wPos.xyz, 1.0);
		shadowCoords.xyz /= shadowCoords.w;
		shadowCoords.xyz = shadowCoords.xyz * 0.5 + 0.5;
		vec2 uv = shadowCoords.xy;
		moments = texture2D(shadowMap3, uv).xy;
		depth = (shadowCoords.z);
		//return vec3(0.0, 0.0, 1.0);
	}
	else
	{
		vec4 shadowCoords = shadowData.lightMatrix4 * vec4(wPos.xyz, 1.0);
		shadowCoords.xyz /= shadowCoords.w;
		shadowCoords.xyz = shadowCoords.xyz * 0.5 + 0.5;
		vec2 uv = shadowCoords.xy;
		moments = texture2D(shadowMap4, uv).xy;
		depth = (shadowCoords.z);
		//return vec3(1.0, 1.0, 0.0);
	}

	return vec3(moments, depth);
}

vec4 CalculateDirlightShadow(float viewDepth, ShadowData shadowData, LightData light, SurfaceData surface, vec4 wPos, vec3 eyePosition, inout float occlusion, float farZ)
{
	//vec4 shadowCoords = lightMatrix * vec4(wPos.xyz, 1.0);
	//shadowCoords.xyz /= shadowCoords.w;
	//shadowCoords.w = 1.0;
	//shadowCoords.xyz = shadowCoords.xyz * 0.5 + 0.5;
	//float shadowmapDepth = texture2D( shadowMap, shadowCoords.xy ).x;
	//
	//float shadowFactor = float(shadowmapDepth + 0.005 > shadowCoords.z);

	vec3 dd = GetDepthData(shadowData, viewDepth, wPos, farZ);
	//return vec4(dd, 1.0);
	vec2 moments = dd.xy;
	float depth = dd.z;

	float shadowFactor;
	if(depth <= moments.x || depth >= 1.0)
	{
		shadowFactor = 1.0;
	}
	else
	{
		float p = smoothstep(depth-0.02, depth, moments.x);
		float variance = max(moments.y - moments.x * moments.x, -0.001);
		float d = depth - moments.x;
		float p_max = linstep(0.2, 1.0, variance / (variance + d*d));
		shadowFactor = clamp(pow(max(p, p_max), 20), 0.0, 1.0);
		//return vec4(moments.x);
		//return vec4(vec2(depth, depth*depth), 0.0, 1.0);
		//return vec4(moments, 0.0, 1.0);
	}
	
	vec3 lightDir = normalize(-light.orientation.xyz);
	vec3 eyeDir = normalize(eyePosition - wPos.xyz);
	float df =  max( 0.0f, dot(surface.normalDepth.xyz, lightDir));
	vec4 color = vec4(BlinnPhong(light, surface, eyeDir, lightDir, 1.0f, occlusion), 0.0f);
	return color * shadowFactor;

}

vec3 Uncharted2Tonemap(vec3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
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

		uint totalShadowcasters = numPointShadows + numSpotShadows + numDirShadows;

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
		
			lightIndex = min(lightIndex, numPointLights+totalShadowcasters-1); // Dont want this because of break above?
		
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
		
			lightIndex = min(lightIndex, numPointLights+numSpotLights+totalShadowcasters-1); // Dont want this because of break above?
		
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
		//surface.occlusion = imageLoad(occlusion, pixel) * 2.0f - 1.0f;

		vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		float sumOcclusion = 0.0f;

		//point lights
		uint i;

		
// Do shadowcasters
		for(i = 0; i < numDirShadows; i++)
		{
			float localOcclusion = 0.0f;
			mat4x4 mat = mat4x4(1.0);
			color += CalculateDirlightShadow((0.5*normalColor.w+0.5)/wPos.w, shadowData[i], lights[i], surface, wPos, eyePos, localOcclusion, zFar);
			sumOcclusion += localOcclusion;
		}


		// Do shadowcasters
// Do regular lights
		for(i = 0; i < pointLightCount; i++)
		{
			float localOcclusion = 0.0f;
			color += CalculatePointlight(lights[pointLightIndex[i]], surface, wPos.xyz, eyePos, localOcclusion);
			sumOcclusion += localOcclusion;
		}

		//spot lights
		for(i = 0; i < spotLightCount; i++)
		{
			float localOcclusion = 0.0f;
			color += CalculateSpotlight(lights[spotLightIndex[i]], surface, wPos.xyz, eyePos, localOcclusion);
			sumOcclusion += localOcclusion;
		}

		//Directional lights
		uint ofst = numPointLights + numSpotLights + totalShadowcasters;
		for(i = i + numDirShadows; i < numDirLights + ofst; i++)
		{
			float localOcclusion = 0.0f;
			color += CalculateDirlight(lights[i],  surface, wPos.xyz, eyePos, localOcclusion);
			sumOcclusion += localOcclusion;
		}
		
		ofst = numPointLights + numSpotLights + numDirLights + totalShadowcasters;

		//float light_occlusion = 1 - clamp(dot(vec4(lightDirection , 1.0f), surface.occlusion), 0.0f, 1.0f);

		// Ambient lights
		for(i = ofst; i < numAmbientLights + ofst; i++)
		{
			color += vec4(lights[i].color*lights[i].intensity, 0.0f) * surface.diffuse;
		}

		//Add glow
		color += surface.glow;
		//Add occlusion
		//color.xyz *= sumOcclusion;

		//Tone map
		//color.xyz = Uncharted2Tonemap(color.xyz * gExposure) / Uncharted2Tonemap(gWhitePoint);

		//Gamma correct
		//color.xyz = pow(color.xyz, vec3(1.0f / gGamma));
		
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
			//imageStore(outTexture, pixel, vec4(surface.occlusion.xyz,1.0f));
			//imageStore(outTexture, pixel, vec4(wPos.xyz, 1.0));
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

//Unreal Engine 4 attenuation
//float att = (pow(clamp( 1 - pow(dist / light.radius_length, 4.0f), 0.0f, 1.0f), 2.0f)) / (pow(dist, 2.0f) + 1);
