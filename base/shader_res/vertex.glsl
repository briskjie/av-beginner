#version 330 core

//in vec3 a_Position ;
//in vec2 texCoord;

layout (location = 0) in vec3 a_Position;
//out vec2 texCoord0;

void main()
{
//    gl_Position = MVP * vec4(a_Position, 1.0);
    gl_Position = vec4(a_Position, 1.0);

//    texCoord0 = texCoord;
}
