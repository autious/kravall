#version 430

in FragmentData
{
    vec4 Position;
    vec2 UV;
    float life;
}FragmentIn;

layout ( location = 1 ) out vec4 diffuseRT;
layout ( location = 2 ) out vec4 glowMatIDRT;

uniform sampler2D gDepthBuffer;

uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

void main()
{
    vec4 diffuse = texture2D(gDiffuse, FragmentIn.UV);
    vec4 glow = texture2D(gGlow, FragmentIn.UV);

    diffuseRT = diffuse * diffuse.a * vec4(1.0f, 1.0f, 1.0f, min(10.0f, FragmentIn.life) / 10.0f);
    glowMatIDRT = glow;
}
