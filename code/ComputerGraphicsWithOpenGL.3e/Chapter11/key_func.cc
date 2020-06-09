#include <iostream>
using namespace std;

#include <GL/glut.h>

GLsizei winWidth = 400, winHeight= 300;

void init(void) {
  glClearColor(0.0, 0.0, 1.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void displayFcn(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 0.0, 0.0);
  glPointSize(3.0);
}

/**
 * 窗口尺寸改变时触发
 * 但不会立即刷新图元信息
*/
void winReshapeFcn(GLint newWidth, GLint newHeight) {
  cout << "winReshapeFcn " << newWidth << " " << newHeight << endl;
  glViewport(0, 0, newWidth, newHeight);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();   // ?
  gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));

  winWidth = newWidth;
  winHeight = newHeight;
}

void plotPoint(GLint x, GLint y) {
  cout << "draw point " << x << " " << y << endl;
  glBegin(GL_POINTS);
    glVertex2i(x, y);
  glEnd();
  glFlush();
}

/**
 * 按下C时开始绘制坐标点
*/
void curveDrawing(GLubyte curvePlotKey, GLint xMouse, GLint yMouse) {
  GLint x = xMouse;
  GLint y = winHeight - yMouse;

  switch(curvePlotKey) {
    case 'c': {
        plotPoint(x, y);
    }break;
    default:
    break;
  }
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(winWidth, winHeight);
  glutCreateWindow("Keyboard Curve-Drawing Example");

  init();
  
  glutDisplayFunc(displayFcn);        // 每次绘制图元（glBegin）之前都会调用的函数，用来设置画笔的粗细、颜色

  glutReshapeFunc(winReshapeFcn);
  glutKeyboardFunc(curveDrawing);

  glutMainLoop();
}