#version 430
#extension GL_EXT_gpu_shader4 : enable

in vec2 uv;

const float PI = 3.141592;

//Textures
uniform sampler2D gNormals_depth;
uniform sampler2D gDiffuse;
uniform sampler2D gRadiance;
uniform sampler2D gSeedTexture;

//The radius in world units the samples that come from a unit sphere are scaled to
uniform float gSampleRadius;

//Matrices
uniform mat4 gViewMatrix;
uniform mat4 gProjMatrix;
uniform mat4 gInvProjViewMatrix;

//Occlusion strength
uniform float gStrength;

//Singularity for distance
uniform float gSingularity;

//Depth bias
uniform float gDepthBias;

//Strength of indirect bounce
uniform float gBounceStrength;
uniform float gBounceSingularity;

//Light data
uniform vec4 gLightDirection;
uniform vec4 gLightDiffuse;

//Number of samples per pixel
uniform int gSampleCount;

//The size in number of pixels of the quasi-random pattern used
uniform int gPatternSize;

//Output
vec4 resultColor;

// compute the local frame (currently no check for singularities)
mat3 ComputeTripodMatrix(const vec3 direction, const vec3 up = vec3(0.01, 0.99, 0)) 
{			
	vec3 tangent = normalize(cross(up, direction));
	vec3 cotangent = normalize(cross(direction, tangent));
	return mat3(tangent, cotangent, direction);
}

vec4 ReconustructPosition(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = gInvProjViewMatrix * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}

void main()
{
	//Get position, normal and color of pixel
	vec4 normal_depth = texture2D(gNormals_depth, uv);
	vec4 pixelPosition = ReconustructPosition(normal_depth.w, uv);
	vec3 pixelNormal = normal_depth.xyz;
	vec4 pixelColor = texture2D(gDiffuse, uv);

	if (pixelColor.a > 0.0f)
	{
		//Accumulation over radiance
		vec3 directRadianceSum = vec3(0.0f);
		vec3 occluderRadianceSum = vec3(0.0f);
		vec3 ambientRadianceSum = vec3(0.0f);
		float ambientOcclusion = 0.0f;

		//A matrix that transforms from the unit hemisphere along z =  -1 to the local frame along this normal
		mat3 localMatrix = ComputeTripodMatrix(pixelNormal);

		//The index of the current pattern
		//We use one out of gPatternSize * gPatternSize pre-defined hemisphere patterns
		//The i'th pixel in every sub-rectangle uses always the same i-th sub-pattern
		int patternIndex = int(uv.x) % gPatternSize + (int(uv.y) % gPatternSize) * gPatternSize;

		for (int i = 0; i < gSampleCount; i++)
		{
			//Get the i-th sample direction and transform it to local space
			vec3 localSample = localMatrix * texture2D(gSeedTexture, ivec2(i, patternIndex)).rgb;

			vec3 normalizedSample = normalize(localSample);

			//Go sample-radius steps along the sample direction, starting at the current pixel world space position
			vec4 worldSampleOccluderPosition = pixelPosition + gSampleRadius * vec4(localSample.x, localSample.y, localSample.z, 0.0f);

			//Project this world occluder position in the current eye space using the view-projection matrix (sample uses modelView, HOW?)
			vec4 occluderSamplePosition = (gProjMatrix * gViewMatrix) * worldSampleOccluderPosition;

			//Do a division by w here and map to window coords using GL rules
			vec2 occlusionTexCoord = textureSize2D(gNormals_depth, 0) * (vec2(0.5f) + 0.5f * (occluderSamplePosition.xy / occluderSamplePosition.w));

			//Read the occluder position and the occluder normal at the occluder texcoord
			vec4 occluderNormal_depth = texture2D(gNormals_depth, occlusionTexCoord);
			vec3 occluderNormal = occluderNormal_depth.xyz;
			vec4 occluderPosition = ReconustructPosition(occluderNormal_depth.w, occlusionTexCoord);

			//Remove influence of undefined pixels
			if (length(occluderNormal) == 0)
			{
				occluderPosition = vec4(100000.0, 100000.0, 100000.0, 1.0);
			}
		}
		
	}
	else
	{
		resultColor = vec4(0.0f);
	}
}