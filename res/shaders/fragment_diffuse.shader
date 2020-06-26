#version 330 core
out vec4 FragColor;  

uniform vec3 lightColor;
uniform vec3 objColor;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoord;
uniform sampler2D ourTexture;

uniform vec3 lightPos;

void main()
{
  float ambient = 0.05f;
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diffuse = max(dot(lightDir, norm), 0.0);

  FragColor = texture(ourTexture, TexCoord) * vec4((ambient+diffuse) * lightColor * objColor, 1.0);
}
