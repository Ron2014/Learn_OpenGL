#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float SCR_WIDTH;
uniform float SCR_HEIGHT;

void main()
{
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    FragColor = vec4(gl_FragCoord.x/SCR_WIDTH, gl_FragCoord.y/SCR_HEIGHT, gl_FragCoord.z, 1.0);
}