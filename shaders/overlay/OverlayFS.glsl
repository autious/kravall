#version 430

uniform sampler2D diffuseMap;

in vec2 uvOUT;

out vec4 color;

void main()
{
    color = texture2D(diffuseMap, uvOUT);
}
