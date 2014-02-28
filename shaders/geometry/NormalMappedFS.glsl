#version 430

uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

uniform float gMaterialID;
uniform float gGamma;

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;

layout ( location = 1 ) out vec4 gNormalDepthRT;
layout ( location = 2 ) out vec4 gDiffuseRT;
layout ( location = 3 ) out vec4 gSpecularRT;
layout ( location = 4 ) out vec4 gGlowMatRT;

void main()
{
	vec3 sampledNormal = texture2D(gNormal, uvFS).xyz;
	sampledNormal = 2.0f * sampledNormal - vec3(1.0f);

	vec3 tangent = normalize(tangentFS.xyz);
	vec3 normal = normalize(normalFS.xyz);

	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = -cross(tangent, normal);
	
	vec3 finalNormal;
	mat3 TBN = mat3(tangent, bitangent, normal);
	finalNormal = TBN * sampledNormal;
	finalNormal = normalize(finalNormal);

	vec4 diffuse = texture2D(gDiffuse, uvFS);
	diffuse.xyz = pow(diffuse.xyz, vec3(gGamma));

	vec4 spec = texture2D(gSpecular, uvFS);
	spec.xyz = pow(spec.xyz, vec3(gGamma));

	vec4 glow = texture2D(gGlow, uvFS);
	glow.xyz = pow(glow.xyz, vec3(gGamma));

	gNormalDepthRT = vec4(finalNormal, posFS.z / posFS.w);//vec4(finalNormal, );
	gDiffuseRT	=  diffuse;
	gSpecularRT = spec;
	gGlowMatRT = vec4(glow.xyz, gMaterialID);
}