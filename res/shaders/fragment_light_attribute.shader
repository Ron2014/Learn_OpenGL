#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

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

in vec2 TexCoord;
uniform sampler2D ourTexture;

uniform vec3 viewPos;

void main()
{
  vec3 ambient = light.ambient * material.ambient;
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(lightDir, norm), 0.0);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);

  // FragColor = texture(ourTexture, TexCoord) * vec4((ambient+diffuse+specular), 1.0); 
  FragColor = vec4((ambient+diffuse+specular), 1.0);
}
