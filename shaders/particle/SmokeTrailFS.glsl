#version 430

in FragmentData
{
    vec4 Position;
    vec2 UV;
    float life;
}FragmentIn;

layout ( location = 1 ) out vec4 diffuseRT;
layout ( location = 2 ) out vec4 glowMatIDRT;

uniform float gGamma;
uniform vec2 depthBufferSize;
uniform sampler2D gDepthBuffer;

uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

void main()
{
    vec4 diffuse = texture2D(gDiffuse, FragmentIn.UV);
    vec4 normal = texture2D(gNormal, FragmentIn.UV);
    vec4 glow = texture2D(gGlow, FragmentIn.UV);

    float depth = FragmentIn.Position.z / FragmentIn.Position.w;
    float sampleDepth = texture2D(gDepthBuffer, gl_FragCoord.xy / depthBufferSize ).w;
    diffuseRT.xyz = min(vec3(1.0f, 1.0f, 1.0f), pow(diffuse.xyz, vec3(gGamma))) * (min(1.5f, (FragmentIn.life-0.15f) ) / 1.5f) * diffuse.a;
    diffuseRT.a = (min(1.5f, (FragmentIn.life-0.15f) ) / 1.5f) * diffuse.a;
}
