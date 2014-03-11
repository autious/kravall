#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout (shared) uniform PerFrameBlock
{
    mat4 gView;
    mat4 gProjection;
};

in GeometryData
{
    vec4 position;
    float life;
}GeometryIn[];

out FragmentData
{
    vec4 Position;
    vec2 UV;
    float life;
}GeometryOut;

void main()
{
    vec4 viewSpacePointPos = GeometryIn[0].position;
    float scale = 0.0f;

    if(GeometryIn[0].life > 0.0f)
    {
        scale =  1.0f - GeometryIn[0].life;
    }

    gl_Position = GeometryOut.Position = gProjection * (viewSpacePointPos + vec4(vec3(1.0f, 1.0f, 0.0f) * scale, 0.0f));
    GeometryOut.UV = vec2(1.0f, 1.0f);
    GeometryOut.life = GeometryIn[0].life;
    EmitVertex();

    gl_Position = GeometryOut.Position = gProjection * (viewSpacePointPos + vec4(vec3(-1.0f, 1.0f, 0.0f) * scale, 0.0f));
    GeometryOut.UV = vec2(0.0f, 1.0f);
    GeometryOut.life = GeometryIn[0].life;
    EmitVertex();

    gl_Position = GeometryOut.Position = gProjection * (viewSpacePointPos + vec4(vec3(1.0f, -1.0f, 0.0f) * scale, 0.0f));
    GeometryOut.UV = vec2(1.0f, 0.0f);
    GeometryOut.life = GeometryIn[0].life;
    EmitVertex();

    gl_Position = GeometryOut.Position = gProjection * (viewSpacePointPos + vec4(vec3(-1.0f, -1.0f, 0.0f) * scale, 0.0f));
    GeometryOut.UV = vec2(0.0f, 0.0f);
    GeometryOut.life = GeometryIn[0].life;
    EmitVertex();

    EndPrimitive();
}
