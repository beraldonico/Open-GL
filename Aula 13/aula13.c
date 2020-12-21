
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <CImg.h>

#include "plylib.h"

GLint g_programId = 0;
GLuint g_modelViewId = 0;
GLuint g_projectionId = 0;
GLuint g_frameBufferName = 0;
ply_model *g_model = NULL;

#define DEFAULT_WINDOW_WIDTH	800
#define DEFAULT_WINDOW_HEIGHT	600
#define DEFAULT_SHADOW_MAP_SIDE	1024

typedef union {
	struct {
		GLfloat _x, _y, _z, _w;
	};
	GLfloat _v[4];
} Vector4f;

Vector4f make_Vector4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	Vector4f v;
	v._x = x;
	v._y = y;
	v._z = z;
	v._w = w;
	return v;
}

Vector4f make_Vector4f(GLfloat x, GLfloat y, GLfloat z) {
	return make_Vector4f(x, y, z, 1.0f);
}

typedef struct {
	Vector4f _position;

	Vector4f _ambient;
	Vector4f _diffuse;
	Vector4f _specular;
} Light;

typedef struct {
	Vector4f _ambient;
	Vector4f _diffuse;
	Vector4f _specular;
	GLfloat  _shininess;
} Material;

typedef struct {
	Vector4f _eye;
	Vector4f _center;
	Vector4f _up;

	GLfloat _fovy;
	GLfloat _aspect;
	GLfloat _zNear;
	GLfloat _zFar;
} Camera;

typedef union {
	struct {
		GLfloat v00, v01, v02, v03;
		GLfloat v10, v11, v12, v13;
		GLfloat v20, v21, v22, v23;
		GLfloat v30, v31, v32, v33;
	};
	GLfloat m[16];
} Matrix4x4;

#define MAX_MATRIX_LIST_SIZE 10
Matrix4x4 g_matrixList[MAX_MATRIX_LIST_SIZE][2];
GLuint g_matrixListIndex;
GLint g_pass;

void performMatrixListTask() {
	if (g_matrixListIndex < MAX_MATRIX_LIST_SIZE-1) {
		if (g_pass == 0) {
			glGetFloatv(GL_MODELVIEW_MATRIX,g_matrixList[g_matrixListIndex][0].m);
			glGetFloatv(GL_PROJECTION_MATRIX,g_matrixList[g_matrixListIndex][1].m);
		} else {
			glUniformMatrix4fv(g_modelViewId,1,false, g_matrixList[g_matrixListIndex][0].m);
			glUniformMatrix4fv(g_projectionId,1,false, g_matrixList[g_matrixListIndex][1].m);
		}
		g_matrixListIndex++;
	} else {
		fprintf(stderr,"Increase matrix list size\n");
	}
}

Camera g_camera;
Light g_light;
Material g_material[2];

GLfloat rotAngle = 0.0f;
GLfloat rotSpeed = 1.0f;
GLfloat lx=0.0f,lz=-10.0f;
GLfloat x=0.0f, z=10.0f;
GLfloat deltaAngle = 0.0f;
GLfloat deltaMove = 0;
GLint xOrigin = -1;

//#define WITH_TEXTURE
#ifdef WITH_TEXTURE
GLuint texName;
#endif
GLuint colorBufferTexture;
GLuint depthBufferTexture;
GLuint g_shadowTexId;

GLuint g_windowWidth	= DEFAULT_WINDOW_WIDTH;
GLuint g_windowHeight	= DEFAULT_WINDOW_HEIGHT;
GLuint g_shadowMapSide	= DEFAULT_SHADOW_MAP_SIDE;

