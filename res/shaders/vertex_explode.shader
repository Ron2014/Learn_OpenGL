#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} gs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    // Normal = vec3(model * vec4(aNormal, 1.0));      // 顶点有矩阵变换，其法向量应该有对应变换才对，但是只能让旋转矩阵影响
    gs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    gs_out.TexCoord = aTexCoord;
}