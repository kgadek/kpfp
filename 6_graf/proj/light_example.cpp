#include <cstdio>
#include <cmath>
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include "obj.h"


#define VERTICES 0
#define COLORS 1
#define NORMALS 2
#define INDICES 3
#define NUM_BUFFERS 4
#define BUFFER_OFFSET(bytes)((GLubyte*) NULL +(bytes))

typedef struct {
    GLfloat x,y,z;
} SVertex;

GLuint handleVAO;
GLuint handleOBJVAO;
GLuint OBJbuffers[NUM_BUFFERS];
GLuint buffers[NUM_BUFFERS];
GLint NumVertices;
GLint NumIndices;
SVertex *Vertices;
SVertex *Colors;
SVertex *Normals;
GLuint* Indices;

GLfloat angle, zoom, ye, ze, xe;

GLfloat pos[4]   = {25.0 ,  35.0 ,  -2.0 ,  1.0};
GLfloat red[4]   = {0.9  ,  0.0  ,  0.0  ,  1.0};
GLfloat green[4] = {0.1  ,  0.9  ,  0.2  ,  1.0};
GLfloat blue[4]  = {0.3  ,  0.3  ,  1.0  ,  1.0};
GLfloat white[4] = {1.0f ,  1.0f ,  1.0f ,  1.0f};

GLfloat light0_position[]  = {1.0 ,  0.0  ,  0.0 ,  0.0};  //światło kierunkowe
GLfloat light1_position[]  = {0.0 ,  0.0  ,  0.0 ,  1.0};  //światło punktowe
GLfloat light2_position[]  = {0.0 ,  0.0  ,  0.0 ,  1.0};  //światło typu spot
GLfloat light2_direction[] = {0.0 ,  -1.0 ,  0.0 ,  1.0}; //kierunek światła typu spot
GLint rotLight = 0;

GLfloat white_light[] = {0.2 ,  0.2 ,  0.2 ,  1.0};
GLfloat red_light[]   = {0.8 ,  0.5 ,  0.6 ,  1.0};
GLfloat blue_light[]  = {0.0 ,  0.0 ,  1.0 ,  1.0};

GLfloat ambient_mat[]  = {0.2 ,  0.2 ,  0.2 ,  1.0};
GLfloat specular_mat[] = {0.5 ,  0.5 ,  0.5 ,  1.0};
GLfloat diffuse_mat[]  = {0.2 ,  1.0 ,  0.2 ,  1.0};

objShape obj;

GLuint vertexShaderObjectA, fragmentShaderObjectA;
GLuint ProgramA;

GLfloat revrot = 0.0f;


unsigned long getFileLength(ifstream& file) {
    if(!file.good())
        return 0;
    unsigned long len;
    file.seekg(0,ios::end);
    len = file.tellg();
    file.seekg(ios::beg);
    return len;
}

int loadShader(char* filename, GLchar** ShaderSource, int * len) {
    ifstream file;
    printf("Shader file: %s\n", filename);
    file.clear();
    file.open(filename, ios::binary);
    if(!file)
        return -1;

    (*len) = getFileLength(file);

    if((*len)==0)
        return -2;

    if(((*ShaderSource) = new char[(*len)+1]) == 0)
        return -3;
    (*ShaderSource)[(*len)] = 0;

    unsigned int i=0;
    while(file.good()) {
        (*ShaderSource)[i] = file.get();
        if(!file.eof())
            i++;
    }
    (*ShaderSource)[i] = 0;

    file.close();
    return 0;
}


int unloadshader(GLubyte** ShaderSource) {
    delete[] *ShaderSource;
    *ShaderSource = 0;
    return 0;
}


void setupShaders(
        char* vertexfile,
        char* fragmentfile, 
        GLuint *Program,
        GLuint *vertexShaderObject,
        GLuint *fragmentShaderObject) {

    glewInit();

    *vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    *fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    GLchar *vertexShaderSource   = 0,
           *fragmentShaderSource = 0;
    GLint vlength                = 0,
          flength                = 0;

    loadShader((char*)vertexfile, &vertexShaderSource, &vlength);
    loadShader((char*)fragmentfile, &fragmentShaderSource, &flength);


    glShaderSource(*vertexShaderObject, 1, (const GLchar**) &vertexShaderSource, &vlength); 
    glShaderSource(*fragmentShaderObject, 1, (const GLchar**) &fragmentShaderSource, &flength); 

    glCompileShader(*vertexShaderObject);
    glCompileShader(*fragmentShaderObject);

    GLint compiled=0; 
    glGetShaderiv(*vertexShaderObject, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint len;
        GLchar *log;
        glGetShaderiv(*vertexShaderObject, GL_INFO_LOG_LENGTH, &len);
        log =(GLchar*) malloc(len);

        glGetShaderInfoLog(*vertexShaderObject, len, &len, log);
        fprintf(stderr, "vertex shader error log: %s\n", log);
        free(log);
    }

    glGetShaderiv(*fragmentShaderObject, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint len;
        GLchar *log;
        glGetShaderiv(*fragmentShaderObject, GL_INFO_LOG_LENGTH, &len);
        log =(GLchar*) malloc(len);

        glGetShaderInfoLog(*fragmentShaderObject, len, &len, log);
        fprintf(stderr, "fragment shader error log: %s\n", log);
        free(log);
    }

    *Program = glCreateProgram();

    glAttachShader(*Program, *vertexShaderObject);
    glAttachShader(*Program, *fragmentShaderObject);

    glLinkProgram(*Program);

    GLint linked = 0;
    glGetProgramiv(*Program, GL_LINK_STATUS, &linked);
    if(!linked) {
        GLint len;
        GLchar *log;
        glGetProgramiv(*Program, GL_INFO_LOG_LENGTH, &len);
        log =(GLchar*) malloc(len);

        glGetProgramInfoLog(*Program, len, &len, log);
        fprintf(stderr, "liner error log: %s\n", log);
        free(log);
    }
}


