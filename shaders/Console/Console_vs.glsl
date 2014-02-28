#version 430

uniform vec3 pointPosition;

void main()
{
	gl_Position = vec4(pointPosition, 1.0f);
}