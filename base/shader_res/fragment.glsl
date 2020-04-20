#version 330 core

uniform sampler2D texture1;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
//    FragColor = vec4(0.0,1.0,0.0,1.0);
    FragColor = texture(texture1,texCoord);

}