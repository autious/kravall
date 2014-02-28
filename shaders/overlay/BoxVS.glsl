#version 430

uniform vec4 point;

void main()
{
	gl_Position = vec4(point.xy, 0.0f, 1.0f);
}