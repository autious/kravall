#version 430 core

struct PointLight
{
	vec3 position;
	float radius;
	vec3 color;
	float intensity;
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

uniform mat4 inv_proj_view_mat;

layout (local_size_x = 16, local_size_y = 16) in;

vec3 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = inv_proj_view_mat * sPos;
     
    return (sPos.xyz / sPos.w);
}

vec4 CalculateLighting( PointLight p, vec3 wPos, vec3 wNormal, vec4 wSpec, vec4 wGlow)
{
	vec3 direction = p.position - wPos;

	if(length(direction) > p.radius)
		return vec4(0.0f, 0.0f, 0.0f, 0.0f);

	float attenuation = 1.0f - length(direction) / (p.radius);
	direction = normalize(direction);
	float diffuseFactor = max(0.0f, dot(direction, wNormal)) * attenuation;
	return vec4(p.color.xyz, 0.0f) * diffuseFactor * p.intensity;
}

void main()
{
        ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
		

        vec4 normalColor = imageLoad(normalDepth, pixel);
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
		for(int i = 0; i < 1; i++)
		{
			color += CalculateLighting(pointLights[i], wPos.xyz, 2*normalColor.xyz-1.0f, specular, glow) * diffuseColor;
		}

		// ambient
		color += vec4(0.05f, 0.05f, 0.05f, 0.0f) * diffuseColor;
		

        imageStore(outTexture, pixel, color);

}