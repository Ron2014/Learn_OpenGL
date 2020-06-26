#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float scale;

void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(scale*(TexCoord.x-0.5)+0.5, scale*(TexCoord.y-0.5)+0.5)), 0.2);
    // FragColor = mix(texture(texture0, TexCoord), texture(texture1, vec2(scale*TexCoord.x, scale*TexCoord.y)), 0.2);
}