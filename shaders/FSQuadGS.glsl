#version 430

out vec2 uv;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

void main()
{
	gl_Position = vec4( 1.0, 1.0, 0.0, 1.0 );
    uv = vec2( 1.0, 1.0 );
    EmitVertex();

    gl_Position = vec4(-1.0, 1.0, 0.0, 1.0 );
    uv = vec2( 0.0, 1.0 ); 
    EmitVertex();

    gl_Position = vec4( 1.0,-1.0, 0.0, 1.0 );
    uv = vec2( 1.0, 0.0 ); 
    EmitVertex();

    gl_Position = vec4(-1.0,-1.0, 0.0, 1.0 );
    uv = vec2( 0.0, 0.0 ); 
    EmitVertex();

    EndPrimitive(); 
}


