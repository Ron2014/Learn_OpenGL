#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
} gs_in[];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord; 

uniform float time;

vec3 GetNormal()
{
  
  vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);   // 1->0
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);   // 1->2
  return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
                              // [-1, 1] -> [0, 1]
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    Normal = gs_in[0].Normal;
    FragPos = gs_in[0].FragPos;
    TexCoord = gs_in[0].TexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    Normal = gs_in[1].Normal;
    FragPos = gs_in[1].FragPos;
    TexCoord = gs_in[1].TexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    Normal = gs_in[2].Normal;
    FragPos = gs_in[2].FragPos;
    TexCoord = gs_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}