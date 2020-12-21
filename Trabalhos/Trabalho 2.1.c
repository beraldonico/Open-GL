/*
MOUSE
    RIGHT start/ stop
    LEFT 
    MIDDLE restart zoom
    Scrool zoom in and out
KEY
    LEFT    |
    UP      | Move camera
    DOWN    |
    RIGHT   |
    F1 increase speed
    F2 decrease speed
    F3 reset speed
*/
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

GLint rot = 0;
static GLfloat spin = 0.0;
GLfloat velo = 1.0;
GLint width, height;
GLdouble move_cx = 0.0;
GLdouble move_cy = 0.0;
GLdouble zoom = 0.1;
GLfloat fovy = 25.0;
GLint programId;
/*
GLdouble move_rx = 0.0;
GLdouble move_ry = 0.0;
GLdouble move_rot= 0.0;
*/

GLuint loadShaders(const char *vsPath, const char *fsPath);
GLint loadShader(GLuint shaderId, const char *path);

void display(void)
{
    if (rot)
    {
        spin += velo;
        if (spin > 360.0)
            spin -= 360.0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programId);

    glPushMatrix();
    
    //glRotatef(move_rot,move_rx,move_ry,0.0);    adaptar right botton
    //modulo do vetor e decompor em x e y

	//cores
    GLfloat vermelho[4] = {1.0f, .0f, 0.0f, 1.0f}; 
    GLfloat branco[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat verde[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    
    //nucleo
    //4 Protons
   	glMaterialfv(GL_FRONT, GL_AMBIENT, vermelho);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, vermelho);
    glMaterialfv(GL_FRONT, GL_SPECULAR, branco);
   	//
    glPushMatrix();
    glTranslatef(0.0,2.0,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();
	//
    glPushMatrix();
    glTranslatef(0.0,-2.0,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();
	//
    glPushMatrix();
    glTranslatef(2.0,0.0,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();
	//
    glPushMatrix();
    glTranslatef(-2.0,0.0,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();


    //5 Neutrons
   	glMaterialfv(GL_FRONT, GL_AMBIENT, branco);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, branco);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, branco);
   	//
    glPushMatrix();
    glTranslatef(0.0,0.0,0.0);
    glutSolidSphere(3.5, 25.0, 25.0);
    glPopMatrix();
    //
    glPushMatrix(); 
    glTranslatef(1.20,1.20,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();
	//
    glPushMatrix();
    glTranslatef(-1.20,-1.20,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();
	//
    glPushMatrix();
    glTranslatef(-1.20,1.20,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();
	//
    glPushMatrix();
    glTranslatef(1.20,-1.20,0.0);
    glutSolidSphere(2.5, 25.0, 25.0);
    glPopMatrix();


    //eletron 1 horinzontal
   	glMaterialfv(GL_FRONT, GL_AMBIENT, verde);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, verde);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, branco);
    glPushMatrix();
    glRotatef(spin, 0.0, 1.0, 0.0);
    //
    glPushMatrix();
    glTranslatef(40.0,0.0,0.0);
    //glColor3f(0.0,1.0,0.0);
    glutSolidSphere(2.5, 50.0, 50.0);
    glPopMatrix();
    //
    glPopMatrix();
	//
    //eletron 2 vertical
    glPushMatrix();
    glRotatef(spin, 1.0, 0.0, 0.0);
    //
    glPushMatrix();
    glTranslatef(0.0,40.0,0.0);
    //glColor3f(0.0,1.0,0.0);
    glutSolidSphere(2.5, 50.0, 50.0);
    glPopMatrix();
    //
    glPopMatrix();
	//
    //eletron 3 diagonal
    glPushMatrix();
    glRotatef(spin, 1.0, 1.0, 0.0);
    //
    glPushMatrix();
    glTranslatef(0.0,0.0,40.0);
    glutSolidSphere(2.5, 50.0, 50.0);
    glPopMatrix();
    //
    glPopMatrix();
	//
    //eletron 4 diagonal 2
    glPushMatrix();
    glRotatef(spin, 1.0, -1.0, 0.0);
    //
    glPushMatrix();
    glTranslatef(0.0,0.0,-40.0);
    glutSolidSphere(2.5, 50.0, 50.0);
    glPopMatrix();
    //
    glPopMatrix();
	
	
    //eletron trajeto 1 horinzontal
   	glMaterialfv(GL_FRONT, GL_AMBIENT, verde);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, verde);
    glPushMatrix();  
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(0.20,40.0,25.0,25.0);
    //
    glPopMatrix();
	//
    //eletron trajeto 2 vertical
    glPushMatrix(); 
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glutSolidTorus(0.20,40.0,25.0,25.0);
    //
    glPopMatrix();
	//
    //eletron trajeto 3 diagonal
    glPushMatrix();
    glRotatef(90.0,1.0,1.0,0.0);
    glutSolidTorus(0.2,40.0,25.0,25.0);
    //
    glPopMatrix();
	//
    //eletron trajeto 4 diagonal
    glPushMatrix();  
    glRotatef(90.0,1.0,-1.0,0.0);
    glutSolidTorus(0.2,40.0,25.0,25.0);
    //
    glPopMatrix();

    glPopMatrix();

    glUseProgram(0);

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    //glShadeModel (GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float position[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float ambient[4]  = {0.2f, 0.2f, 0.2f, 1.0f};
    float diffuse[4]  = {0.7f, 0.7f, 0.7f, 1.0f};
    float specular[4] = {0.0f, 0.5f, 0.5f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0);
}

void initShaders()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
        fprintf(stderr, "%s\n", glewGetErrorString(err));

    programId = loadShaders("vertex.glsl","fragment.glsl");
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(fovy, (float)w/(float)h, 200.0, 400.0);
    glOrtho(-w*zoom, w*zoom, -h*zoom, h*zoom, -10000.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0.0,0.0,-300.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
	gluLookAt(move_cx,move_cy,-1.0, 0.0,0.0,0.0, 0.0,1.0,0.0);
	
	//float position[4] = {move_cx, move_cy, 0.0f, 0.0f};
	//glLightfv(GL_LIGHT0, GL_POSITION, position);

    //printf("%d %d\n", w, h);
}

void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            if(rot == 1)
            {
                rot = 0;
            }
            else
            {
                rot = 1;
            }
            printf("LEFT BUTTON\n");
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN)
        {
            //fovy = 25.0;
            zoom = 0.1;
            reshape(width, height);
            printf("MIDDLE BUTTON\n");
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            //mexer camera
             printf("RIGHT BUTTON\n");
        }
        break;
    case 3:
        //fovy *= 0.99;
        zoom *= 0.99;
        reshape(width, height);
             printf("ZOOM IN\n");
        break;
    case 4:
        //fovy *= 1.01;
        //fovy = (fovy >= 100.0)?100.0:fovy;  ??
        if (zoom > 0 )
        {
        	zoom *= 1.01;
        }
        reshape(width, height);
             printf("ZOOM OUT\n");
        break;
    default:
        printf("button: %d\n",button);
        break;
    }
}

