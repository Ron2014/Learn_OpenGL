#version 330 core
in vec4 FragPos;

struct DirectLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform DirectLight directLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform float far_plane;

float CalcPointLight(PointLight light) {
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - light.position);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    return lightDistance;
}

void main()
{
    // write this as modified depth
    float value = 0.0f; 
    for (int i=0; i<NR_POINT_LIGHTS; i++){
        value += CalcPointLight(pointLights[i]);
    }
    gl_FragDepth = value / NR_POINT_LIGHTS;
}