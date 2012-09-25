/* Pawel Topa, 2007
   Shaders */	


#include <stdio.h>
#include <fstream>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>


#include "obj.h"

#define PI 3.141596
#define PI_OVER_360 0.0087266

using namespace std;

//Vertex Buffer Objects
#define VERTICES 0
#define COLORS 1
#define NORMALS 2
#define INDICES 3
#define NUM_BUFFERS 4
#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))
GLuint handleVAO;
GLuint handleOBJVAO;
GLuint OBJbuffers[NUM_BUFFERS];
GLuint buffers[NUM_BUFFERS];
GLint NumVertices;
GLint NumIndices;
typedef struct 
{
        GLfloat x,y,z;
       // GLfloat r,g,b;
} SVertex;
SVertex *Vertices;
SVertex *Colors;
SVertex *Normals;
GLuint* Indices;



GLfloat angle, zoom, ye, ze, xe;


GLfloat pos[4] = {25.0, 35.0, -2.0, 1.0};
GLfloat red[4] = {0.9, 0.0, 0.0, 1.0};
GLfloat green[4] = {0.1, 0.9, 0.2, 1.0};
GLfloat blue[4] = {0.3, 0.3, 1.0, 1.0};
GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0f};



GLfloat light0_position[] = {0.0, 0.0, 0.0, 1.0};  //światło pozycyjne
GLfloat light1_position[] = {0.0, 0.0, 0.0, 0.0};  //światło kierunkowe
GLfloat light2_position[] = {0.0, 0.0, 0.0, 1.0};  //światło typu spot
GLfloat light2_direction[] = {0.0, -1.0, 0.0, 1.0}; //kierunek światła typu spot
GLint rotLight = 0;

GLfloat white_light[] = {0.2, 0.2, 0.2, 1.0};
GLfloat red_light[] = {1.0, 0.0, 0.0, 1.0};
GLfloat blue_light[] = {0.0, 0.0, 1.0, 1.0};

GLfloat ambient_mat[] = {0.2, 0.2, 0.2, 1.0};
GLfloat specular_mat[] = {0.5, 0.5, 0.5, 1.0};
GLfloat diffuse_mat[] = {0.2, 1.0, 0.2, 1.0};

objShape obj;



//Shaders
GLuint vertexShaderObjectA, fragmentShaderObjectA;
GLuint ProgramA;




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
  glewInit();
  
  //1. Stworzyć obiekty shaderów
  *vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
  *fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  
  //2. Wczytać kody źródłowe
  GLchar *vertexShaderSource=0, *fragmentShaderSource=0;
  GLint vlength=0, flength=0;
  
  loadShader((char*)vertexfile, &vertexShaderSource, &vlength);
  loadShader((char*)fragmentfile, &fragmentShaderSource, &flength);
  
  
  glShaderSource(*vertexShaderObject, 1, 
		(const GLchar**) &vertexShaderSource,
		&vlength); 
  glShaderSource(*fragmentShaderObject, 1, 
		(const GLchar**) &fragmentShaderSource,
		&flength); 
  //3. Kompilacja
  glCompileShader(*vertexShaderObject);
  glCompileShader(*fragmentShaderObject);
  
  GLint compiled=0; 
  glGetShaderiv(*vertexShaderObject, GL_COMPILE_STATUS, &compiled);
  if(!compiled)
  {
    GLint len;
    GLchar *log;
    glGetShaderiv(*vertexShaderObject, GL_INFO_LOG_LENGTH, &len);
    log = (GLchar*) malloc(len);
    
    glGetShaderInfoLog(*vertexShaderObject, len, &len, log);
    fprintf(stderr, "VS compile log: %s\n", log);
    free(log);
  }
  
  glGetShaderiv(*fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
  if(!compiled)
  {
    GLint len;
    GLchar *log;
    glGetShaderiv(*fragmentShaderObject, GL_INFO_LOG_LENGTH, &len);
    log = (GLchar*) malloc(len);
    
    glGetShaderInfoLog(*fragmentShaderObject, len, &len, log);
    fprintf(stderr, "FS compile log: %s\n", log);
    free(log);
  }
  
  //4. Stworzyć program
  *Program = glCreateProgram();
  
  //5. Połączyć program z shaderami
  glAttachShader(*Program, *vertexShaderObject);
  glAttachShader(*Program, *fragmentShaderObject);
  
  //6. Linkowanie 
  glLinkProgram(*Program);
  
  //weryfikacja linkowana
  GLint linked = 0;
  glGetProgramiv(*Program, GL_LINK_STATUS, &linked);
  if(!linked)
  {
    GLint len;
    GLchar *log;
    glGetProgramiv(*Program, GL_INFO_LOG_LENGTH, &len);
    log = (GLchar*) malloc(len);
    
    glGetProgramInfoLog(*Program, len, &len, log);
    fprintf(stderr, "Link log: %s\n", log);
    free(log);
  }
  
  //glUseProgram(...);
  
}


