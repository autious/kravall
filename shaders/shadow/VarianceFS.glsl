#version 430

in vec4 posFS;

out vec4 varianceMap;

void main()
{
	/*
	float depth = posFS.z/posFS.w;
	depth = depth * 0.5 + 0.5;
	depth += 0.0001;
	varianceMap = vec4(depth, depth*depth, 0.0, 1.0);*/
	//varianceMap = vec4(gl_FragCoord.xy/vec2(1024, 1024),0, 1);

	
	float depth = posFS.z/posFS.w;//gl_FragCoord.z;
	depth = depth * 0.5 + 0.5;
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	varianceMap = vec4(depth, depth*depth + 0.25 * (dx*dx + dy*dy), 0.0, 1.0);

}