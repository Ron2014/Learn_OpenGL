#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Back Facing描边
    // 在不光滑物体存在破边问题
    vec3 pos = aPos + normalize(aNormal)*0.01f;   // 沿法线移动1个单位(就放大了嘛)
    gl_Position = projection * view * model * vec4(pos, 1.0);
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // 顶点有矩阵变换，其法向量应该有对应变换才对，但是只能让旋转矩阵影响
    TexCoord = aTexCoord;
}