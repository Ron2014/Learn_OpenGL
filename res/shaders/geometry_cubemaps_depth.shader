#version 330 core
layout (points) in;
layout (points, max_vertices = 6) out;

out vec4 FragPos;
out vec3 lightPos;

struct PointLightData {
  vec3 position;

  mat4 right;
  mat4 left;
  mat4 top;
  mat4 bottom;
  mat4 front;
  mat4 back;
};

// #define NR_POINT_LIGHTS 4
#define NR_POINT_LIGHTS 1
uniform PointLightData pointLightData[NR_POINT_LIGHTS];

void main() {
    FragPos = gl_in[0].gl_Position;
    
    for(int i=0; i<NR_POINT_LIGHTS; i++) {
      lightPos = pointLightData[i].position;

      gl_Layer = 0; // built-in variable that specifies to which face we render.
      gl_Position = pointLightData[i].right * FragPos;
      EmitVertex();
      
      gl_Layer = 1; // built-in variable that specifies to which face we render.
      gl_Position = pointLightData[i].left * FragPos;
      EmitVertex();
      
      gl_Layer = 2; // built-in variable that specifies to which face we render.
      gl_Position = pointLightData[i].top * FragPos;
      EmitVertex();
      
      gl_Layer = 3; // built-in variable that specifies to which face we render.
      gl_Position = pointLightData[i].bottom * FragPos;
      EmitVertex();
      
      gl_Layer = 4; // built-in variable that specifies to which face we render.
      gl_Position = pointLightData[i].front * FragPos;
      EmitVertex();
      
      gl_Layer = 5; // built-in variable that specifies to which face we render.
      gl_Position = pointLightData[i].back * FragPos;
      EmitVertex();
    }

    EndPrimitive();
}