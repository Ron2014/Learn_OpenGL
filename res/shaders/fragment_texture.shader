#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;

void main()
{
    // vec4 texColor = texture(texture0, TexCoord);
    // if(texColor.a < 0.1)
    //     discard;
    // FragColor = texColor;

    FragColor = texture(texture0, TexCoord);
/*
    vec4 texColor = texture(texture0, TexCoord);
    if(texColor.a < 0.01)
        discard;
    float a = (1-texColor.a)*(1-texColor.a) / (1-texColor.a+gl_FragCoord.z);
    float b = gl_FragCoord.z*texColor.a / (1-texColor.a+gl_FragCoord.z);
    gl_FragDepth = a + b;
    FragColor = texColor;
*/

}