
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

struct Material {
    sampler2D diffuse0;
    sampler2D specular0;
    sampler2D normal0;
    sampler2D height0;
}; 
uniform Material material;

void main()
{    
    FragColor = texture(material.diffuse0, TexCoord);
}