GLint loadShader(GLuint shaderId, const char *path) {
	FILE *file = fopen(path,"r");
	if (file == NULL) {
		fprintf(stderr, "Couldn't open file %s for reading\n", path);
		return GL_FALSE;
	}

	int codeBufferSize = 4096;
	int codeSize = 0;
	char *code = (char*)malloc(codeBufferSize * sizeof(char));
	if (code == NULL) {
		fprintf(stderr, "Failed to request %d bytes of memory\n", codeBufferSize);
		return GL_FALSE;
	}

	int rl = 256;
	int r = 0;
	int i = 0;
	while ((r = fread(&code[i], 1, rl, file)) > 0) {
		i += r;
		if ((i + rl) > (codeBufferSize - 1)) {
			codeBufferSize *= 2;
			char *newCode = (char*)malloc(codeBufferSize * sizeof(char));
			if (newCode == NULL) {
				fprintf(stderr, "Failed to request %d bytes of memory\n", codeBufferSize);
				free(code);
				return GL_FALSE;
			}

			for (int j = 0; j < i; j++) {
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
	if ( infoLogLength > 0 ){
		char msg[4096];
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, msg);
		printf("%s\n", msg);
	}

}

GLuint loadShaders(const char *vsPath, const char *fsPath){

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
	if ( infoLogLength > 0 ){
		char msg[4096];
		glGetProgramInfoLog(progId, infoLogLength, NULL, msg);
		printf("%s\n", msg);
	}

	glDetachShader(progId, vsId);
	glDetachShader(progId, fsId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);

	glUseProgram(progId);

	g_shadowTexId = glGetUniformLocation(progId, "shadowTexture");
	g_modelViewId = glGetUniformLocation(progId, "modelview");
	g_projectionId = glGetUniformLocation(progId, "projection");

	glUseProgram(0);

	return progId;
}

void loadTexture(GLuint *texId, const char* imgPath) {

	// textures
	cimg_library::CImg<unsigned char> image(imgPath);
	GLubyte *imgBuffer = (GLubyte*)malloc(image.width() * image.height() * 4 * sizeof(GLubyte));
	cimg_forXY(image,x,y) {
		imgBuffer[(image.width() * y + x) * 4 + 0] = image(x,y,0);
		imgBuffer[(image.width() * y + x) * 4 + 1] = image(x,y,1);
		imgBuffer[(image.width() * y + x) * 4 + 2] = image(x,y,2);
		imgBuffer[(image.width() * y + x) * 4 + 3] = 1.0;
	};
	GLsizei width = image.width();
	GLsizei height = image.height();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, texId);
	glBindTexture(GL_TEXTURE_2D, *texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
				height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				imgBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(imgBuffer);
}

void loadFrameBuffer(int width, int height) {

	glGenFramebuffers(1, &g_frameBufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, g_frameBufferName);

	glGenTextures(1, &colorBufferTexture);

	glBindTexture(GL_TEXTURE_2D, colorBufferTexture);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &depthBufferTexture);

	glBindTexture(GL_TEXTURE_2D, depthBufferTexture);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBufferTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferTexture, 0);

	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("FramebufferStatus error\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void resizeFrameBuffer(int width, int height) {

	glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBufferTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferTexture, 0);

	// Set the list of draw buffers.
	//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("FramebufferStatus error\n");

}

void setLight(GLenum lightNum, Light *light) {
	glLightfv(lightNum, GL_AMBIENT, light->_ambient._v);
	glLightfv(lightNum, GL_DIFFUSE, light->_diffuse._v);
	glLightfv(lightNum, GL_SPECULAR, light->_specular._v);
	glLightfv(lightNum, GL_POSITION, light->_position._v);
}

void setMaterial(GLenum face, Material *material) {
	glMaterialfv(face,GL_AMBIENT, material->_ambient._v);
	glMaterialfv(face,GL_DIFFUSE, material->_diffuse._v);
	glMaterialfv(face,GL_SPECULAR, material->_specular._v);
	glMateriali(face,GL_SHININESS, material->_shininess);
}