void special(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        printf("LEFT KEY\n");
        if( move_cx >= -20.0){
        	move_cx -= 0.25;
        }
        reshape(width, height);
        break;
    case GLUT_KEY_DOWN:
        printf("DOWN KEY\n");
        if( move_cy <= 20.0){
        	move_cy += 0.25;
        }
        reshape(width, height);
        break;
    case GLUT_KEY_RIGHT:
        printf("RIGHT KEY\n");
        if( move_cx <= 20.0){
        	move_cx += 0.25;
        }
        reshape(width, height);
        break;
    case GLUT_KEY_UP:
        printf("UP KEY\n");
        if( move_cy >= -20.0){
        	move_cy -= 0.25;
        }
        reshape(width, height);
        break;
    case GLUT_KEY_F1:
        printf("F1 KEY\n");
        velo -= 0.25;
        break;
    case GLUT_KEY_F2:
        printf("F2 KEY\n");
        velo += 0.25;
        break;
    case GLUT_KEY_F3:
        printf("F3 KEY\n");
        velo = 1.0;
        break;
    default:
        //so printa keys especiais
        printf("KEY: %d\n",key);
        break;
    }
}

void motion(int x, int y)
{
    //printf("Motion: %d %d\n", x, y);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH |GLUT_RGB);
    glutInitWindowSize (927, 560);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Trabalho 2");
    initShaders();
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;
}

GLint loadShader(GLuint shaderId, const char *path)
{
    FILE *file = fopen(path,"r");
    if (file == NULL)
    {
        fprintf(stderr, "Couldn't open file %s for reading\n", path);
        return GL_FALSE;
    }
    int codeBufferSize = 4096;
    int codeSize = 0;
    char *code = (char*)malloc(codeBufferSize * sizeof(char));
    if (code == NULL)
    {
        fprintf(stderr, "Failed to request %d bytes of memory\n", codeBufferSize);
        return GL_FALSE;
    }
    int rl = 256;
    int r = 0;
    int i = 0;
    while ((r = fread(&code[i], 1, rl, file)) > 0)
    {
        i += r;
        if ((i + rl) > (codeBufferSize - 1))
        {
            codeBufferSize *= 2;
            char *newCode = (char*)malloc(codeBufferSize * sizeof(char));
            if (newCode == NULL)
            {
                fprintf(stderr, "Failed to request %d bytes of memory\n", codeBufferSize);
                free(code);
                return GL_FALSE;
            }
            for (int j = 0; j < i; j++)
            {
                code[j] = newCode[j];
            }
            free(code);
            code = newCode;
        }
    }
    code[i] = '\0';
    //printf("%s\n", code);

    const char *src = code;
    glShaderSource(shaderId, 1, &src, NULL);
    glCompileShader(shaderId);

    GLint ret = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &ret);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        char msg[4096];
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, msg);
        printf("%s\n", msg);
    }
}


GLuint loadShaders(const char *vsPath, const char *fsPath)
{
    GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

    loadShader(vsId, vsPath);
    loadShader(fsId, fsPath);

    GLuint progId = glCreateProgram();
    glAttachShader(progId, vsId);
    glAttachShader(progId, fsId);
    glLinkProgram(progId);

    GLint ret = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(progId, GL_LINK_STATUS, &ret);
    glGetProgramiv(progId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if ( infoLogLength > 0 )
    {
        char msg[4096];
        glGetProgramInfoLog(progId, infoLogLength, NULL, msg);
        printf("%s\n", msg);
    }

    glDetachShader(progId, vsId);
    glDetachShader(progId, fsId);

    glDeleteShader(vsId);
    glDeleteShader(fsId);

    glUseProgram(progId);

    GLuint t0 = glGetUniformLocation(progId, "normalTexture");
    GLuint t1 = glGetUniformLocation(progId, "colorTexture");

    glUniform1i(t0, 0);
    glUniform1i(t1, 1);

    glUseProgram(0);

    return progId;
}
