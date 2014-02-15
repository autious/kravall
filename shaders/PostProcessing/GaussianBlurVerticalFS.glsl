//#version 430

//in vec2 uv;

//uniform sampler2D gTexture;
//uniform vec2 gScreenDimensions;

//out vec4 resultColor;

//void main()
//{
//	vec4 sum = vec4(0.0);
//	float blurSize = 1.0f / gScreenDimensions.y;

 
//   // blur in y (vertical)
//   // take nine samples, with the distance blurSize between them
//   sum += texture2D(gTexture, vec2(uv.x, uv.y - 4.0 * blurSize)) * 0.05;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y - 3.0 * blurSize)) * 0.09;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y - 2.0 * blurSize)) * 0.12;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y - blurSize)) * 0.15;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y)) * 0.16;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y + blurSize)) * 0.15;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y + 2.0 * blurSize)) * 0.12;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y + 3.0 * blurSize)) * 0.09;
//   sum += texture2D(gTexture, vec2(uv.x, uv.y + 4.0 * blurSize)) * 0.05;
 
//   resultColor = sum;
//}

#version 430

in vec2 uv;

uniform sampler2D gTexture;
uniform vec2 gScreenDimensions;

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
	vec2 delta = 0.3/textureSize(gTexture,0);
	vec4 color = vec4(0);
	int  index = 20;

	for(int i=-10;i<=10;i++) {
		color += kernel[index--]*texture(gTexture, uv + (vec2(0, i*delta.y)));
	}

	resultColor =  color;
}