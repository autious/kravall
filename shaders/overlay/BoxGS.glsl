#version 430


uniform vec4 point;
uniform vec4 color;

layout(points) in;
layout(line_strip, max_vertices = 5) out;

void main()
{
	gl_Position = vec4(point.x, point.y, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(point.x + point.z, point.y, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(point.x + point.z, point.y - point.w, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(point.x, point.y - point.w, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(point.x, point.y, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
}
