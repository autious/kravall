#version 430

uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;
uniform samplerCube gInteriorMap;

uniform float gMaterialID;
uniform float gGamma;
uniform vec3 gCameraPosition;

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

	//Interior mapping
	vec2 f = fract(uvFS);
	vec2 r = floor(uvFS) * vec2(0.679570, 0.785398) + vec2(0.414214, 0.732051);
	float cubeIndex = fract(r.x + r.y + r.x * r.y) * 8.0;

	// Entrance position into the room
	vec4 pos = vec4(f * vec2(2.0, -2.0) - vec2(1.0, -1.0), -1.0, cubeIndex);
	
	vec3 dir = posW.xyz - vec3(gCameraPosition.x, -gCameraPosition.y, gCameraPosition.z);
	//dir.x = -dir.x;// * 2.2f;
	//dir.y = dir.y * 0.3f;
	//dir.z *= 5;
	// Compute position where the ray intersects the cube
	vec3 id = 1.0 / dir;
	vec3 k = abs(id) - pos.xyz * id;
	float kMin = min(min(k.x, k.y), k.z);
	pos.xyz += kMin * dir.xyz;
	
	vec4 interiorMap = texture(gInteriorMap, vec3(pos.x, pos.y, pos.z));
	interiorMap.xyz = pow(interiorMap.xyz, vec3(gGamma));

	vec4 diffuse = texture2D(gDiffuse, uvFS);
	diffuse.xyz = pow(diffuse.xyz, vec3(gGamma));

	vec4 spec = texture2D(gSpecular, uvFS);
	spec.xyz = pow(spec.xyz, vec3(gGamma));

	vec4 glow = texture2D(gGlow, uvFS);
	glow.xyz = pow(glow.xyz, vec3(gGamma));

	gNormalDepthRT = vec4(finalNormal, posFS.z / posFS.w);
	gDiffuseRT	=  diffuse * interiorMap;
	gSpecularRT = spec;
	gGlowMatRT = vec4(glow.xyz, gMaterialID);
}