// Inicializa parâmetros de rendering
void init (void)
{
	g_camera._eye		= make_Vector4f(0.0f,5.0f,15.0f);
	g_camera._center	= make_Vector4f(0.0f,0.0f,0.0f);
	g_camera._up		= make_Vector4f(0.0f,1.0f,0.0f);

	g_camera._fovy		= 45.0f;
	g_camera._aspect	= (GLfloat)g_windowWidth/(GLfloat)g_windowHeight;
	g_camera._zNear		= 5.0f;
	g_camera._zFar		= 25.0f;

	g_light._position	= make_Vector4f(-15.0f, 10.0f, 0.0f);
	g_light._ambient	= make_Vector4f(0.0f,0.0f,0.0f,1.0f);
	g_light._diffuse	= make_Vector4f(1.0f,1.0f,1.0f,1.0f);
	g_light._specular	= make_Vector4f(1.0f,1.0f,1.0f,1.0f);

	g_material[0]._ambient	= make_Vector4f(1.0f,1.0f,1.0f,1.0f);
	g_material[0]._diffuse	= make_Vector4f(0.0f,0.2f,0.6f,1.0f);
	g_material[0]._specular	= make_Vector4f(0.6f,0.6f,0.6f,1.0f);
	g_material[0]._shininess = 50.0f;

	g_material[1]._ambient	= make_Vector4f(1.0f,1.0f,1.0f,1.0f);
	g_material[1]._diffuse	= make_Vector4f(0.8f,0.8f,0.6f,1.0f);
	g_material[1]._specular	= make_Vector4f(0.6f,0.6f,0.6f,1.0f);
	g_material[1]._shininess = 3.0f;

	glShadeModel(GL_SMOOTH);

	setMaterial(GL_FRONT, &g_material[0]);
	setLight(GL_LIGHT0, &g_light);

	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	#ifdef WITH_TEXTURE
	loadTexture(&texName, "brick1Colors.jpg");
        #endif

	loadFrameBuffer(g_shadowMapSide, g_shadowMapSide);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void drawTexture(GLuint tex) {
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glColor3f(1.0f, 1.0f, 1.0f);

	glDisable(GL_LIGHTING);

	GLfloat h = 4.0f;
	GLfloat w = h;

	glBegin(GL_TRIANGLE_STRIP);

	glNormal3f(0.0f,0.0f,1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-w,-h,0.0f);

	glTexCoord2f(1.0, 0.0);
	glVertex3f( w,-h,0.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-w, h,0.0f);

	glTexCoord2f(1.0, 1.0);
	glVertex3f( w, h,0.0f);

	glEnd();

	glEnable(GL_LIGHTING);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
}

#ifdef WITH_TEXTURE
void drawTextured() {
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texName);

	glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0.0,0.0,1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -2.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(2.0, -2.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 2.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 2.0, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
#endif

#ifdef WITH_TEXTURE
void drawTexturedSphere() {

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texName);

	GLUquadric *quad = gluNewQuadric();
	gluQuadricDrawStyle( quad, GLU_FILL);
	gluQuadricNormals( quad, GLU_SMOOTH);
	gluQuadricOrientation( quad, GLU_OUTSIDE);
	gluQuadricTexture( quad, GL_TRUE);
	//glRotatef(90.0,1.0,0.0,0.0);
	//gluSphere(quad, 2.0f, 36, 24);
	glutSolidTeapot(2.0f);

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
}
#endif

void drawWall(void) {

	glColor3f(1.0f, 1.0f, 1.0f);

	GLfloat side   = 5.0f;
	GLfloat height = -2.0f;
	GLfloat width  = 0.2f;

	performMatrixListTask();

	glBegin(GL_TRIANGLE_STRIP);

		glNormal3f(0.0f, 1.0f, 0.0f);

		glVertex3f(-side, height, -side);
		glVertex3f(-side, height,  side);
		glVertex3f( side, height, -side);
		glVertex3f( side, height,  side);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f( side, height+1.25*side, -side);
		glVertex3f( side, height+1.25*side,  side);

	glEnd();

}

void drawScene(void) {

	//gluLookAt(0,0,10, 0,0,0, 0,1,0);

	rotAngle += rotSpeed;
	if (rotAngle > 360.0f) rotAngle -= 360.0f;

	//glTranslatef(-3.0f,0.0f,0.0f);

	glPushMatrix();
		glRotatef(rotAngle,0.0f,1.0f,0.0f);

		//glutSolidTeapot(2.0f);
		//glutSolidSphere(2.0f, 60, 40);
		//drawTextured();
		performMatrixListTask();
		//drawTexturedSphere();
		setMaterial(GL_FRONT, &g_material[0]);
//		/glutSolidCylinder(1.0,3.0,12,24);

                if (g_model != NULL) {
                  int i;
                  glPushMatrix();
                  glTranslatef(0.0,-1.1,0.0);
                  glScalef(20.0,20.0,20.0);
		  performMatrixListTask();
                  glBegin(GL_TRIANGLES);
                  for (i=0;i<g_model->nTriangles;i++) {
                    glNormal3f(g_model->triangles[i].normal.x, g_model->triangles[i].normal.y, g_model->triangles[i].normal.z);
                    glVertex3f(g_model->vertices[g_model->triangles[i].a].x,g_model->vertices[g_model->triangles[i].a].y,g_model->vertices[g_model->triangles[i].a].z);
                    glVertex3f(g_model->vertices[g_model->triangles[i].b].x,g_model->vertices[g_model->triangles[i].b].y,g_model->vertices[g_model->triangles[i].b].z);
                    glVertex3f(g_model->vertices[g_model->triangles[i].c].x,g_model->vertices[g_model->triangles[i].c].y,g_model->vertices[g_model->triangles[i].c].z);
                  }
                  glEnd();
                  glPopMatrix();
                } else {

		  glutSolidTorus(0.3,1.2,12,36);
                }
//		/glutSolidSphere(2.0,36,12);
		//glutSolidTeapot(2.0f);
	glPopMatrix();

	setMaterial(GL_FRONT, &g_material[1]);
	drawWall();
}


void draw(void)
{
	g_matrixListIndex = 0;
	g_pass = 0;

	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, g_frameBufferName);
	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f,1.0f,15.0f,30.0f);
	//glOrtho(-10.0f,10.0f,-10.0f,10.0f,15.0f,30.0f);

	glViewport(0,0,g_shadowMapSide,g_shadowMapSide);

	glClearColor(0.0f,0.2f,0.6f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	g_light._position._x, g_light._position._y, g_light._position._z,
				g_camera._center._x, g_camera._center._y,  g_camera._center._z,
				g_camera._up._x, g_camera._up._y, g_camera._up._z);

	setLight(GL_LIGHT0, &g_light);

	drawScene();

	g_matrixListIndex = 0;
	g_pass = 1;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(g_programId);
	//glUseProgram(0);

	glUniform1i(g_shadowTexId, 0);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
	//glBindTexture(GL_TEXTURE_2D, colorBufferTexture);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-5.0,5.0,-5.0,5.0,0.4,500);
	gluPerspective(g_camera._fovy,g_camera._aspect,g_camera._zNear,g_camera._zFar);

	// Render to the screen
	glViewport(0,0,g_windowWidth,g_windowHeight);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(g_camera._eye._x, g_camera._eye._y, g_camera._eye._z,
			  g_camera._center._x, g_camera._center._y, g_camera._center._z,
			  g_camera._up._x, g_camera._up._y,  g_camera._up._z);

	setLight(GL_LIGHT0, &g_light);

	drawScene();
	//drawTexture(depthBufferTexture);
	//glRotatef(rotAngle,0.0f,1.0f,0.0f);
	//drawTexture(colorBufferTexture);

	glutSwapBuffers();
}

// Função callback chamada quando o tamanho da janela é alterado
void resize(GLsizei width, GLsizei height)
{
	g_windowWidth = width;
	g_windowHeight = height;

	// Para previnir uma divisão por zero
	if ( g_windowHeight == 0 ) g_windowHeight = 1;

	// Calcula a correção de aspecto
	g_camera._aspect = (GLfloat)g_windowWidth/(GLfloat)g_windowHeight;
}

// Função callback chamada para gerenciar eventos do mouse
void mouse(int button, int state, int x, int y)
{
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			rotAngle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void motion(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.01f;

		// update camera's direction
		lx = sin(rotAngle + deltaAngle) * 10.0;
		lz = -cos(rotAngle + deltaAngle) * 10.0;
	}
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(33,timer, 1);
}

void initShaders() {
	GLenum err = glewInit();
	if (GLEW_OK != err)
	  fprintf(stderr, "%s\n", glewGetErrorString(err));
//	/printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	g_programId = loadShaders("vertex.glsl","fragment.glsl");

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(g_windowWidth, g_windowHeight);
	glutCreateWindow("Shadow Mapping");
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(33,timer,1);

        g_model = load_ply_model(argv[1]);

	initShaders();
	init();

	glutMainLoop();
}

