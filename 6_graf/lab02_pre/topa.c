/* Pawel Topa, 2007
   Teksturowanie */	
#include <GL/glut.h>
#include <stdio.h>
#include "bitmaps.h"

#define PI 3.141596


BITMAPINFO *BitmapInfo1;
void *BitmapBits1;

BITMAPINFO *BitmapInfo2;
void *BitmapBits2;

BITMAPINFO *BitmapInfo3;
void *BitmapBits3;

static GLuint texName[3];

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


/*normalizacja wektora*/
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

/*obliczanie wektora normalnego*/
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




void init(char* fileName1, char* fileName2, char* fileName3)
{
	void *bits;
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//wielokąty wypełnione/ połączpne wierzchołki/...
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
	

/************************************************************************/
  /*wczytanie bitmapy 1 z pliku*/
/*wczytuje się obraz z pliku, na jego podstawie tworzona jest tekstura. tekstura nie ma rozdzielczości, trzeba ją naciągać w zależności od rzutowania. ustalamy paramatry rzutowania , filtorwanie - jak realizować zwiąkszanie/zmniejsaanie tekstury...*/
  if (!(bits = LoadBitmap(fileName1, &BitmapInfo1))) exit(0);
  BitmapBits1 = ConvertRGB(BitmapInfo1, bits);
  free(bits);

  if (BitmapBits1 == NULL)
  {
    printf("Could not convert bitmap to RGB!");
    free(BitmapInfo1);
    BitmapInfo1 = NULL;
  }
 /************************************************************************/
 /************************************************************************/
  /*wczytanie bitmapy 2 z pliku*/
  if (!(bits = LoadBitmap(fileName2, &BitmapInfo2))) exit(0);
  BitmapBits2 = ConvertRGB(BitmapInfo2, bits);
  free(bits);

  if (BitmapBits2 == NULL)
  {
    printf("Could not convert bitmap to RGB!");
    free(BitmapInfo2);
    BitmapInfo2 = NULL;
  }
 /************************************************************************/
 /************************************************************************/
  /*wczytanie bitmapy 3 z pliku*/
  if (!(bits = LoadBitmap(fileName3, &BitmapInfo3))) exit(0);
  BitmapBits3 = ConvertRGB(BitmapInfo3, bits);
  free(bits);

  if (BitmapBits3 == NULL)
  {
    printf("Could not convert bitmap to RGB!");
    free(BitmapInfo3);
    BitmapInfo3 = NULL;
  }
 /************************************************************************/
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glEnable(GL_TEXTURE_2D);
  

	glGenTextures(3, texName);
	
	
	/*...*/
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BitmapInfo1->bmiHeader.biWidth, 
			BitmapInfo1->bmiHeader.biHeight, 0, 
			GL_RGB, GL_UNSIGNED_BYTE, BitmapBits1);

	/*...*/
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BitmapInfo2->bmiHeader.biWidth, 
			BitmapInfo2->bmiHeader.biHeight, 0, 
			GL_RGB, GL_UNSIGNED_BYTE, BitmapBits2);

	/*...*/
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BitmapInfo3->bmiHeader.biWidth, 
			BitmapInfo3->bmiHeader.biHeight, 0, 
			GL_RGB, GL_UNSIGNED_BYTE, BitmapBits3);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); 
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}
	
void reshape(int w, int h)	//do rzutowania, "stały element gry", patrz: main
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
		glBegin(GL_TRIANGLE_STRIP);	//początek defiicji wierzchołków
		for(z = 0; z < 1.0f; z+=krok)
			{
			glNormal3f(0.0f, 1.0f, 0.0f);	//cośtam  z wektorm normalnym,pozostaje taki do następnego wywołąnia.
			glTexCoord2f(x+krok, z);  glVertex3f(x+krok, value, z); 
			glTexCoord2f(x, z);       glVertex3f(x, value, z);
			};
		glEnd();//jeden do każego begin
	}; 
}

