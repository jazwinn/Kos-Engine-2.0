#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;

out vec3 ReflectDir;

void main()
{
    ReflectDir=reflect( normalize(cameraPosition - aPos), normalize(aNormal));
    mat4 MV = view * model; // Model-View transform matrix
    mat3 N = mat3(vec3(MV[0]), vec3(MV[1]), vec3(MV[2])); // Normal transform matrix
    Normal = normalize(N * aNormal);

    vec4 VertexPositionInView = MV * vec4(aPos, 1.0f);
    Position = VertexPositionInView.xyz;
    gl_Position = projection * VertexPositionInView; 

    TexCoords = aTexCoords;    
}