#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

struct Light {
    vec3 position;
    vec3 color;
};

struct Material {
  sampler2D diffuse;
};

uniform Light lights[4];
uniform Material material;
uniform vec3 viewPos;

void main()
{           
    vec3 color = texture(material.diffuse, fs_in.TexCoord).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; i++) {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = lights[i].color * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - lights[i].position);
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    FragColor = vec4(ambient + lighting, 1.0);
}