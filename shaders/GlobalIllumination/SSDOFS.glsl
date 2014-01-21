#version 430
#extension GL_EXT_gpu_shader4 : enable

in vec2 uv;

const float PI = 3.141592;

uniform sampler2D gDiffuse;
uniform sampler2D gNormals_depth;
uniform sampler2D gRadianceTexture;
uniform sampler2D gSeedTexture;

uniform float gSampleRadius;

uniform mat4 gViewMatrix;
uniform mat4 gProjMatrix;
uniform mat4 gInvViewProjMatrix;

uniform float gStrength;
uniform float gSingularity;
uniform float gDepthBias;
uniform float gBounceStrength;
uniform float gBounceSingularity;

uniform sampler2D gEnvMapTexture;

uniform float gSampleCount;

uniform float gPatternSize;
uniform float gLightRotationAngle;

out vec4 resultColor;

vec4 ReconstructPosition(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = gInvViewProjMatrix * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}

// compute the local frame (currently no check for singularities)
mat3 ComputeTripodMatrix(const vec3 direction, const vec3 up = vec3(0.01, 0.99, 0)) 
{			
	vec3 tangent = normalize(cross(up, direction));
	vec3 cotangent = normalize(cross(direction, tangent));
	return mat3(tangent, cotangent, direction);
}

void main()
{
	vec4 normal_depth = texture2D(gNormals_depth, uv);
	vec4 position = ReconstructPosition(normal_depth.w, uv);
	vec4 color = texture2D(gDiffuse, uv);

	if (color.a > 0.0f)
	{
		vec3 directRadianceSum = vec3(0.0);
		vec3 occluderRadianceSum = vec3(0.0);
		vec3 ambientRadianceSum = vec3(0.0);
		float ambientOcclusion = 0.0;

		mat3 localMatrix = ComputeTripodMatrix(normal_depth.xyz);

		int patternIndex = int(uv.x) % int(gPatternSize) + (int(uv.y) % int(gPatternSize)) * int(gPatternSize);

		for (int i = 0; i < gSampleCount; i++)
		{
			vec3 localSample = localMatrix * texture2D(gSeedTexture, ivec2(i, patternIndex)).xyz;
			vec3 normalizedSample = normalize(localSample);

			vec4 worldSampleOccluderPosition = position + gSampleRadius * vec4(localSample.x, localSample.y, localSample.z, 0.0f);
			vec4 occluderSamplePosition = gProjMatrix * gViewMatrix * worldSampleOccluderPosition;
			vec2 occlusionUV = textureSize2D(gNormals_depth, 0) * (vec2(0.5f) + 0.5f * (occluderSamplePosition.xy / occluderSamplePosition.w));

			vec4 occluderNormal = texture2D(gNormals_depth, occlusionUV);
			vec4 occluderPosition = ReconstructPosition(occluderNormal.w, occlusionUV);

			if (length(occluderNormal.xyz) == 0)
			{
				occluderPosition = vec4(100000.0, 100000.0, 100000.0, 1.0);
			}

			float depth = gViewMatrix * worldSampleOccluderPosition.z;
			float sampleDepth = (gViewMatrix * occluderPosition).z + gDepthBias;

			float distanceTerm = abs(depth - sampleDepth) < gSingularity ? 1.0f : 0.0f;
			float visibility = 1.0f - gStrength * (sampleDepth > depth ? 1.0f : 0.0f) * distanceTerm;

			float receiverGeometricTerm = max(0.0f, dot(normalizedSample, normal_depth.xyz));

			float theta = acos(normalizedSample.y);
			float phi = atan(normalizedSample.z, normalizedSample.x);
			
			phi += gLightRotationAngle;
			
			if (phi < 0)
				phi += 2 * PI;
			if (phi > 2 * PI)
				phi -= 2 * PI;

			//WE DONT HAVE AN ENVIRONMENT MAP, WAT DO?
			//vec3 senderRadiance = texture2D(envmapTexture, vec2( phi / (2.0*PI), 1.0 - theta / PI ) ).rgb;

			vec3 radiance = vec3(0.1f) * visibility * receiverGeometricTerm;
			directRadianceSum += radiance;
			
			vec3 ambientRadiance = vec3(1.0f) * receiverGeometricTerm;
			ambientRadianceSum += ambientRadiance;
			ambientOcclusion += visibility;

			vec3 directRadiance = texture2D(gRadianceTexture, occlusionUV).xyz;

			vec3 delta = position.xyz - occluderPosition.xyz;
			vec3 normalizedDelta = normalize(delta);

			float unclampedBounceGeometricTerm =
				max(0.0f, dot(normalizedDelta, -normal_depth.xyz)) *
				max(0.0f, dot(normalizedDelta, occluderNormal.xyz)) /
				max(dot(delta, delta), gBounceSingularity);
			
			float bounceGeometricTerm = min (unclampedBounceGeometricTerm, gBounceSingularity);

			vec3 bounceRadiance = gBounceStrength * directRadiance * bounceGeometricTerm;
			vec3 occluderRadiance = bounceRadiance;
			
			occluderRadianceSum += occluderRadiance;			
		}

		directRadianceSum = max(vec3(0), directRadianceSum);
		occluderRadianceSum = max(vec3(0), occluderRadianceSum);

		// Add direct and indirect radiance
		vec3 radianceSum = directRadianceSum + occluderRadianceSum;
		
		// Multiply by solid angle for one sample
		radianceSum *= 2.0 * PI / gSampleCount;
				
		// Store final radiance value in the framebuffer		

		resultColor.rgb = radianceSum;		
		resultColor.a = 1.0;
	}
	else
	{
		resultColor = vec4(0.0f);
	}
}
