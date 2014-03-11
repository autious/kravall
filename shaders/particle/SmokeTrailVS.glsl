#version 430

layout (shared) uniform PerFrameBlock
{
    mat4 gView;
    mat4 gProjection;
};

layout (location = 0) in vec3 position;
layout (location = 1) in float life;

out GeometryData
{
    vec4 position;
    float life;
}VertexOut;

void main()
{
    VertexOut.position = gView * vec4(position, 1.0f);
    VertexOut.life = life;
    gl_Position = VertexOut.position;
}
