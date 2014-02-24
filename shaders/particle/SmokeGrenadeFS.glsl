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
    vec4 glow = texture2D(gGlow, FragmentIn.UV);

    float depth = FragmentIn.Position.z / FragmentIn.Position.w;
    float sampleDepth = texture2D(gDepthBuffer, gl_FragCoord.xy / depthBufferSize ).w;
    
    //Something is borked with depth, seems to be dependant on viewspace
    //float fade = clamp((sampleDepth - depth) * 0.2f, 0.0f, 1.0f);
    
    diffuseRT.xyz = pow(diffuse.xyz, vec3(gGamma)) * (min(5.0f, (FragmentIn.life-0.15f) ) / 5.0f) * diffuse.a;
    diffuseRT.a = min(diffuse.a, (min(5.0f, (FragmentIn.life-0.15f) ) / 5.0f));
    //diffuseRT = diffuse * diffuse.a;
    //diffuseRT.a = diffuse.a * vec4(1.0f, 1.0f, 1.0f, min(5.0f, FragmentIn.life) / 5.0f); // * fade; 
    //glowMatIDRT = glow;
}
