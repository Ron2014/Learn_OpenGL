#version 330 core
out vec4 FragColor;
uniform vec4 color;
out vec3 Normal;

void main()
{
    FragColor = color;
}