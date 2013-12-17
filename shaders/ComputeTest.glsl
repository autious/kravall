#version 430 core

#define MAX_LIGHTS 1024
#define MAX_LIGHTS_PER_TILE 40

#define WORK_GROUP_SIZE 16

//struct PointLight
//{
//	vec3 position;
//	float radius;
//	vec3 color;
//	float intensity;
//};
struct PointLight
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

layout (std430, binding = 5) buffer BufferObject
{
    PointLight pointLights[];
};

uniform mat4 view;
uniform mat4 invProj;
uniform mat4 proj;
uniform mat4 inv_proj_view_mat;
uniform uint numActiveLights;
uniform vec2 framebufferDim;

layout (local_size_x = WORK_GROUP_SIZE, local_size_y = WORK_GROUP_SIZE) in;

shared uint minDepth = 0xFFFFFFFF;
shared uint maxDepth = 0;
shared uint pointLightIndex[MAX_LIGHTS];
shared uint pointLightCount = 0;

vec3 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = inv_proj_view_mat * sPos;
     
    return (sPos.xyz / sPos.w);
}

vec4 CalculateLighting( PointLight p, vec3 wPos, vec3 wNormal, vec4 wSpec, vec4 wGlow)
{
	vec3 direction = p.position - wPos;

	if(length(direction) > p.radius_length)
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);

	float attenuation = 1.0f - length(direction) / (p.radius_length);
	direction = normalize(direction);
	float diffuseFactor = max(0.0f, dot(direction, wNormal)) * attenuation;
	return vec4(p.color.xyz, 0.0f) * diffuseFactor * p.intensity;
}

vec4 ConvertProjToView( vec4 p )
{
    p = invProj * p;
    p /= p.w;
    return p;
}

// calculate the number of tiles in the horizontal direction
uint GetNumTilesX()
{
    return uint(( ( 1280 + WORK_GROUP_SIZE - 1 ) / float(WORK_GROUP_SIZE) ));
}

// calculate the number of tiles in the vertical direction
uint GetNumTilesY()
{
    return uint(( ( 720 + WORK_GROUP_SIZE - 1 ) / float(WORK_GROUP_SIZE) ));
}


vec4 CreatePlaneEquation( vec4 b, vec4 c )
{
    vec4 n;

    // normalize(cross( b.xyz-a.xyz, c.xyz-a.xyz )), except we know "a" is the origin
     n.xyz = normalize(cross( b.xyz, c.xyz ));

    // -(n dot a), except we know "a" is the origin
    n.w = 0;

    return n;
}

float GetSignedDistanceFromPlane( vec4 p, vec4 eqn )
{
    // dot( eqn.xyz, p.xyz ) + eqn.w, , except we know eqn.w is zero 
    // (see CreatePlaneEquation above)
    return dot( eqn.xyz, p.xyz );
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

				//total tiles = tileScale * 2
		vec2 tileScale = vec2(1280, 720) * (1.0f / float( 2 * WORK_GROUP_SIZE));
		vec2 tileBias = tileScale - vec2(gl_WorkGroupID.xy);
		
		vec4 c1 = vec4(proj[0][0] * tileScale.x, 0.0f, tileBias.x, 0.0f);
		vec4 c2 = vec4(0.0f, proj[2][2] * tileScale.y, tileBias.y, 0.0f);
		vec4 c4 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		
		 // Derive frustum planes
		vec4 frustumPlanes[6];
		// Sides
		//right
		frustumPlanes[0] = c4 - c1;
		//left
		frustumPlanes[1] = c4 + c1;
		//bottom
		frustumPlanes[2] = c4 - c2;
		//top
		frustumPlanes[3] = c4 + c2;
		// Near/far
		frustumPlanes[4] = vec4(0.0f, 0.0f,  1.0f, -minDepthZ);
		frustumPlanes[5] = vec4(0.0f, 0.0f, -1.0f,  maxDepthZ);
		
		for(int i = 0; i < 4; i++)
		{
			frustumPlanes[i] *= 1.0f / length(frustumPlanes[i].xyz);
		}
		
		//DO CULLING HERE
		for (uint lightIndex = gl_LocalInvocationIndex; lightIndex < numActiveLights; lightIndex += WORK_GROUP_SIZE)
		{
			PointLight p = pointLights[lightIndex];
			
			if (lightIndex < numActiveLights)
			{

				bool inFrustum = true;
				for (uint i = 0; i < 6; i++)
				{
					float d = dot(frustumPlanes[i], proj * view * vec4(p.position, 1.0f));
					inFrustum = inFrustum && (d >= -p.radius_length);
				}
		
				if (inFrustum)
				{
					uint id = atomicAdd(pointLightCount, 1);
					pointLightIndex[id] = lightIndex;
				}
			}
		}

		barrier();

		vec4 diffuseColor = imageLoad(diffuse, pixel);
		vec4 specular = imageLoad(specular, pixel);
		vec4 glow = imageLoad(glowMatID, pixel);

		//float diffuseFactor = max(0, dot(normalize(hardCodedDirectionalLightDirection), normalize(normalColor.xyz)));
		vec2 uv = vec2(pixel.x / 1280.0f, pixel.y / 720.0f);
		vec4 wPos = vec4(reconstruct_pos(normalColor.w, uv),1.0f);

		//vec4 color = wPos;
		vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		//point lights
		//#pragma optionNV(unroll all)
		for(int i = 0; i < pointLightCount; i++)
		{
			color += CalculateLighting(pointLights[i], wPos.xyz, 2*normalColor.xyz-1.0f, specular, glow) * diffuseColor;
		}

		// ambient
		//color += vec4(0.05f, 0.05f, 0.05f, 0.0f) * diffuseColor;
		
		barrier();

		if (gl_LocalInvocationID.x == 0 || gl_LocalInvocationID.y == 0 || gl_LocalInvocationID.x == 16 || gl_LocalInvocationID.y == 16)
			imageStore(outTexture, pixel, vec4(.2f, .2f, .2f, 1.0f));
		else
		{
			imageStore(outTexture, pixel, 100 * color);
			//imageStore(outTexture, pixel, vec4(maxDepthZ));
			imageStore(outTexture, pixel, vec4(pointLightCount));
			//imageStore(outTexture, pixel, vec4(vec2(tilePos.xy), 0.0f, 1.0f));
		}

}



