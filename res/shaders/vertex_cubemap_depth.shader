#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 6)   in mat4 pointLightMatrix0;
layout (location = 7)   in mat4 pointLightMatrix1;
layout (location = 8)   in mat4 pointLightMatrix2;
layout (location = 9)   in mat4 pointLightMatrix3;
layout (location = 10)  in mat4 pointLightMatrix4;
layout (location = 11)  in mat4 pointLightMatrix5;

out VS_OUT {
    mat4 pointLightMatrix[6];
} vs_out;

uniform mat4 model;

void main()
{
    vs_out.pointLightMatrix[0] = pointLightMatrix0;
    vs_out.pointLightMatrix[1] = pointLightMatrix1;
    vs_out.pointLightMatrix[2] = pointLightMatrix2;
    vs_out.pointLightMatrix[3] = pointLightMatrix3;
    vs_out.pointLightMatrix[4] = pointLightMatrix4;
    vs_out.pointLightMatrix[5] = pointLightMatrix5;
    
    gl_Position = model * vec4(aPos, 1.0);
}