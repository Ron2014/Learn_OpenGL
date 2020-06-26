#version 330 core
out vec4 FragColor;  

uniform vec3 lightColor;
uniform vec3 objColor;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoord;
uniform sampler2D ourTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
  float ambient = 0.1f;
  float specularIntension = 2.0f;
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diffuse = max(dot(lightDir, norm), 0.0);

  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularIntension * spec * lightColor;

  FragColor = texture(ourTexture, TexCoord) * vec4((ambient+diffuse+specular) * lightColor * objColor, 1.0);
}
