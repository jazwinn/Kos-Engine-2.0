R"(
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D lowerBuffer;
uniform sampler2D upperBuffer;

void main()
{
    vec4 sceneColor = texture(lowerBuffer, TexCoords);
    vec4 uiColor    = texture(upperBuffer, TexCoords);

    // Alpha blend UI on top of scene
    FragColor = mix(sceneColor, uiColor, uiColor.a);
}
)"