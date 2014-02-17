#version 430

in FragmentData
{
    vec4 Position;
    vec2 UV;
    float life;
}FragmentIn;

layout ( location = 1 ) out vec4 normalDepthRT;
layout ( location = 2 ) out vec4 diffuseRT;
layout ( location = 3 ) out vec4 specularRT;
layout ( location = 4 ) out vec4 glowMatIDRT;

uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gSpecular;
uniform sampler2D gGlow;

void main()
{
    vec4 diffuse = texture2D(gDiffuse, FragmentIn.UV);
    vec4 glow = texture2D(gGlow, FragmentIn.UV);

    normalDepthRT = vec4(vec3(0.0f, 0.0f, 1.0f), FragmentIn.Position.z / FragmentIn.Position.w);
    diffuseRT = diffuse * vec4(1.0f, 1.0f, 1.0f, min(1.0f, FragmentIn.life));
    glowMatIDRT = glow;
}