/*

//vec4 bottomLeft = vec4(vec2(tilePos), minDepth / float(0xFFFFFFFF), 1.0f);
		//vec4 topRight = vec4(tilePos.x + WORK_GROUP_SIZE, tilePos.y + WORK_GROUP_SIZE, maxDepth/ float(0xFFFFFFFF), 1.0f);

		vec4 bottomLeft = invProj * vec4(0, 0, 0, 1.0f);
		vec4 topRight = invProj * vec4(1, 1, 1,1);


		//  t0   t1   t2   t3	..   tn
		//|----|----|----|----|....|----|

		uint stride = MAX_LIGHTS / (WORK_GROUP_SIZE * WORK_GROUP_SIZE);
		for( uint i = 0; i < stride; i++ )
		{
			uint lightIndex = i + stride * gl_LocalInvocationIndex;
			vec4 pos = vec4(pointLights[lightIndex].position, 1.0f);
			float radius = pointLights[lightIndex].radius;
			
			bool collided = false;

			vec3 cpiabb = min(max(pos.xyz, bottomLeft.xyz), topRight.xyz);
			float dsqrt = length(cpiabb - pos.xyz);

			collided = dsqrt < (radius * radius);

			if(collided)
			{	
				uint id = atomicAdd(pointLightCount, 1);
				pointLightIndex[id] = lightIndex;
			}

		}
		
*/

/*
	////number of tiles in the range [0, 1]
		//total tiles = tileScale * 2
		vec2 tileScale = vec2(1280, 720) * (1.0f / float(2 * WORK_GROUP_SIZE));
		vec2 tileBias = tileScale - vec2(gl_WorkGroupID.xy);
		
		vec4 c1 = vec4(viewProj[0][0] * tileScale.x, 0.0f, tileBias.x, 0.0f);
		vec4 c2 = vec4(0.0f, viewProj[2][2] * tileScale.y, tileBias.y, 0.0f);
		vec4 c4 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		
		 // Derive frustum planes
		vec4 frustumPlanes[6];
		// Sides
		//right
		frustumPlanes[0] = c4 - c1;
		//left
		frustumPlanes[1] = c4 + c1;
		//bottom
		frustumPlanes[2] = c4 - c2;
		//top
		frustumPlanes[3] = c4 + c2;
		// Near/far
		frustumPlanes[4] = vec4(0.0f, 0.0f,  1.0f, -minDepthZ);
		frustumPlanes[5] = vec4(0.0f, 0.0f, -1.0f,  maxDepthZ);
		
		for(int i = 0; i < 4; i++)
		{
			frustumPlanes[i] *= 1.0f / length(frustumPlanes[i].xyz);
		}
		
		//DO CULLING HERE
		
		for (uint lightIndex = gl_LocalInvocationIndex; lightIndex < MAX_LIGHTS; lightIndex += WORK_GROUP_SIZE)
		{
			PointLight p = pointLights[lightIndex];
		
			bool inFrustum = true;
			for (uint i = 0; i < 6; i++)
			{
				float d = dot(frustumPlanes[i], view * vec4(p.position, 1.0f));
				inFrustum = inFrustum && (d >= -p.radius);
			}
		
			if (inFrustum)
			{
				uint id = atomicAdd(pointLightCount, 1);
				pointLightIndex[id] = lightIndex;
			}
		}
*/

