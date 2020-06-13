#version 330 core
out vec4 FragColor;  

uniform vec3 lightColor;
uniform vec3 objColor;

in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(lightColor * objColor, 1.0);
}
