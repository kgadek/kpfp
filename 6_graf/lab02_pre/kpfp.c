#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

/*#define PI 3.141596


GLfloat angle, zoom, ye;

GLfloat rlight, alfa;

int light, texture, move;


GLUquadricObj *obj1;


GLfloat pos[4] = {25.0, 35.0, -2.0, 1.0};
GLfloat red[4] = {0.9, 0.0, 0.0, 1.0};
GLfloat green[4] = {0.1, 0.9, 0.2, 1.0};
GLfloat blue[4] = {0.3, 0.3, 1.0, 1.0};
GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	
GLfloat ambientLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat diffuseLight[4] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat specularLight[4] = {1.0f, 1.0f, 1.0f, 1.0f};


/ *normalizacja wektora* /
void ReduceToUnit(float vector[3])
{
	float length;
	
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));
	if(length == 0.0f)
		length = 1.0f;

	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

/ *obliczanie wektora normalnego* /
void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}




//void init(char* fileName1, char* fileName2, char* fileName3) {
void init() {
	void *bits;
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	alfa = 0.0f;
	rlight = 50.0f;
	pos[0] = rlight*cos(alfa*PI/180.0);
	pos[2] = rlight*sin(alfa*PI/180.0);
	angle = 0.0f;
	zoom = 55.0f;
	ye = 7.0;

	light = 1;
	texture = 1; 

	obj1 = gluNewQuadric();



	gluQuadricDrawStyle(obj1, GLU_FILL);
	

	gluQuadricNormals(obj1, GLU_SMOOTH);
	

	gluQuadricOrientation(obj1, GLU_OUTSIDE);
	

	gluQuadricTexture(obj1, GLU_TRUE);
	

 / ************************************************************************ /
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glEnable(GL_TEXTURE_2D);
  

	/ *...* /
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/ *...* /
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/ *...* /
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); 
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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


void drawTexturedMesh(){
	float value = 0.0f, krok = 0.05f;
	float x, z;
	glColor3f(1.0, 1.0, 1.0);

	for(x = 0; x < 1.0f; x+=krok)
	{
		glBegin(GL_TRIANGLE_STRIP);	
		for(z = 0; z < 1.0f; z+=krok)
			{
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(x+krok, z);  glVertex3f(x+krok, value, z); 
			glTexCoord2f(x, z);       glVertex3f(x, value, z);
			};
		glEnd();
	}; 
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//if(light)
	//	glEnable(GL_LIGHTING);
	//else
		glDisable(GL_LIGHTING);

	//if(texture)
	//	glEnable(GL_TEXTURE_2D);
	//else
		glDisable(GL_TEXTURE_2D);

	/ *polozenia obserwatora - troche inaczej niz zwykle* /
	glRotatef(180.0f*atan(ye/zoom)/PI, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -ye, -zoom);
	glRotatef(angle, 0.0, 1.0, 0.0);
	
	
	glPushMatrix();
	
		glPushMatrix();
			glTranslatef(-90.0f, -5.0f, -90.0f);
			glScalef(180.0f, 0.0f, 180.0f);
			/ *...* /
			drawTexturedMesh();
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(28.0f, -5.0f, 28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/ *...* /
			gluCylinder(obj1, 8.0f, 8.0f, 15.0f, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(0.0f, 7.0f, 0.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/ *...* /
			gluSphere(obj1, 10.0, 30, 30);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(28.0f, 10.0f, 28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/ *...* /
			gluCylinder(obj1, 5.0f, 0.0f, 8.0f, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(28.0f, 10.0f, 28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/ *...* /
			gluDisk(obj1, 5.0f, 8.0f,  20, 2);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-28.0f, 5.0f, -28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);

			gluSphere(obj1, 10.0, 30, 30);
		glPopMatrix();

	
	glPopMatrix();

	
	glFinish();
	glutSwapBuffers();
}


void skeys(int skey, int x, int y ) {
	switch (skey) {
	case GLUT_KEY_LEFT: angle -=2.0f;
		if (angle < 0.0f) angle = 360.0f;
		break;
	case GLUT_KEY_RIGHT: angle +=2.0f;
		if (angle > 360.0f) angle = 0.0f;
		break;
	case GLUT_KEY_UP: zoom -=2.0f;
		if(zoom < 2.0f) zoom = 2.0f;
		break;
	case GLUT_KEY_DOWN: zoom += 2.0f;
		break;
	case GLUT_KEY_PAGE_UP: ye+=2.0f;
		break;
	case GLUT_KEY_PAGE_DOWN: ye-=2.0f;
		break;
	}
	glutPostRedisplay();
}


void keybs(unsigned char key, int x, int y) {

	if(key == 'l')
		light = !light;
	if(key == 't')
		texture = !texture;
	if(key == 'm')
		move = !move;
	glutPostRedisplay();
	
}


void idle()
{
	if(move)
	{
		alfa+=1.0f;
		if(alfa > 360.0) alfa = 0.0f;
		pos[0] = rlight*cos(alfa*PI/180.0);
		pos[2] = rlight*sin(alfa*PI/180.0);
		glutPostRedisplay();	
	};
};

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(950, 540);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	
	obj1 = gluNewQuadric();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutSpecialFunc(skeys);
	//glutKeyboardFunc(keybs);
	//glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}*/

const float SMALLSTEPD = 10.0f;

float angle = 0.0f, step = 0.5f;

void timer(int t) {
	angle += step;
	if(angle>SMALLSTEPD*360.0f)
		angle -= SMALLSTEPD*360.0f;

	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat) w/ (GLfloat) h, 1.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
};

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//gluLookAt(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, sin(angle/SMALLSTEPD), cos(angle/SMALLSTEPD));

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -4.0f);
		glRotatef(angle, 5.0f, 7.0f, 11.0f);
		glutWireTeapot(1.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(3.0f*sin(180.0f+angle/SMALLSTEPD), 3.0f*cos(180.0f+angle/SMALLSTEPD), -4.0f);
		glRotatef(angle, -17.0f, 19.0f, -13.0f);
		glutWireTeapot(0.3f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(3.0f*sin(90.0f+angle/SMALLSTEPD), 3.0f*cos(90.0f+angle/SMALLSTEPD), -4.0f);
		glRotatef(angle, -17.0f, 19.0f, -13.0f);
		glutWireTeapot(0.3f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(3.0f*sin(angle/SMALLSTEPD), 3.0f*cos(angle/SMALLSTEPD), -4.0f);
		glRotatef(angle, -17.0f, 19.0f, -13.0f);
		glutWireTeapot(0.3f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -4.0f);
		glRotatef(3.0f*angle, 1.0f, 0.0f, 0.0f);
		glutWireTorus(1.0f, 4.0f, 36.0f, 36.0f);
	glPopMatrix();

	glFinish();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(950, 540);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);
	
	// init
	glClearColor(0.15f, 0.15f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	//glutSpecialFunc(skeys);
	//glutKeyboardFunc(keybs);
	//glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