void createGeometry()
{

        NumVertices = 4; //kwadrat
        Vertices = new SVertex[NumVertices];
        Colors = new SVertex[NumVertices]; // kolory na podstawie parametrów
        Normals = new SVertex[NumVertices];
        
               
	Vertices[0].x = 5.0f;
	Vertices[0].y = 0.0f;
	Vertices[0].z = 5.0f;
	
	Vertices[1].x = 5.0f;
	Vertices[1].y = 0.0f;
	Vertices[1].z = -5.0f;
	
	Vertices[2].x = -5.0f;
	Vertices[2].y = 0.0f;
	Vertices[2].z = -5.0f;
	
	Vertices[3].x = -5.0f;
	Vertices[3].y = 0.0f;
	Vertices[3].z = 5.0f;
	

        Colors[0].x = 1.0f;
	Colors[0].y = 0.0f;
	Colors[0].z = 0.0f;
	
	Colors[1].x = 0.0f;
	Colors[1].y = 1.0f;
	Colors[1].z = 0.0f;
	
	Colors[2].x = 0.0f;
	Colors[2].y = 0.0f;
	Colors[2].z = 1.0f;
	
	Colors[3].x = 1.0f;
	Colors[3].y = 1.0f;
	Colors[3].z = 1.0f;
	
	
	Normals[0].x = 0.0f;
	Normals[0].y = 1.0f;
	Normals[0].z = 0.0f;
	
	Normals[1].x = 0.0f;
	Normals[1].y = 1.0f;
	Normals[1].z = 0.0f;
	
	Normals[2].x = 0.0f;
	Normals[2].y = 1.0f;
	Normals[2].z = 0.0f;
	
	Normals[3].x = 0.0f;
	Normals[3].y = 1.0f;
	Normals[3].z = 0.0f;
	
	
	NumIndices = 5;
	Indices = new GLuint[NumIndices];
	
	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;
	Indices[3] = 3;
	Indices[4] = 0;
}


void setupVBO()
{
    glGenVertexArrays(1, &handleVAO);
    createGeometry();
    glBindVertexArray(handleVAO);
    glGenBuffers(NUM_BUFFERS, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, NumVertices*sizeof(SVertex), Vertices, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_VERTEX_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[COLORS]);
    glBufferData(GL_ARRAY_BUFFER, NumVertices*sizeof(SVertex), Colors, GL_STATIC_DRAW);
    glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, NumVertices*sizeof(SVertex), Normals, GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices*sizeof(GLint), Indices,   GL_STATIC_DRAW);
}

int setupVBOfromObj(char *objFileName){
	int objError;
  	// wczytanie obiektu z pliku .obj i przygotowanie go
	if ( (objError = obj.readFromFile(objFileName)) )
		return objError;
	//obj.writeProps();
	// przeskalowanie wczytanego obj, tak aby był wpisany w jednostkowy sześcian
	// o środku w początku układu współrzędnych
	//obj.scale();

	// sprawdzenie czy zostały poprawnie zdefiniowane normalne
	if (!obj.nNormals)
		// wygenerowanie uśrednionych normalnych
		//obj.genSmoothNormals();
		// wygenerowanie normalnych dla ścianek
		obj.genFacesNormals();
	else
		if (!obj.normIndGood)
			// gdy indeksy normalnych nie zgadzają się z indeksami wierzhołków
			// należy przebudować obie tablice, aby były tak samo indeksowane
			// przbudowanie indeksów normalnych i jeśli trzeba indeksów wierchołków
			obj.rebuildAttribTable('n');

	
	
	//obj.writeProps();
	//obj.writeTables();
	
	glGenVertexArrays(1, &handleOBJVAO);
    
	glBindVertexArray(handleOBJVAO);
	glGenBuffers(NUM_BUFFERS, OBJbuffers);

	glBindBuffer(GL_ARRAY_BUFFER, OBJbuffers[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, 3*obj.nAttribs*sizeof(GLfloat), obj.vertices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_VERTEX_ARRAY);

	
	glBindBuffer(GL_ARRAY_BUFFER, OBJbuffers[NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, 3*obj.nAttribs*sizeof(GLfloat), obj.normals, GL_STATIC_DRAW);
	glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));
	glEnableClientState(GL_NORMAL_ARRAY);
    
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OBJbuffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*obj.nFaces*sizeof(GLuint), obj.faces,   GL_STATIC_DRAW);

  return 0;
}

