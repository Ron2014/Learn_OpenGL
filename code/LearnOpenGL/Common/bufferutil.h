
#include "texture2d.h"
#include <vector>

extern unsigned int WIN_WIDTH;
extern unsigned int WIN_HEIGHT;

void initBuffer(unsigned int &vao, unsigned int &vbo, const unsigned int size, const void* data, const vector<unsigned int> &attrsize);
Texture2D *initFrameBuffer(unsigned int &frameBufferID, string uniform_name = "");
Texture2D *initFrameBufferSample(unsigned int &frameBufferID, const int sample, string uniform_name = "");
Texture2D *initFrameBufferColor(unsigned int &frameBufferID, string uniform_name = "");


Texture2D *initFrameBufferDepth(unsigned int &frameBufferID, string uniform_name = "", int width=WIN_WIDTH, int height=WIN_HEIGHT);
Cubemaps *initFrameBufferCubemapDepth(unsigned int &frameBufferID, string uniform_name = "", int width=WIN_WIDTH, int height=WIN_HEIGHT);