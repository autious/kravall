#version 430
 
uniform float roll;
 
//uniform writeonly image2D outTexture;
//uniform sampler2D diffuse;
//uniform sampler2D normal;
//uniform sampler2D specular;
//uniform sampler2D glowMatID;

layout (binding = 0, rgba32f) uniform writeonly image2D outTexture;
layout (binding = 1, rgba32f) uniform readonly image2D normalDepth;
layout (binding = 2, rgba32f) uniform readonly image2D diffuse;
layout (binding = 3, rgba32f) uniform readonly image2D specular;
layout (binding = 4, rgba32f) uniform readonly image2D glowMatID;

layout (std430, binding = 5) buffer BufferObject
{
    vec4 points[];
};
 
layout (local_size_x = 16, local_size_y = 16) in;

vec3 hardCodedDirectionalLightDirection = vec3(1,1,-1);

void main()
{
        ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
		
        vec4 normalColor = imageLoad(normalDepth, pixel);
		vec4 diffuseColor = imageLoad(diffuse, pixel);
		vec4 specular = imageLoad(specular, pixel);
		vec4 glow = imageLoad(glowMatID, pixel);

		float diffuseFactor = max(0, dot(normalize(hardCodedDirectionalLightDirection), normalize(normalColor.xyz)));

		vec4 color = diffuseFactor * points[gl_GlobalInvocationID.x%100];

        imageStore(outTexture, pixel, color);

}