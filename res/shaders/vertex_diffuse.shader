#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Normal = vec3(model * vec4(aNormal, 1.0));      // 顶点有矩阵变换，其法向量应该有对应变换才对，但是只能让旋转矩阵影响
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
}