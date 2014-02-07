#version 430

in vec4 posFS;
in vec4 posW;
in vec4 normalFS;
in vec4 tangentFS;
in vec2 uvFS;

uniform vec4 gOutlineColor;

layout ( location = 1 ) out vec4 gNormalDepthRT;
layout ( location = 2 ) out vec4 gDiffuseRT;
layout ( location = 3 ) out vec4 gSpecularRT;
layout ( location = 4 ) out vec4 gGlowMatRT;

void main()
{
	gNormalDepthRT = vec4(0);//vec4(finalNormal, );
	gDiffuseRT	= gOutlineColor;
	gSpecularRT = vec4(0);
	gGlowMatRT = vec4(1);//vec4(1, 0, 0, 1);
}