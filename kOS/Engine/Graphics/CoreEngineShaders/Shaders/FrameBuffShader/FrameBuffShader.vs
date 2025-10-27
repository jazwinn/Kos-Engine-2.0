/*
 FILENAME: FrameBuffShader.vs
 AUTHOR(S): Gabe (100%)
 @version 460 core
 All content � 2024 DigiPen Institute of Technology Singapore. All
 rights reserved.
 */

R"(
#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform vec4 color2;
out vec4 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(aPos.x, aPos.y,0.9, 1.0); 
    ourColor=color2;
    TexCoords = aTexCoords;
}  
)"