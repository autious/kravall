#version 430

in vec2 uv;

uniform sampler2D gTexture;
uniform vec2 gScreenDimensions;
uniform float gBlurFactor;

out vec4 resultColor;

const float kernel[]=float[21] (
								0.000272337,  0.00089296, 0.002583865, 
								0.00659813,  0.014869116, 0.029570767, 
								0.051898313, 0.080381679, 0.109868729, 
								0.132526984, 0.14107424,  0.132526984, 
								0.109868729, 0.080381679, 0.051898313, 
								0.029570767, 0.014869116, 0.00659813, 
								0.002583865, 0.00089296, 0.000272337
								);

void main()
{
	vec2 delta = gBlurFactor/textureSize(gTexture,0);
	vec4 color = vec4(0);
	int  index = 20;

	for(int i=-10;i<=10;i++) {
		color += kernel[index--]*texture(gTexture, uv + (vec2(i*delta.x,0)));
	}

	resultColor =  color;
}