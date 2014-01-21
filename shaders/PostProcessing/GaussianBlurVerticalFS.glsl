#version 430

in vec2 uv;

uniform sampler2D gTexture;
uniform vec2 gScreenDimensions;

out vec4 resultColor;

void main()
{
	vec4 sum = vec4(0.0);
	float blurSize = 1.0f / gScreenDimensions.y;

 
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(gTexture, vec2(uv.x, uv.y - 4.0 * blurSize)) * 0.05;
   sum += texture2D(gTexture, vec2(uv.x, uv.y - 3.0 * blurSize)) * 0.09;
   sum += texture2D(gTexture, vec2(uv.x, uv.y - 2.0 * blurSize)) * 0.12;
   sum += texture2D(gTexture, vec2(uv.x, uv.y - blurSize)) * 0.15;
   sum += texture2D(gTexture, vec2(uv.x, uv.y)) * 0.16;
   sum += texture2D(gTexture, vec2(uv.x, uv.y + blurSize)) * 0.15;
   sum += texture2D(gTexture, vec2(uv.x, uv.y + 2.0 * blurSize)) * 0.12;
   sum += texture2D(gTexture, vec2(uv.x, uv.y + 3.0 * blurSize)) * 0.09;
   sum += texture2D(gTexture, vec2(uv.x, uv.y + 4.0 * blurSize)) * 0.05;
 
   resultColor = sum;
}