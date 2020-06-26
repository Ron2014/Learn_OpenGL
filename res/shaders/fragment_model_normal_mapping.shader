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

float ShadowCalcDirectLight(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowCalcPointLight(vec3 fragToLight, vec3 normal);

// normal 用来计算漫反射
// viewDir 用来计算高光
vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
// 拥有衰减属性的光, 需要片段坐标计算距离和角度
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos);

out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

uniform sampler2D shadowMap;
uniform samplerCube shadowCubemap;
uniform vec3 viewPos;
uniform Material material;      // == objColor

uniform DirectLight directLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform float near_plane;
uniform float far_plane;

void main() {
    // 属性
    vec3 normal = texture(material.normal0, fs_in.TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(fs_in.TBN * normal);

    // vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = vec3(0.0f);

    // 第一阶段：定向光照
    result += CalcDirectLight(directLight, normal, viewDir);
    // 第二阶段：点光源
    result += CalcPointLight(pointLight, normal, viewDir, fs_in.FragPos);
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, normal, viewDir, fs_in.FragPos);    

    FragColor = vec4(result, 1.0);
}

float ShadowCalcDirectLight(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
  // return 0.0;
  // if (dot(normal, lightDir) <=0 ) return 1.0f;
  // 执行透视除法
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float currentDepth = projCoords.z;
  if(currentDepth > 1.0) return 0.0f;
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
  for(int x = -1; x <= 1; ++x) {
      for(int y = -1; y <= 1; ++y) {
          float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
      }    
  }
  shadow /= 9.0;
  return shadow;
}

vec3 sampleOffsetDirections[20] = vec3[] (
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float ShadowCalcPointLight(vec3 fragToLight, vec3 normal) {
    // if (dot(normal, fragToLight) <=0 ) return 1.0f;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fs_in.FragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i) {
        float closestDepth = texture(shadowCubemap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        shadow += currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    }
    shadow /= float(samples);
    return shadow;
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir) {
  vec2 texCoord = fs_in.TexCoord;
  vec3 ambient = light.ambient * vec3(texture(material.diffuse0, texCoord));
  
  vec3 fromLight = light.direction;
  vec3 toLight = -fromLight;
  float diff = max(dot(toLight, normal), 0.0);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse0, texCoord)));

  vec3 halfwayDir = normalize(viewDir + toLight);
  float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular0, texCoord)));

  float shadow = ShadowCalcDirectLight(fs_in.FragPosLightSpace, normal, toLight);
  return ambient + (diffuse + specular) * (1.0f - shadow);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  vec3 fragToLight = fragPos - light.position;
  vec2 texCoord = fs_in.TexCoord;
  float distance = length(fragToLight);
  // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
  float attenuation = 1.0 / (light.constant + light.quadratic * distance * distance);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse0, texCoord));
  
  vec3 fromLight = normalize(fragToLight);
  vec3 toLight = -fromLight;
  float diff = max(dot(toLight, normal), 0.0);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse0, texCoord)));

  float spec;
  vec3 halfwayDir = normalize(viewDir + toLight);
  spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

  // vec3 reflectDir = reflect(fromLight, normal);
  // spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular0, texCoord)));

  float shadow = ShadowCalcPointLight(fragToLight, normal);
  return (ambient + (diffuse + specular) * (1.0f - shadow)) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos) {
  vec2 texCoord = fs_in.TexCoord;
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  vec3 fromLight = normalize(FragPos - light.position);
  vec3 toLight = -fromLight;

  vec3 ambient = light.ambient * vec3(texture(material.diffuse0, texCoord));
  
  float intensity = clamp((dot(fromLight, light.direction)-light.cutoff_outter) / (light.cutoff-light.cutoff_outter), 0.0f, 1.0f);

  float diff = max(dot(toLight, normal), 0.0f);
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse0, texCoord)));
  diffuse *= intensity;

  vec3 reflectDir = reflect(fromLight, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular0, texCoord)));
  specular *= intensity;

  return (ambient + diffuse + specular) * attenuation;
}