void createGeometry() {
    NumVertices = 4;
    Vertices    = new SVertex[NumVertices];
    Colors      = new SVertex[NumVertices];
    Normals     = new SVertex[NumVertices];

    Vertices[0].x = 5.0f  ; Vertices[0].y = 0.0f ; Vertices[0].z = 5.0f  ;
    Vertices[1].x = 5.0f  ; Vertices[1].y = 0.0f ; Vertices[1].z = -5.0f ;
    Vertices[2].x = -5.0f ; Vertices[2].y = 0.0f ; Vertices[2].z = -5.0f ;
    Vertices[3].x = -5.0f ; Vertices[3].y = 0.0f ; Vertices[3].z = 5.0f  ;

    Colors[0].x = 1.0f    ; Colors[0].y = 0.0f   ; Colors[0].z = 0.0f    ;
    Colors[1].x = 0.0f    ; Colors[1].y = 1.0f   ; Colors[1].z = 0.0f    ;
    Colors[2].x = 0.0f    ; Colors[2].y = 0.0f   ; Colors[2].z = 1.0f    ;
    Colors[3].x = 1.0f    ; Colors[3].y = 1.0f   ; Colors[3].z = 1.0f    ;

    Normals[0].x = 0.0f   ; Normals[0].y = 1.0f  ; Normals[0].z = 0.0f   ;
    Normals[1].x = 0.0f   ; Normals[1].y = 1.0f  ; Normals[1].z = 0.0f   ;
    Normals[2].x = 0.0f   ; Normals[2].y = 1.0f  ; Normals[2].z = 0.0f   ;
    Normals[3].x = 0.0f   ; Normals[3].y = 1.0f  ; Normals[3].z = 0.0f   ;

    NumIndices = 5;
    Indices = new GLuint[NumIndices];

    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 3;
    Indices[4] = 0;
}


void setupVBO() {
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
    if((objError = obj.readFromFile(objFileName)))
        return objError;
    obj.scale();

    if(!obj.nNormals)
        obj.genFacesNormals();
    else if(!obj.normIndGood)
        obj.rebuildAttribTable('n');

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

void init() {

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);


    glLightfv(GL_LIGHT0, GL_AMBIENT, red_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, red_light);

    glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

    glLightfv(GL_LIGHT2, GL_AMBIENT, white_light);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white_light);

    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 5.0);

    angle = 0.0f;
    ye    = 17.0;
    xe    = 30.0f;
    ze    = 0.0f;

    setupShaders(
            (char*)"pointlight.vert", 
            (char*)"pointlight.frag",
            &ProgramA, 
            &vertexShaderObjectA,
            &fragmentShaderObjectA);
    setupShaders(
            (char*)"dirLightAmbDiffSpecPix.vert", 
            (char*)"dirLightAmbDiffSpecPix.frag",
            &ProgramA, 
            &vertexShaderObjectA,
            &fragmentShaderObjectA);
    setupVBO();
    setupVBOfromObj((char*)"rev2.obj");
    glUseProgram(ProgramA);
}

void animate(int v) {
    rotLight += 7;
    rotLight = rotLight  % 360;
    glutPostRedisplay();
    glutTimerFunc(50, animate, 0);
}


void reshape(int w, int h) {
    glViewport(0, 0,(GLsizei) w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLfloat) w/(GLfloat) h, 1.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
};



void display() {
    glClearColor(0.2f,0.2f,0.2f,1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(xe, ye, ze, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    GLint var1 = glGetUniformLocation(ProgramA, "numLights");
    glUniform1i(var1, 3);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_mat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60);

    glPushMatrix();
    glTranslatef(0.0f, 15.0f, 0.0f);
    glRotatef((GLfloat)rotLight, 0.0, 1.0, 0.0);
    glTranslatef(45.0f, 0.0f, 0.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);	  
    glPopMatrix();

    glPushMatrix();
    glTranslatef(00.0f, 45.0f, 0.0f);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);	  
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 25.0f, 0.0f);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);	
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);
    glPopMatrix();

    glBindVertexArray(handleVAO);

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(0.0f, 0.0f, 0.0f, 0.0f);
    glScalef(25.0f, 25.0f, 25.0f);
    glBindVertexArray(handleOBJVAO);
    glDrawElements(GL_TRIANGLES, 3*obj.nFaces, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glPopMatrix();

    glFinish();
    glutSwapBuffers();
}


void keybs(int skey, int x, int y ) {
    switch(skey) {
        case GLUT_KEY_LEFT:
            xe -= 1.0f;
            break;
        case GLUT_KEY_RIGHT:
            xe += 1.0f;
            break;
        case GLUT_KEY_UP:
            ye -= 1.0f;
            break;
        case GLUT_KEY_DOWN:
            ye += 1.0f;
            break;
        case GLUT_KEY_PAGE_UP:
            ze+= 1.0f;
            break;
        case GLUT_KEY_PAGE_DOWN:
            ze-= 1.0f;
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

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keybs);
    glutTimerFunc(50, animate, 0);
    glutMainLoop();

    delete [] Vertices;
    delete [] Colors;
    delete [] Indices;

    return 0;
}

