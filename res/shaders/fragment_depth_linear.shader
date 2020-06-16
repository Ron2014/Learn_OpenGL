#version 330 core
out vec4 FragColor;

uniform float near; 
uniform float far; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

float Linearize(float d) {
  return d * near / (far + d * (near - far));
}

/**
完全不明白教程的公式怎么来的.
为什么要从[0,1]变换到[-1,1]再参与运算?
反正函数1简化之后和函数2一摸一样.

令 d = gl_FragCoord.z
则有 d = \frac{1/z-1/near}{1/far-1/near}
逆推 z = \frac{far*near}{far+d*near-d*far}
我们要求得新的深度, 公式 d' = \frac{z-near}{far-near}
代入z
就最终得到 d' = \frac{d*near}{far+d*near-d*far}

线性的灰度空间有一种重度雾霾的感觉.
*/

void main()
{             
    // float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    float depth = Linearize(gl_FragCoord.z);
    FragColor = vec4(vec3(depth), 1.0);
}