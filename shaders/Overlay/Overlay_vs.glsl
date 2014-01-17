#version 430

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

layout ( location = 0 ) in vec4 positionIN;
layout ( location = 1 ) in vec4 normalIN;
layout ( location = 2 ) in vec4 tangentIN;
layout ( location = 3 ) in vec4 boneIndexIN;
layout ( location = 4 ) in vec4 boneWeightsIN;
layout ( location = 5 ) in vec2 uvIN;

out vec2 uvOUT;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionIN;
    uvOUT = uvIN;
}