void init()
{
	
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	//aktywujemy światło
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	
	
	//definiujemy kolor światła 
	glLightfv(GL_LIGHT0, GL_AMBIENT, red_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, red_light); // rozproszone
	glLightfv(GL_LIGHT0, GL_SPECULAR, red_light); // skierowane
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

		
	glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white_light);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 15.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 5.0);
	
	// światło kierunkowe i punktowe
	// kierunkowe i posiada tylko kierunek
	// punktowe i jest punktem i świeci na wszystkie strony
	// spot i jest połączeniem tych dwóch powyższych, to jest 
	
	angle = 0.0f;
	ye = 17.0;
	xe = 30.0f;
	ze = 0.0f; 
	
	// kiedys wszystkie obliczenia od swiatla zaleza od OpenGL
	// obecnie zaleza od shaderow
	
	setupShaders((char*)"pointlight.vert", 
		     (char*)"pointlight.frag",
		     &ProgramA, 
		     &vertexShaderObjectA,
		     &fragmentShaderObjectA);
	
	/*setupShaders((char*)"dirLightAmbDiffSpecPix.vert", 
		     (char*)"dirLightAmbDiffSpecPix.frag",
		     &ProgramA, 
		     &vertexShaderObjectA,
		     &fragmentShaderObjectA);*/
	
	/*setupShaders((char*)"spotlight.vert", 
		     (char*)"spotlight.frag",
		     &ProgramA, 
		     &vertexShaderObjectA,
		     &fragmentShaderObjectA);*/
	setupVBO();
	setupVBOfromObj((char*)"monkey.obj");
	
	
	
	glUseProgram(ProgramA);
}







void animate(int v)
{
  rotLight+=5;
  rotLight= rotLight  % 360;
  glutPostRedisplay();
  glutTimerFunc(50, animate, 0);
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
	
	gluLookAt(xe, ye, ze, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	 GLint var1 = glGetUniformLocation(ProgramA, "numLights");
	 glUniform1i(var1, 3);
    
	 //ustawienie własności materiałów
	 glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_mat);
	 glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_mat);
	 glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_mat);
	 glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60);
	 
	 // mozna zdefiniowac, w jaki sposob nastepuje odbicie swiatla od powierzchni
	 // mozna zdefiniowac sposoby odbicia dla roznych zrodel swiatla
	 //
	
	glPushMatrix();
	  glTranslatef(0.0f, 15.0f, 0.0f);
	  glRotatef((GLfloat)rotLight, 0.0, 1.0, 0.0);
	  glTranslatef(45.0f, 0.0f, 0.0f);
	  //lokalizacja światła 0
	  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);	  
	glPopMatrix();
	
	glPushMatrix();
	  glTranslatef(00.0f, 45.0f, 0.0f);
	  //lokalizacja światłą 1
	  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);	  
	glPopMatrix();
	
	
        glPushMatrix();
	  glTranslatef(0.0f, 25.0f, 0.0f);
	  glLightfv(GL_LIGHT2, GL_POSITION, light2_position);	
	  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
        glPopMatrix();
	
	
	

	
	
	glBindVertexArray(handleVAO);
        glDrawElements(GL_QUADS, //mode
                    NumIndices,  //count, ie. how many indices
                    GL_UNSIGNED_INT, //type of the index array
                    BUFFER_OFFSET(0));
	
	
	
	
	glPushMatrix();
	   glTranslatef(0.0f, 10.0f, 0.0f);
	   glScalef(5.0f, 5.0f, 5.0f);
	   glBindVertexArray(handleOBJVAO);
	   // narysowanie danych zawartych w tablicy wierzchołków .obj
	   glDrawElements(GL_TRIANGLES, 3*obj.nFaces, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glPopMatrix();
	
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
	glutTimerFunc(1000, animate, 0);
	glutMainLoop();
	
	delete [] Vertices;
	delete [] Colors;
	delete [] Indices;
	
	return 0;
}
