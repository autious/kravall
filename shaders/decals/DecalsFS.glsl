#version 430

in vec4 posFS;
in vec2 uvFS;

uniform sampler2D gNormalDepth;
uniform sampler2D gDiffuse;

uniform float gGamma;

uniform mat4 invProjView;
uniform mat4 invModelMatrix;



vec4 reconstruct_pos(float z, vec2 uv_f)
{
    vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);
    sPos = invProjView * sPos;
    return vec4((sPos.xyz / sPos.w ), sPos.w);
}

layout ( location = 1 ) out vec4 diffuseRT;
layout ( location = 2 ) out vec4 specularRT;
layout ( location = 3 ) out vec4 glowMatIDRT;

void main()
{
	vec4 normal_depth = texture2D(gNormalDepth, uvFS);
	vec4 gBufferPos = reconstruct_pos(normal_depth.w, posFS.xy);

	//Transform the gBufferPos into decals local space
	vec4 decalPos = invModelMatrix * gBufferPos;

	//use the xy position of the position for the texture lookup
	vec2 texcoord = decalPos.xy * 1.0f / textureSize(gDiffuse, 0) * 0.5 + 0.5;
	diffuseRT = texture2D(gDiffuse, texcoord);

}