/*
for (uint lightIndex = gl_LocalInvocationIndex; lightIndex < MAX_LIGHTS; lightIndex += WORK_GROUP_SIZE)
		{
			PointLight p = pointLights[lightIndex];
		
			vec4 center = view * vec4(p.position, 1.0f);

			 if( ( GetSignedDistanceFromPlane( center, frustumEqn[0] ) < p.radius ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[1] ) < p.radius ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[2] ) < p.radius ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[3] ) < p.radius ) )
                {
					uint id = atomicAdd(pointLightCount, 1);
					pointLightIndex[id] = lightIndex;
				}
		}

*/

/*


	int threadsPerTile = WORK_GROUP_SIZE * WORK_GROUP_SIZE;

		for (uint i = 0; i < MAX_LIGHTS; i+= threadsPerTile)
		{
			uint il = gl_LocalInvocationIndex + i;

			if (il < MAX_LIGHTS)
			{
				PointLight p = pointLights[il];
		
				vec4 viewPos = view * vec4(p.position, 1.0f);
				float r = p.radius;

				//if (viewPos.z + minDepthZ < r && viewPos.z - maxDepthZ < r)
				{
					
				 if( ( GetSignedDistanceFromPlane( viewPos, frustumEqn[0] ) < r ) &&
						( GetSignedDistanceFromPlane( viewPos, frustumEqn[1] ) < r ) &&
						( GetSignedDistanceFromPlane( viewPos, frustumEqn[2] ) < r ) &&
						( GetSignedDistanceFromPlane( viewPos, frustumEqn[3] ) < r) )
					{
						uint id = atomicAdd(pointLightCount, 1);
						pointLightIndex[id] = il;
					}
				}

			}
		}

*/

/*

		float minDepthZ = float(minDepth / float(0xFFFFFFFF));
		float maxDepthZ = float(maxDepth / float(0xFFFFFFFF));

		vec4 frustumEqn[4];
		uint pxm = WORK_GROUP_SIZE * gl_WorkGroupID.x;
		uint pym = WORK_GROUP_SIZE * gl_WorkGroupID.y;
		uint pxp = WORK_GROUP_SIZE * (gl_WorkGroupID.x + 1);
		uint pyp = WORK_GROUP_SIZE * (gl_WorkGroupID.y + 1);
				
		uint uWindowWidthEvenlyDivisibleByTileRes = WORK_GROUP_SIZE * GetNumTilesX();
        uint uWindowHeightEvenlyDivisibleByTileRes = WORK_GROUP_SIZE * GetNumTilesY();

		vec4 frustum[4];
		frustum[0] = ConvertProjToView( vec4( pxm / float(uWindowWidthEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 
			(uWindowHeightEvenlyDivisibleByTileRes - pym) / float(uWindowHeightEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 1.0f, 1.0f) );
		
        frustum[1] = ConvertProjToView( vec4( pxp / float(uWindowWidthEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 
			(uWindowHeightEvenlyDivisibleByTileRes - pym) / float(uWindowHeightEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 1.0f, 1.0f) );
		
        frustum[2] = ConvertProjToView( vec4( pxp / float(uWindowWidthEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 
			(uWindowHeightEvenlyDivisibleByTileRes - pyp) / float(uWindowHeightEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 1.0f ,1.0f) );
		
        frustum[3] = ConvertProjToView( vec4( pxm / float(uWindowWidthEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 
			(uWindowHeightEvenlyDivisibleByTileRes - pyp) / float(uWindowHeightEvenlyDivisibleByTileRes) * 2.0f - 1.0f, 1.0f, 1.0f) );


		for (int i = 0; i < 4; i++)
			frustumEqn[i] = CreatePlaneEquation(frustum[i], frustum[(i+1) & 3]);

		barrier();

		int threadsPerTile = WORK_GROUP_SIZE * WORK_GROUP_SIZE;

		for (uint i = 0; i < numActiveLights; i+= threadsPerTile)
		{
			uint il = gl_LocalInvocationIndex + i;

			if (il < numActiveLights)
			{
				PointLight p = pointLights[il];
		
				vec4 viewPos = view * vec4(p.position, 1.0f);
				float r = p.radius_length;

				if (viewPos.z + minDepthZ < r && viewPos.z - maxDepthZ < r)
				{
					
				if( ( GetSignedDistanceFromPlane( viewPos, frustumEqn[0] ) < r ) &&
					( GetSignedDistanceFromPlane( viewPos, frustumEqn[1] ) < r ) &&
					( GetSignedDistanceFromPlane( viewPos, frustumEqn[2] ) < r ) &&
					( GetSignedDistanceFromPlane( viewPos, frustumEqn[3] ) < r) )

					{
						uint id = atomicAdd(pointLightCount, 1);
						pointLightIndex[id] = il;
					}
				}

			}
		}


*/