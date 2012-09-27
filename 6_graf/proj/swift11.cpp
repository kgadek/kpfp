#include <GL/gl.h>
#include <GL/glut.h>
GLfloat angle = 0.0;

void cube(void) {
    glRotatef(angle, 0.3f, 0.0f, 0.0);
    glRotatef(angle, 0.0f, 0.5f, 0.0);
    glRotatef(angle, 0.0f, 0.0f, 0.7);
    glColor3f(1.0f, 0.5f, 0.0f);
    glutSolidTeapot(1.5f);
}

void display(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  
    gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0);
    cube();
    glutSwapBuffers();
    angle ++;
}

void reshape(int w, int h) {
    glViewport(0, 0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(GLfloat)w /(GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 700);
    glutCreateWindow("Cool window w/ OpenGL stuff");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

