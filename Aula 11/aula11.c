/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  double.c
 *  This is a simple double buffered program.
 *  Pressing the left mouse button rotates the rectangle.
 *  Pressing the middle mouse button stops the rotation.
 */

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#include "plylib.h"

#include <CImg.h>

ply_model *model = NULL;

GLuint g_texId;
GLuint g_normalTexId;
GLuint g_cubeMap;

GLuint loadTexture(const char* imgPath) {
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

  GLuint texId=0;
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_MIRRORED_REPEAT);// GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// GL_MIRRORED_REPEAT);// GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
  			height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			imgBuffer);
  glGenerateMipmap(GL_TEXTURE_2D);

  free(imgBuffer);

  return texId;
}

GLuint loadCubeMapTexture(const char* imgPath) {

	// textures
	cimg_library::CImg<unsigned char> image(imgPath);

	int cubeMapType = 0;
	int squareSide1 = image.width()/4;
	int squareSide2 = image.height()/3;
	if (image.width() < image.height()) {
		cubeMapType = 1;
		squareSide1 = image.width()/3;
		squareSide2 = image.height()/4;
	}

	int squareSide = (squareSide1<squareSide2)?squareSide1:squareSide2;
	if (squareSide1 != squareSide2) {
		printf("Warning: image proportion width x height is not matching a cube map\n");
	}

	GLint topLeftCorner[] = {2,1,
							 0,1,
							 1,0,
							 1,2,
							 1,1,
							 3,1,
							 1,1,
							 1,3,
							 1,0,
							 1,2,
							 0,1,
							 2,1};

	GLubyte *imgBuffer[6];
	for (int i=0; i<6; i++) {
		imgBuffer[i] = (GLubyte*)malloc(squareSide * squareSide * 4 * sizeof(GLubyte));
		for (int x=0;x<squareSide;x++) {
			for (int y=0;y<squareSide;y++) {
				int ix = x;
				int iy = y;
				if (cubeMapType == 1) {
					switch (i) {
					case 1:
						ix = squareSide - x - 1;
						iy = squareSide - y - 1;
						break;
					case 2:
						iy = squareSide - x - 1;
						ix = y;
						break;
					case 3:
						iy = x;
						ix = squareSide - y - 1;
						break;
					default:
						break;
					}
				}

				imgBuffer[i][(squareSide * iy + ix) * 4 + 0] = image(topLeftCorner[cubeMapType * 12 + i * 2 + 0] * squareSide + x,topLeftCorner[cubeMapType * 12 + i * 2 + 1] * squareSide + y,0);
				imgBuffer[i][(squareSide * iy + ix) * 4 + 1] = image(topLeftCorner[cubeMapType * 12 + i * 2 + 0] * squareSide + x,topLeftCorner[cubeMapType * 12 + i * 2 + 1] * squareSide + y,1);
				imgBuffer[i][(squareSide * iy + ix) * 4 + 2] = image(topLeftCorner[cubeMapType * 12 + i * 2 + 0] * squareSide + x,topLeftCorner[cubeMapType * 12 + i * 2 + 1] * squareSide + y,2);
				imgBuffer[i][(squareSide * iy + ix) * 4 + 3] = 255;
			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	for (int i=0; i<6; i++) {

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, squareSide,
					squareSide, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					imgBuffer[i]);

		free(imgBuffer[i]);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        return texId;
}

GLint programId;

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

	GLuint t0 = glGetUniformLocation(progId, "colorTexture");
	GLuint t1 = glGetUniformLocation(progId, "normalTexture");
	GLuint t2 = glGetUniformLocation(progId, "cubemap");

	glUniform1i(t0, 0);
	glUniform1i(t1, 1);
        glUniform1i(t2, 2);

	glUseProgram(0);

	return progId;
}

GLint rot = 0;
static GLfloat spin = 0.0;
GLfloat fovy = 45.0;
GLint width, height;

void display(void)
{
   if (rot) {
     spin = spin + 0.1;
     if (spin > 360.0)
       spin = spin - 360.0;
   }

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glUseProgram(programId);  

   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   glActiveTexture(GL_TEXTURE0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, g_texId);

   glActiveTexture(GL_TEXTURE1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, g_normalTexId);

   glActiveTexture(GL_TEXTURE2);
   glEnable(GL_TEXTURE_CUBE_MAP);
   glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubeMap);

   if (model != NULL) {
     int i;
     glPushMatrix();
     glTranslatef(0.0,-1.1,0.0);
     glScalef(10.0,10.0,10.0);
     glRotatef(spin, 0.0, 1.0, 0.0);
     glBegin(GL_TRIANGLES);
       for (i=0;i<model->nTriangles;i++) {
         glNormal3f(model->triangles[i].normal.x, model->triangles[i].normal.y, model->triangles[i].normal.z);
         glVertex3f(model->vertices[model->triangles[i].a].x,model->vertices[model->triangles[i].a].y,model->vertices[model->triangles[i].a].z);
         glVertex3f(model->vertices[model->triangles[i].b].x,model->vertices[model->triangles[i].b].y,model->vertices[model->triangles[i].b].z);
         glVertex3f(model->vertices[model->triangles[i].c].x,model->vertices[model->triangles[i].c].y,model->vertices[model->triangles[i].c].z);
       }
     glEnd();

     glPopMatrix();
   } else {

     glPushMatrix();

     glRotatef(spin, 0.0, 1.0, 0.0);
     glColor3f(1.0,1.0,1.0);
     //glFrontFace(GL_CW);
     //glutSolidTeapot(1.0);
     //glFrontFace(GL_CCW);
     glutSolidSphere(1.0,36,24);

     glPopMatrix();
   }

   glUseProgram(0);

   glActiveTexture(GL_TEXTURE0);
   glDisable(GL_TEXTURE_2D);

   glActiveTexture(GL_TEXTURE1);
   glDisable(GL_TEXTURE_2D);

   glActiveTexture(GL_TEXTURE2);
   glDisable(GL_TEXTURE_CUBE_MAP);

   glutSwapBuffers();
   
   glutPostRedisplay();
}

void init(void) 
{
   //g_texId = loadTexture("color.jpg");
   //g_normalTexId = loadTexture("normal.jpg");
   g_texId = loadTexture("brick2Colors.png");
   g_normalTexId = loadTexture("brick2Normals.png");
   //g_cubeMap = loadCubeMapTexture("cubemap.png");
   g_cubeMap = loadCubeMapTexture("cubemap.jpg");

   glClearColor (0.0, 0.0, 0.0, 0.0);
   //glShadeModel (GL_FLAT);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   float position[4] = {100.0f, 0.0f, 0.0f, 1.0f};
   float light_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
   float light_diffuse[4] = {0.9f, 0.9f, 0.9f, 1.0f};
   float light_specular[4] = {0.0f, 0.5f, 0.5f, 1.0f};
   float ambient[4] = {0.4f, 0.4f, 0.4f, 1.0f};
   float diffuse[4] = {0.8f, 0.0f, 0.0f, 1.0f};
   float specular[4] = {0.0f, 0.5f, 0.5f, 1.0f};
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

   glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 16.0);

}

