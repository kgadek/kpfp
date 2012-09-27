#include <GL/gl.h>
#include <GL/glut.h>

GLfloat redDiffuseMaterial[]    = { 1.f, 0.f, 0.f }; // set the material to red
GLfloat whiteSpecularMaterial[] = { 1.f, 1.f, 1.f }; // set the material to white
GLfloat greenEmissiveMaterial[] = { 0.f, 1.f, 0.f }; // set the material to green
GLfloat whiteSpecularLight[]    = { 1.f, 1.f, 1.f }; // set the light specular to white
GLfloat blackAmbientLight[]     = { 0.f, 0.f, 0.f }; // set the light ambient to black
GLfloat whiteDiffuseLight[]     = { 1.f, 1.f, 1.f }; // set the diffuse light to white
GLfloat blankMaterial[]         = { 0.f, 0.f, 0.f }; // set the diffuse light to white
GLfloat mShininess[]            = { 128.f };         // set the shininess of the material

GLfloat angle = 0.0;

GLfloat step = 0.1;
GLfloat wsad[] = {0.f, 0.f};

void display(void) {
    glClearColor(0.f,0.f,0.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 

    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redDiffuseMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);

    glPushMatrix();
    glTranslatef(-2.f,0.f,-5.f);
    glRotatef(angle,wsad[0],wsad[1],1.f);
    glutSolidTeapot(1.f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.f,0.f,-5.f);
    glRotatef(angle,1.f,wsad[1],-wsad[0]);
    glutSolidTeapot(1.f);
    glPopMatrix();

    glutSwapBuffers();
    angle++;
}

void reshape(int w, int h) {
    glViewport(0, 0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,(GLfloat)w /(GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'w': wsad[0] += step; break;
        case 's': wsad[0] -= step; break;
        case 'a': wsad[1] -= step; break;
        case 'd': wsad[1] += step; break;
        default:
            ;
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("A basic OpenGL Window");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}

