#include "UnitTest++/src/UnitTest++.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
using namespace std;
using namespace glm;

/**
 * glm 的 vector 乘法，是各分量相乘
 * 结果还是个 vector。
 * 
 * 所以既不是点乘，也不是叉乘。
*/
TEST(GLMVec3Mul) {
  vec3 lightColor(1.0f, 1.0f, 1.0f);
  vec3 toyColor(1.0f, 0.5f, 0.31f);
  vec3 result = lightColor * toyColor;
  cout << result.x << " " << result.y << " " << result.z << endl;
}