#version 430

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


layout ( location = 0 ) in vec4 positionIN;
layout ( location = 1 ) in vec2 uvIN;

out vec2 uvOUT;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionIN;
    uvOUT = uvIN;
}
