#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

layout(location=0)  uniform sampler2D gPosition;
layout(location=1)  uniform sampler2D gNormal;
layout(location=2)  uniform sampler2D gAlbedoSpec;
layout(location=3)  uniform sampler2D gReflect;  // Tangent-space light direction
layout(binding=5)   uniform samplerCube cubeTexture;


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
    float linear;
    float quadratic;
    float radius;
};
struct SpotLight 
{
    vec3 position;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity
    float linear;
    float quadratic;
    float radius;
    float cutOff;
    float outerCutOff;
    vec3 direction;      // Position of the light source in the world space

};

struct DirectionalLight 
{
    vec3 direction;      // Position of the light source in the world space
    vec3 color;
    vec3 La;            // Ambient light intensity
    vec3 Ld;            // Diffuse light intensity
    vec3 Ls;            // Specular light intensity

};

uniform int pointLightNo;
uniform int dirLightNo;
uniform int spotLightNo;

uniform Light light[32];
uniform DirectionalLight directionalLight[10];
uniform SpotLight spotLight[32];
uniform vec3 lightAmbience;

uniform Material material;
uniform mat4 view;  
vec3 diffuseColor;
float specularColor;


//Lighting

vec3 BlinnPhong(vec3 position,vec3 normal, Light light, mat4 view)
{
    vec3 color = light.color;
    //Check if light volume is in scene

        vec3 lightPos = vec3(view * vec4(light.position, 1.0));
    float distance = length(lightPos - position);

    if (distance < light.radius||any(notEqual(normal, vec3(0.0f, 0.0f, 0.0f))))
    {
         vec3 ambient =light.La * material.Ka*diffuseColor*light.color;
    // Calculate the ambient component

    normal = normalize(normal);
    // Vector from fragment to light source
    vec3 lightDir = normalize(lightPos - position);
    vec3 viewDir = normalize(-position);
    // Calculate the diffuse component
    float diff = dot(normal, lightDir) > 0.0f ? dot(normal, lightDir) : 0.0f;

    vec3 diffuse = light.Ld * (diff* material.Kd)*diffuseColor.rgb*light.color;

    vec3 specular;
    if (diff > 0.f) {
        // Vector from fragment to camera (view position)
        vec3 reflectDir = normalize(lightDir + viewDir);
        // Calculate the specular component
        float spec = 0.0;
        float specAngle = dot(reflectDir, normal) > 0.0f ? dot(normal, reflectDir) : 0.0f;
        spec = pow(specAngle, material.shininess);

        specular = light.Ls * (spec* material.Ks)*specularColor*light.color;

        //Spot light calculation
        float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
    }


    // Combine the results
    vec3 result = diffuse + specular;

    return result;   
    }
    return  vec3(0.0f);
}


vec3 SpotLightBlinnPhong(vec3 position,vec3 normal, SpotLight light, mat4 view)
{
    vec3 color = light.color;

    //Check if light volume is in scene

    vec3 lightPos = vec3(view * vec4(light.position, 1.0));
    vec3 dir = normalize(mat3(view) * light.direction);

    float distance = length(lightPos - position);

    if (distance < light.radius||any(notEqual(normal, vec3(0.0f, 0.0f, 0.0f))))
    {
         vec3 ambient =light.La * material.Ka*diffuseColor*light.color;
    // Calculate the ambient component

    normal = normalize(normal);
    // Vector from fragment to light source
    vec3 lightDir = normalize(lightPos - position);
    vec3 viewDir = normalize(-position);
    // Calculate the diffuse component
    float diff = dot(normal, lightDir) > 0.0f ? dot(normal, lightDir) : 0.0f;

    vec3 diffuse = light.Ld * (diff* material.Kd)*diffuseColor.rgb*light.color;

    vec3 specular;
    if (diff > 0.f) {
        // Vector from fragment to camera (view position)
        vec3 reflectDir = normalize(lightDir + viewDir);
        // Calculate the specular component
        float spec = 0.0;
        float specAngle = dot(reflectDir, normal) > 0.0f ? dot(normal, reflectDir) : 0.0f;
        spec = pow(specAngle, material.shininess);

        specular = light.Ls * (spec* material.Ks)*specularColor*light.color;

        float theta = dot(lightDir, dir); 
        float epsilon = (light.cutOff - light.outerCutOff);
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
        //Spot light calculation
        float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
    }


    // Combine the results
    vec3 result = diffuse + specular;

    return result;   
    }
    return  vec3(0.0f);
}

vec3 DirBlinnPhong(vec3 position,vec3 normal, DirectionalLight light, mat4 view){
    vec3 color = light.color;
    if (any(notEqual(normal, vec3(0.0f, 0.0f, 0.0f))))
    {
         vec3 ambient =light.La * material.Ka*diffuseColor*light.color;
    // Calculate the ambient component

    normal = normalize(normal);
    // Vector from fragment to light source
    vec3 viewDir = normalize(-position);
    // Calculate the diffuse component
    float diff = dot(normal, light.direction) > 0.0f ? dot(normal, light.direction) : 0.0f;

    vec3 diffuse = light.Ld * (diff* material.Kd)*diffuseColor*light.color;

    vec3 specular;
    if (diff > 0.f) {
        // Vector from fragment to camera (view position)
        vec3 reflectDir = normalize(light.direction + viewDir);
        // Calculate the specular component
        float spec = 0.0;
        float specAngle = dot(reflectDir, normal) > 0.0f ? dot(normal, reflectDir) : 0.0f;
        spec = pow(specAngle, material.shininess);

        specular = light.Ls * (spec* material.Ks)*specularColor*light.color;
    }

    // Combine the results
    vec3 result = diffuse + specular;

    return result;   
    }
    return  vec3(0.0f);
}
void main()
{   
    
     vec4 testColor = texture(gAlbedoSpec, TexCoords);
    if (testColor.x==0&&testColor.y==0.f&&testColor.z==0.f&&testColor.a==0.f){
            discard;;   
    }
    diffuseColor =testColor.rgb;
    specularColor =testColor.a;
    //Create Reflection
    vec3 envMap = texture(cubeTexture, vec3(texture(gReflect, TexCoords)) ).rgb;
    
    vec3 newLight=diffuseColor*lightAmbience;

    for(int i=0;i<pointLightNo;i++){
        newLight+=BlinnPhong(vec3(texture(gPosition, TexCoords)),  vec3(texture(gNormal, TexCoords)),
        light[i],
        view);
    }
    for(int i=0;i<spotLightNo;i++){
        newLight+=SpotLightBlinnPhong(vec3(texture(gPosition, TexCoords)),  vec3(texture(gNormal, TexCoords)), spotLight[i],view);
    }
    for(int i=0;i<dirLightNo;i++){
        newLight+=DirBlinnPhong(vec3(texture(gPosition, TexCoords)),  vec3(texture(gNormal, TexCoords)), directionalLight[i],view);
    }
    FragColor = vec4(mix(newLight,envMap,material.reflectivity),1.0);

    //Use re
}