#include <iostream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char *argv[]) {
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);  // 原始坐标
  glm::mat4 trans(1.0f);                  // 单位矩阵

  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));  // 构造位移矩阵
  /***
   * m 4x4矩阵, v=3x1向量
   * m 的分量 m[X] 是个 4维向量
   * v 的分量 v[Y] 是个 标量数字
   * 于是就有了下面的公式
   * 
   * mat<4, 4, T, Q> Result(m);
   * Result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	 * return Result;
   */

  vec = trans * vec;
  std::cout << vec.x << "," << vec.y << "," << vec.z << std::endl;
}