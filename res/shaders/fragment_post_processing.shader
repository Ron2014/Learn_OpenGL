#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;

vec4 Default() {
    return vec4(texture(texture0, TexCoord).rgb, 1.0);
}

// 反相
vec4 Inversion() {
    return vec4(vec3(1.0f) - texture(texture0, TexCoord).rgb, 1.0);
}

// 灰度
vec4 Grayscale() {
    vec4 col = texture(texture0, TexCoord);
    float average = (col.r + col.g + col.b) / 3.0f;
    return vec4(average, average, average, 1.0f);
}

// 灰度加权
// 人眼会对绿色更加敏感一些，而对蓝色不那么敏感
vec4 GrayscaleWeighted() {
    vec4 col = texture(texture0, TexCoord);
    float average = col.r *0.2126 + col.g * 0.7152 + col.b * 0.0722;
    return vec4(average, average, average, 1.0f);
}

// 核(Kernel)（或卷积矩阵(Convolution Matrix)）
// 锐化核, 不少运动类照片通过该处理来凸显身形线条和力量感.
const float offset = 1.0 / 300.0;  
vec4 KernelSharpen3x3() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(texture0, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

// 模糊
vec4 KernelBlur3x3() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(texture0, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

// 边缘检测
vec4 KernelEdge_detection3x3() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        1.0 / 8, 1.0 / 8, 1.0 / 8,
        1.0 / 8, -8.0 / 8, 1.0 / 8,
        1.0 / 8, 1.0 / 8, 1.0 / 8  
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(texture0, TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}

void main()
{
    // FragColor = Default();
    // FragColor = Inversion();
    // FragColor = Grayscale();
    // FragColor = GrayscaleWeighted();
    // FragColor = KernelSharpen3x3();
    // FragColor = KernelBlur3x3();
    FragColor = KernelEdge_detection3x3();
}