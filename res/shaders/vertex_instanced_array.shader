#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 ourColor;
uniform float time;

void main()
{
    // vec2 pos = aPos * (sin(gl_InstanceID*3.1415926*0.06125) / 1.0);
    // vec2 pos = aPos * (gl_InstanceID / 100.0);

    /*
    int x = gl_InstanceID % 10;
    int y = gl_InstanceID / 10;
    float centerx = 4.5;
    float centery = 4.5;
    vec2 pos = vec2(
        aPos.x * (centerx-abs(x-centerx))/(float(centerx)),
        aPos.y * (centery-abs(y-centery))/(float(centery))
        );
    */
    
    // matrix
    float floating = 0.003f;
    int x = gl_InstanceID % 10;
    int y = gl_InstanceID / 10;
    float centerx = 4.5;
    float centery = 4.5;
    vec2 pos = vec2(
        aPos.x * (abs(x-centerx))/(float(centerx)) + sin(time) * floating,
        aPos.y * (abs(y-centery))/(float(centery)) + cos(time) * floating
        );

    gl_Position = vec4(pos + aOffset, 0.0, 1.0);
    ourColor = aColor;
}