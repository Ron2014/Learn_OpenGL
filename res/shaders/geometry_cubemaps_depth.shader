#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 75) out;

out vec4 FragPos;

uniform mat4 directLightMatrix;

struct PointLightMatrix {
  mat4 right;
  mat4 left;
  mat4 top;
  mat4 bottom;
  mat4 front;
  mat4 back;
};

#define NR_POINT_LIGHTS 4
uniform PointLightMatrix pointLightMatrix[NR_POINT_LIGHTS];

void main() {
    for(int j=0; j<3; j++) {
      // FragPos = gl_in[j].gl_Position;
      // gl_Position = directLightMatrix * FragPos;

      // for(int k=0; k<6; k++) {
      //   gl_Layer = k; // built-in variable that specifies to which face we render.
      //   EmitVertex();
      // }
      FragPos = gl_in[j].gl_Position;
      gl_Position = directLightMatrix * FragPos;
      EmitVertex();
      
      for(int i=0; i<NR_POINT_LIGHTS; i++) {
        gl_Layer = 0; // built-in variable that specifies to which face we render.
        gl_Position = pointLightMatrix[i].right * FragPos;
        EmitVertex();
        
        gl_Layer = 1; // built-in variable that specifies to which face we render.
        gl_Position = pointLightMatrix[i].left * FragPos;
        EmitVertex();
        
        gl_Layer = 2; // built-in variable that specifies to which face we render.
        gl_Position = pointLightMatrix[i].top * FragPos;
        EmitVertex();
        
        gl_Layer = 3; // built-in variable that specifies to which face we render.
        gl_Position = pointLightMatrix[i].bottom * FragPos;
        EmitVertex();
        
        gl_Layer = 4; // built-in variable that specifies to which face we render.
        gl_Position = pointLightMatrix[i].front * FragPos;
        EmitVertex();
        
        gl_Layer = 5; // built-in variable that specifies to which face we render.
        gl_Position = pointLightMatrix[i].back * FragPos;
        EmitVertex();
      }
    }

    EndPrimitive();
}