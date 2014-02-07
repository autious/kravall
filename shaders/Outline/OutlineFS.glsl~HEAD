#version 430

in vec4 posFS;
in vec4 outlineColor;

layout ( location = 1 ) out vec4 gNormalDepthRT;
layout ( location = 2 ) out vec4 gDiffuseRT;
layout ( location = 3 ) out vec4 gSpecularRT;
layout ( location = 4 ) out vec4 gGlowMatRT;

void main()
{
	gNormalDepthRT = vec4(0);//vec4(finalNormal, );
	gDiffuseRT	= vec4(0);
	gSpecularRT = vec4(0);
	gGlowMatRT = outlineColor;//vec4(0);//outlineColor;
}