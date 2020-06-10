#include <glad/glad.h>

#define TEXTURE_PATH "E:\\GitHub\\Learn_OpenGL\\res\\images"
#define TEXTURE_DEFAULT "avengers.jpg"

class Texture2D {
  static unsigned int TEXTURE_UNIT_ID;
public:
  unsigned int ID;
  unsigned int unitID;
  
  int width;
  int height;
  int nrChannels;
  unsigned char* data;

  Texture2D(const GLchar *texPath = TEXTURE_DEFAULT);
  ~Texture2D();

  void use();
};