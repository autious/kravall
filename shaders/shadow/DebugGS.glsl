#version 430

out vec4 posFS;

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

void main()
{
	
	gl_Position = vec4( -100, 100, -10, 1 );
	EmitVertex();
	
	
	gl_Position = vec4( 100, -100, -10, 1 );
	EmitVertex();
	gl_Position = vec4( 100, 100, -10, 1 );
	EmitVertex();
	
	gl_Position = vec4(-100, -100, -10, 1 );
	EmitVertex();

	

	EndPrimitive();
}



