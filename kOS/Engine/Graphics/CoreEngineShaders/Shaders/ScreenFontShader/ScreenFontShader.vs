R"(
#version 460 core

layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat3 projection;

uniform mat3 rotate;

uniform vec3 point;


void main()
{
    vec3 pos = vec3(vertex.xy, 1.0);
    pos.xy -= point.xy;
    pos = rotate * pos;
    pos.xy += point.xy;
    gl_Position = vec4(projection * pos, 1.f);
    gl_Position.z = point.z;
    texCoords = vertex.zw;
}
)"