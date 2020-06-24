#version 330 core
out vec4 FragColor;  
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
uniform PointLight pointLight;

struct Material {
    sampler2D diffuse;
    sampler2D normal;
}; 

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

uniform sampler2D shadowMap;
uniform samplerCube shadowCubemap;
uniform Material material;      // == objColor

uniform DirectLight directLight;
uniform vec3 viewPos;
uniform bool blinn;
uniform bool gamma;

uniform float near_plane;
uniform float far_plane;

float ShadowCalcDirectLight(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
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

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float ShadowCalcPointLight(vec3 fragToLight, vec3 normal) {
    float currentDepth = length(fragToLight);   

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fs_in.FragPos);
    // float diskRadius = 0.05;
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
  vec3 ambient = light.ambient;
  
  vec3 fromLight = light.direction;
  vec3 toLight = -fromLight;
  float diff = max(dot(toLight, normal), 0.0);
  vec3 diffuse = light.diffuse * diff;

  float spec;
  if (blinn) {
    vec3 halfwayDir = normalize(viewDir + toLight);
    spec = pow(max(dot(normal, halfwayDir), 0.0f), 32.0f);
  } else {
    vec3 reflectDir = reflect(fromLight, normal);
    spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8.0f);
  }
  vec3 specular = light.specular * spec * 0.005;

  float shadow = ShadowCalcDirectLight(fs_in.FragPosLightSpace, normal, toLight);
  return ambient + (diffuse + specular) * (1.0f - shadow);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  vec3 lightToFrag = fragPos - light.position;
  float distance = length(lightToFrag);
  float attenuation;
  if (gamma) 
    attenuation = 1.0 / (light.constant + light.quadratic * distance * distance);
  else
    attenuation = 1.0 / (light.constant + light.linear * distance);

  vec3 ambient = light.ambient;
  
  vec3 fromLight = normalize(lightToFrag);
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

  float shadow = ShadowCalcPointLight(lightToFrag, normal);
  return (ambient + (diffuse + specular) * (1.0f - shadow)) * attenuation;
}

void main()
{
  vec3 texColor = texture(material.diffuse, fs_in.TexCoord).rgb;
  // 从法线贴图范围[0,1]获取法线
  vec3 norm = texture(material.normal, fs_in.TexCoord).rgb;
  // 将法线向量转换为范围[-1,1]
  norm = normalize(norm * 2.0 - 1.0);   
  // vec3 norm = normalize(fs_in.Normal);
  vec3 viewDir = normalize(viewPos - fs_in.FragPos);

  vec3 lighting = vec3(0.0f);
  lighting += CalcDirectLight(directLight, norm, viewDir);
  lighting += CalcPointLight(pointLight, norm, viewDir, fs_in.FragPos);
  texColor *= lighting;
  if (gamma) texColor = pow(texColor, vec3(1.0f/2.2f));
  FragColor = vec4(texColor, 1.0);
}
