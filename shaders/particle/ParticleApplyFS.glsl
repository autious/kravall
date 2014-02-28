#version 430

out vec4 color;

in vec2 uv;

uniform sampler2D textureIN;

void main()
{
	color = texture2D(textureIN, uv);
}
