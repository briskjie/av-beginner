#version 120

attribute vec3 a_Position ;

uniform mat4 MVP;

void main()
{
//    gl_Position = MVP * vec4(a_Position, 1.0);
    gl_Position = vec4(a_Position, 1.0);
}
