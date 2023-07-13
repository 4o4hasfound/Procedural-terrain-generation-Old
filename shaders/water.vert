#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform float height;

void main()
{
    TexCoord=aTexCoord;
    gl_Position= projection * view * vec4(aPos.x, height, aPos.y, 1.0);
}  