/*display w jakiejs czesci też wszędzie wyglada tak samo*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//czyści bufory
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*openGL przechowyue stos macierzy przekaszałceń. LoadIdentity ustawia I(macierz identyczności) jako macierz przekształcenia, czyli resetuje przekształcenia. Mamy do czynienia z lokalnym układem odniesienia*/
	if(light)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if(texture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	/*polozenia obserwatora - troche inaczej niz zwykle*/
	glRotatef(180.0f*atan(ye/zoom)/PI, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -ye, -zoom);
	glRotatef(angle, 0.0, 1.0, 0.0);
	
	
	glLightfv(GL_LIGHT0, GL_POSITION, pos);	//umieszcza w ukłądzie odniesienia światło
	
	glPushMatrix();
	/*w trakcie rysowania zachodzi konieczność poruszania elemetów sceny względem siebie. pushmatrix zapamiętuje aktualnny stan przekształceń, tzn zapamiętuję w zasadzie położenie układu odniesienia. */
		glPushMatrix();	//operacja na stosie macierzy przekształcenia. 
			glTranslatef(-90.0f, -5.0f, -90.0f);	//przesuwa o wektor uklad odniesienia (lokalny?) - odsuwa od obserwarota. 
			glScalef(180.0f, 0.0f, 180.0f);		 
			/*...*/
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			drawTexturedMesh();	//tutaj rysowanie, zawsze w punkcie 0,0 układu odniesienia (być może odsuniętego od obserwatora.) Funkcja powyżej
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(28.0f, -5.0f, 28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/*...*/
			glBindTexture(GL_TEXTURE_2D, texName[2]);
			gluCylinder(obj1, 8.0f, 8.0f, 15.0f, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(0.0f, 7.0f, 0.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/*...*/
			glBindTexture(GL_TEXTURE_2D, texName[1]);
			gluSphere(obj1, 10.0, 30, 30);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(28.0f, 10.0f, 28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/*...*/
			glBindTexture(GL_TEXTURE_2D, texName[2]);
			gluCylinder(obj1, 5.0f, 0.0f, 8.0f, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(28.0f, 10.0f, 28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);
			/*...*/
			glBindTexture(GL_TEXTURE_2D, texName[2]);
			gluDisk(obj1, 5.0f, 8.0f,  20, 2);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-28.0f, 5.0f, -28.0f);
			glRotatef(-90.0f, 1.0, 0.0f, 0.0f);

			glBindTexture(GL_TEXTURE_2D, texName[1]);
			gluSphere(obj1, 10.0, 30, 30);
		glPopMatrix();

	
	glPopMatrix();

	
	glFinish();	//zmusz potok renderowania do opróżnienia się przed rysowaniem (?)
	glutSwapBuffers();	//zmiana buforów (patrz: double buffer w main)
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
		glutPostRedisplay();	//wymuszenie przerysowania ekranu
	};
};

/*przedrostek funkcji oznacza skąd wziełą się dakna funkcja - gltu, glu, gl ; gl - natywne
glu miałą rozszerzyć możliwości openGL,
glut miał zapewnić przenośność kodu pomiędzy platformami. Zadaniem gluta jest ukrycie tego, że systemu okienkowe w różnych systemach różnią się. openGL nie zawieta funkcji platform specific?.
*/



/*w każdyn OpenGLowym programie(przynajmniej w starym openGL) main będzie wyglądał podobnie*/
int main(int argc, char** argv) {
	glutInit(&argc, argv); //inicjalizuje opengl i gluta. z argumentów w zasadzie się nie korzysta. musi być w tej postaci
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //jaki chcemy mieć tryb renderowania. GLUT_RGB  - bufor kolorów w formacie RGB(),  GLUT_DOUBLE (chcemy stosować podwójny bufor kolorów - do animacji. piszemy po tym niewidocznym, a potem zamieniamy) GLUT_DEPTH - włłącza algorytm usuwania niewidocznych krawędzi
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);	//obowiązkowa, tworzy wszystkie potrzebne struktury

	init("grass.bmp", "ziemia.bmp", "bricks.bmp");
	//kolejne funkcje to obsługa zdarzeń. mechanizm calllbacków, funkcje wywoływane kiedy zajdzie odpowiednie zdarzenie. arg - nazwa funkcji (wskaźnik na funkcję)	
	glutDisplayFunc(display);	//kiedy zajdzie konieczność przerysowania ekranu
	glutReshapeFunc(reshape);	//zmienił się rozmiar okna (tu musimu od nowa ustawić od nowa parametry rzutowania)
	glutSpecialFunc(skeys);		//naciśniecie kalwisza niealfanumerycznego
	glutKeyboardFunc(keybs);	//naciśniecie alfanumerycznego
	glutIdleFunc(idle);		//program nic nie robi. wywołuje się, kiedy program kończy jakąś operację

	glutMainLoop();
	return 0;
}
