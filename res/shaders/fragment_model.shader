
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// struct Material {
//     sampler2D diffuse0;
//     sampler2D specular0;
//     sampler2D normal0;
// }; 
// uniform Material material;

uniform sampler2D texture_diffuse0;
void main()
{    
    // FragColor = texture(material.diffuse0, TexCoords);
    FragColor = texture(texture_diffuse0, TexCoords);
}