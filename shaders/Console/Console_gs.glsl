#version 430

uniform vec3 pointPosition;
uniform vec3 pointPosition2;
uniform vec4 inColor;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void main()
{

gl_Position = vec4( pointPosition.x + pointPosition2.x, 
	pointPosition.y - pointPosition2.y, 0, 1 );
	EmitVertex();

	gl_Position = vec4( pointPosition.x + pointPosition2.x, 
	pointPosition.y, 0, 1 );
	EmitVertex();

	
		
	gl_Position = vec4( pointPosition.x, 
	pointPosition.y - pointPosition2.y, 0, 1 );
	EmitVertex();	

	gl_Position = vec4( pointPosition.x, 
	pointPosition.y, 0, 1 );
	EmitVertex();


		
	EndPrimitive();
}
