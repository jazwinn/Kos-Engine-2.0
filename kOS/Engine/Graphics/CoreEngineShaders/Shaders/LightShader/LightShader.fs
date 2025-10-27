#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

layout(binding=0) uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
layout(binding=2) uniform samplerCube cubeTexture;

struct Material 
{
    vec3 Ka;            // Ambient reflectivity
    vec3 Kd;            // Diffuse reflectivity
    vec3 Ks;            // Specular reflectivity
    float shininess;    // Specular shininess factors
    float reflectivity;

};

struct Light 
{
    vec3 position;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity
};

struct DirectionalLight 
{
    vec3 direction;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity
};

in vec3 Position;       // In view space
in vec3 Normal;         // In view space
in vec3 ReflectDir;

uniform Light light[1];
uniform DirectionalLight directionalLight[1];

uniform Material material;
uniform mat4 view;  

vec3 BlinnPhong(vec3 position,vec3 normal, Light light, mat4 view)
{
    vec3 color = light.color;

    if (any(notEqual(normal, vec3(0.0f, 0.0f, 0.0f))))
    {
         vec3 ambient =light.La * material.Ka*texture(texture_diffuse1, TexCoords).rgb*light.color;
    // Calculate the ambient component
    vec3 lightPos = vec3(view * vec4(light.position, 1.0));
    normal = normalize(normal);
    // Vector from fragment to light source
    vec3 lightDir = normalize(lightPos - position);
    vec3 viewDir = normalize(-position);
    // Calculate the diffuse component
    float diff = dot(normal, lightDir) > 0.0f ? dot(normal, lightDir) : 0.0f;

    vec3 diffuse = light.Ld * (diff* material.Kd)*texture(texture_diffuse1, TexCoords).rgb*light.color;

    vec3 specular;
    if (diff > 0.f) {
        // Vector from fragment to camera (view position)
        vec3 reflectDir = normalize(lightDir + viewDir);
        // Calculate the specular component
        float spec = 0.0;
        float specAngle = dot(reflectDir, normal) > 0.0f ? dot(normal, reflectDir) : 0.0f;
        spec = pow(specAngle, material.shininess);

        specular = light.Ls * (spec* material.Ks)*texture(texture_diffuse1, TexCoords).rgb*light.color;
    }

    // Combine the results
    vec3 result = ambient + diffuse + specular;

    return result;   
    }
    return color;
}
vec3 DirBlinnPhong(vec3 position,vec3 normal, DirectionalLight light, mat4 view){
    vec3 color = light.color;

    if (any(notEqual(normal, vec3(0.0f, 0.0f, 0.0f))))
    {
         vec3 ambient =light.La * material.Ka*texture(texture_diffuse1, TexCoords).rgb*light.color;
    // Calculate the ambient component
    normal = normalize(normal);
    // Vector from fragment to light source
    vec3 viewDir = normalize(-position);
    // Calculate the diffuse component
    float diff = dot(normal, light.direction) > 0.0f ? dot(normal, light.direction) : 0.0f;

    vec3 diffuse = light.Ld * (diff* material.Kd)*texture(texture_diffuse1, TexCoords).rgb*light.color;

    vec3 specular;
    if (diff > 0.f) {
        // Vector from fragment to camera (view position)
        vec3 reflectDir = normalize(light.direction + viewDir);
        // Calculate the specular component
        float spec = 0.0;
        float specAngle = dot(reflectDir, normal) > 0.0f ? dot(normal, reflectDir) : 0.0f;
        spec = pow(specAngle, material.shininess);

        specular = light.Ls * (spec* material.Ks)*texture(texture_diffuse1, TexCoords).rgb*light.color;
    }

    // Combine the results
    vec3 result = ambient + diffuse + specular;

    return result;   
    }
    return color;
}
void main()
{    
    //Create Reflection
    vec3 envMap = texture(cubeTexture, ReflectDir).rgb;
    
    FragColor = vec4(mix(BlinnPhong(Position, normalize(Normal), light[0],view),envMap,material.reflectivity),1.0);
}