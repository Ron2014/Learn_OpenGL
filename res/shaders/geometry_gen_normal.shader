#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} gs_in[];

out GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} gs_out;

uniform mat4 model;

vec3 GetNormal() { 
//   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);   // 1->0
//   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);   // 1->2
  vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);   // 0->1
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);   // 1->2
  return normalize(cross(a, b));
}

void GenVertex(int i, vec3 normal) {
    gl_Position = gl_in[i].gl_Position;
    gs_out.Normal = normal;
    gs_out.FragPos = gs_in[i].FragPos;
    gs_out.TexCoord = gs_in[i].TexCoord;
    gs_out.FragPosLightSpace = gs_in[i].FragPosLightSpace;
    EmitVertex();
}

void main()
{
    vec3 normal = mat3(transpose(inverse(model))) * GetNormal();
    
    GenVertex(0, normal); // 第一个顶点法线
    GenVertex(1, normal); // 第二个顶点法线
    GenVertex(2, normal); // 第三个顶点法线
}