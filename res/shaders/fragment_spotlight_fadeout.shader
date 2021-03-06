#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 
in vec2 TexCoord;

struct Light {
  vec3 position;
  vec3 direction;
  float cutoff;
  float cutoff_outter;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

out vec4 FragColor;  

uniform Light light;
uniform Material material;      // == objColor

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

void main()
{
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
  vec3 fromLight = normalize(FragPos - light.position);
  vec3 toLight = -fromLight;

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  
  float intensity = clamp((dot(fromLight, light.direction)-light.cutoff_outter) / (light.cutoff-light.cutoff_outter), 0.0f, 1.0f);

  vec3 norm = normalize(Normal);
  float diff = max(dot(toLight, norm), 0.0f);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));
  diffuse *= intensity;

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(fromLight, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));
  specular *= intensity;

  // vec3 emission = vec3(texture(material.emission, TexCoord));
  // FragColor = vec4(ambient+diffuse+specular+emission, 1.0); 
  
  FragColor = vec4((ambient + diffuse + specular)*attenuation, 1.0); 
}
