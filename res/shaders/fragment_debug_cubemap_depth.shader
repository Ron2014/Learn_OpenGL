#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform samplerCube shadowCubemap;

void main()
{
  float depthValue = texture(shadowCubemap, FragPos).r;
  FragColor = vec4(vec3(depthValue), 1.0f);
}