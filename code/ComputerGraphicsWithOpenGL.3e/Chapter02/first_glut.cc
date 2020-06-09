#include <GL/glut.h>
#include <GL/freeglut.h>        // for glutLeaveMainLoop

void KeyFunction(GLubyte key, GLint xMouse, GLint yMouse) {
    switch(key) {
        case VK_ESCAPE: {
            glutLeaveMainLoop();
        }
        break;
    }
}

/**
 * create a graphic with a line
*/
void lineSegment() {
    glClear(GL_COLOR_BUFFER_BIT);       // flush buffer to set the default color
    glColor3f(1.0, 0.0, 0.0);           // object color

    // 拉伸窗口，线段依然完整显示
    glBegin(GL_LINES);                  // draw a line, NOTES: GL_LINES, not GL_LINE
        glVertex2i(180, 15);
        glVertex2i(10, 145);
    glEnd();

    glFlush();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);                              // init
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);        // display mode
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 300);
    glutCreateWindow("An Example OpenGL Program");

    glClearColor(1.0, 1.0, 1.0, 0.0);   // background color of window
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 150.0); // view size of window
    
    glutKeyboardFunc(KeyFunction);

    glutDisplayFunc(lineSegment);       // show the graphic
    glutMainLoop();                     // render loop

    return 0;
}