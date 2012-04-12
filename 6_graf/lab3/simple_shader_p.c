/* Pawel Topa, 2007
   Shaders */	


#include <stdio.h>
#include <fstream>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#define PI 3.141596
#define PI_OVER_360 0.0087266

using namespace std;

GLfloat angle, zoom, ye, ze, xe;

float projMat[16] = {0};

GLfloat pos[4] = {25.0, 35.0, -2.0, 1.0};
GLfloat red[4] = {0.9, 0.0, 0.0, 1.0};
GLfloat green[4] = {0.1, 0.9, 0.2, 1.0};
GLfloat blue[4] = {0.3, 0.3, 1.0, 1.0};
GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0f};

//DECLARE HERE

unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;

    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);

    return len;
}

int loadShader(char* filename, GLchar** ShaderSource, int * len)
{
   ifstream file;
   fprintf(stderr, "shader file name %s\n", filename);
   file.clear();
   file.open(filename, ios::binary); // opens as ASCII!
   if(!file) return -1;

  (*len) = getFileLength(file);

   if ((*len)==0) return -2;   // Error: Empty File

   (*ShaderSource) = (GLchar*) new char[(*len)+1];
   if ((*ShaderSource) == 0) return -3;   // can't reserve memory

    //Koniec łańcucha znaków powinien być oznaczony 0
    //nie zawsze jednak wartość len odpowiada rzeczywistej długości 
    //ponieważ niektóre znaki mogą być pomijane podczas czytania
    //na koniec należy upewnić się co do pozycji "0".
    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value...
   (*ShaderSource)[(*len)] = 0;

   unsigned int i=0;
   while (file.good())
   {
       (*ShaderSource)[i] = file.get();       // get character from file.
       if (!file.eof())
        i++;
   }

   (*ShaderSource)[i] = 0;  // Koniec łańcucha znaków kody shadera - obowiązkowo

   file.close();

   return 0;
}


int unloadshader(GLubyte** ShaderSource)
{
   if (*ShaderSource != 0)
     delete[] *ShaderSource;
   *ShaderSource = 0;
   return 0;
}


void setupShaders(char* vertexfile, char* fragmentfile, 
			GLuint *Program, 
			GLuint *vertexShaderObject,
			GLuint *fragmentShaderObject)
{
  //TODO
  

}


void init()
{
	
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);


	angle = 0.0f;
	ye = 17.0;
	xe = 30.0f;
	ze = 0.0f; 
	
	
	
	

}
	
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/ (GLfloat) h, 1.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(xe, ye, ze, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	
	
	glutWireTorus(2, 12, 20, 20);
	
	
	
	glutSolidTeapot(5);
	
	
	glFinish();
	glutSwapBuffers();
}


void keybs(int skey, int x, int y ) {
	switch (skey) {
	case GLUT_KEY_LEFT: xe -=1.0f;
		break;
	case GLUT_KEY_RIGHT: xe +=1.0f;
		break;
	case GLUT_KEY_UP: ye -=1.0f;
		break;
	case GLUT_KEY_DOWN: ye += 1.0f;
		break;
	case GLUT_KEY_PAGE_UP: ze+=1.0f;
		break;
	case GLUT_KEY_PAGE_DOWN: ze-=1.0f;
		break;
	}
	glutPostRedisplay();
}





int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	
	fprintf(stdout, "OpenGL version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keybs);
	glutMainLoop();
	return 0;
}
