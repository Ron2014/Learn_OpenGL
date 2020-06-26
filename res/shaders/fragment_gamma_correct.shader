#version 330 core
out vec4 FragColor;  

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform bool blinn;
uniform bool gamma;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos) {
  float distance = length(light.position - FragPos);
  float attenuation;
  if (gamma) 
    attenuation = 1.0 / (light.constant + light.quadratic * distance * distance);
  else
    attenuation = 1.0 / (light.constant + light.linear * distance);

  vec3 ambient = light.ambient;
  
  vec3 fromLight = normalize(FragPos-light.position);
  vec3 toLight = -fromLight;
  float diff = max(dot(toLight, normal), 0.0f);
  vec3 diffuse = light.diffuse * diff;

  float spec;
  if (blinn) {
    vec3 halfwayDir = normalize(viewDir + toLight);
    spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
  } else {
    vec3 reflectDir = reflect(fromLight, normal);
    spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8.0f);
  }
  vec3 specular = light.specular * spec;

  return (ambient + diffuse + specular) * attenuation;
}

void main()
{
  vec3 texColor = texture(ourTexture, TexCoord).rgb;
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 lighting = vec3(0.0f);
  for(int i = 0; i < NR_POINT_LIGHTS; i++)
      lighting += CalcPointLight(pointLights[i], norm, viewDir, FragPos);
  texColor *= lighting;
  if (gamma) texColor = pow(texColor, vec3(1.0f/2.2f));
  FragColor = vec4(texColor, 1.0);
}
