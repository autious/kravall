#version 430

in vec4 posFS;

out vec4 varianceMap;

void main()
{
	float depth = posFS.z/posFS.w;
	depth += 0.001;
	varianceMap = vec4(depth, depth*depth, 0.0, 1.0);
	//varianceMap = vec4(gl_FragCoord.xy/vec2(1024, 1024),0, 1);
}