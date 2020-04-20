#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
    texCoord = a_TexCoord;
}
