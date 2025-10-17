R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

struct DirectionalLight 
{
    vec3 direction;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity
    mat4 shadowMtx;
};
uniform DirectionalLight directionalLight[1];

void main()
{
	gl_Position = directionalLight[0].shadowMtx * model * vec4(aPos, 1.0);
}
)"