void reshape(int w, int h)
{
   width = w;
   height = h;
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fovy, (float)w/(float)h, 1.0, 100.0);
   //glFrustum(-w*0.2, w*0.2, -h*0.2, h*0.2, 10.0, 70.0);
   //glOrtho(-w*0.2, w*0.2, -h*0.2, h*0.2, 1.0, 200.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(2.0,5.0,-10.0,0.0,0.0,0.0,0.0,1.0,0.0);

   //printf("%d %d\n", w, h);
}

void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            rot = 1;
            //glutIdleFunc(spinDisplay);
            printf("LEFT\n");
         }
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
            rot = 0;
            //glutIdleFunc(NULL);
         break;
      case 3:
         fovy *= 1.01;
         fovy = (fovy >= 100.0)?100.0:fovy;
         reshape(width, height);
         break;
      case 4:
         fovy *= 0.99;
         reshape(width, height);
         break;
      default:
         printf("button: %d\n",button);
         break;
   }
}

void motion(int x, int y) {
  //printf("Motion: %d %d\n", x, y);
}

void special(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT:
      printf("LEFT\n");
      break;
    case GLUT_KEY_DOWN:
      printf("DOWN\n");
      break;
    case GLUT_KEY_RIGHT:
      printf("RIGHT\n");
      break;
    case GLUT_KEY_UP:
      printf("UP\n");
      break;
    default:
      break;
  }
}

void initShaders() {
	GLenum err = glewInit();
	if (GLEW_OK != err)
	  fprintf(stderr, "%s\n", glewGetErrorString(err));

	programId = loadShaders("vertex.glsl","fragment.glsl");
}
 
  
/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (250, 250); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[1]);
   initShaders();
   
   model = load_ply_model(argv[1]);

   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutSpecialFunc(special);
   glutMainLoop();

   delete_ply_model(model);

   return 0;   /* ANSI C requires main to return int. */
}

