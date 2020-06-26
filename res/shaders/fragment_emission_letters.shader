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
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec4 FragColor;  

// uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform Light light;

// uniform vec3 objColor;
uniform Material material;      // == objColor

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

void main()
{
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(lightDir, norm), 0.0);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

  vec3 emission = vec3(texture(material.emission, TexCoord));
  
  FragColor = vec4(ambient+diffuse+specular+emission, 1.0); 
}
