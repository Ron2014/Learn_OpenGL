#version 330 core
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

struct SpotLight {
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

struct Material {
    sampler2D diffuse0;
    sampler2D specular0;
    sampler2D normal0;
    sampler2D height0;
    float shininess;
}; 

// normal 用来计算漫反射
// viewDir 用来计算高光
vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
// 拥有衰减属性的光, 需要片段坐标计算距离和角度
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos);

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;      // == objColor

uniform DirectLight directLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

void main() {
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirectLight(directLight, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, viewDir, FragPos);
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, viewDir, FragPos);    

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir) {
  vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoord));
  
  vec3 fromLight = light.direction;
  vec3 toLight = -fromLight;
  float diff = max(dot(toLight, normal), 0.0);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse0, TexCoord)));

  vec3 reflectDir = reflect(fromLight, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular0, TexCoord)));

  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos) {
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoord));
  
  vec3 fromLight = normalize(FragPos-light.position);
  vec3 toLight = -fromLight;
  float diff = max(dot(toLight, normal), 0.0);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse0, TexCoord)));

  vec3 reflectDir = reflect(fromLight, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular0, TexCoord)));

  return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  vec3 fromLight = normalize(FragPos - light.position);
  vec3 toLight = -fromLight;

  vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoord));
  
  float intensity = clamp((dot(fromLight, light.direction)-light.cutoff_outter) / (light.cutoff-light.cutoff_outter), 0.0f, 1.0f);

  float diff = max(dot(toLight, normal), 0.0f);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse0, TexCoord)));
  diffuse *= intensity;

  vec3 reflectDir = reflect(fromLight, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular0, TexCoord)));
  specular *= intensity;

  return (ambient + diffuse + specular) * attenuation;
}
