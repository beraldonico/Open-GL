/*
MOUSE
    RIGHT start
    LEFT stop
    MIDDLE restart zoom
    Scrool zoom in and out
KEY
    LEFT    |
    UP      | Move camera
    DOWN    |
    RIGHT   |
    F1 increase speed
    F2 decrease speed
*/
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

GLint rot = 0;
static GLfloat spin = 0.0;
GLfloat fovy = 25.0;
GLfloat velo = 5.0;
GLint width, height;
GLdouble move_cx = 0.0;
GLdouble move_cy = 0.0;

void display(void)
{
    if (rot)
    {
        spin = spin + velo;
        if (spin > 360.0)
            spin = spin - 360.0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(move_cx, move_cy, 0.0);

    //nucleo
    //4 Protons
    glPushMatrix();
    glTranslatef(0.0,2,0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,-2,0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,0.0,0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2,0.0,0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    //5 Neutrons
    glPushMatrix();
    glTranslatef(0,0.0,0.0);
    glColor3f(1.0,1.0,1.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.20,1.20,0.0);
    glColor3f(1.0,1.0,1.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.20,-1.20,0.0);
    glColor3f(1.0,1.0,1.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.20,1.20,0.0);
    glColor3f(1.0,1.0,1.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.20,-1.20,0.0);
    glColor3f(1.0,1.0,1.0);
    glutSolidSphere(2.5, 25, 25);
    glPopMatrix();

    //eletron 1 horinzontal
    glPushMatrix();
    glRotatef(spin, 0.0, 1.0, 0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(1, 1, 1);
    //
    glPushMatrix();
    glTranslatef(40.0,0.0,0.0);
    glColor3f(0.0,1.0,0.0);
    glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
    //
    glPopMatrix();

    //eletron 2 vertical
    glPushMatrix();
    glRotatef(spin, 1.0, 0.0, 0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(1, 1, 1);
    //
    glPushMatrix();
    glTranslatef(0.0,40.0,0.0);
    glColor3f(0.0,1.0,0.0);
    glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
    //
    glPopMatrix();

    //eletron 3 diagonal
    glPushMatrix();
    glRotatef(spin, 1.0, 1.0, 0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(1, 1, 1);
    //
    glPushMatrix();
    glTranslatef(0.0,0.0,40.0);
    glColor3f(0.0,1.0,0.0);
    glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
    //
    glPopMatrix();

    //eletron 4
    glPushMatrix();
    glRotatef(spin, 1.0, -1.0, 0.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(1, 1, 1);
    //glutWireSphere(5, 5, 5);
    //
    glPushMatrix();
    glTranslatef(0.0,0.0,-40.0);
    glColor3f(0.0,1.0,0.0);
    glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
    //
    glPopMatrix();

    //eletron 1 horinzontal
    glPushMatrix();
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(0.20,40,25,25);
    //
    glPopMatrix();

    //eletron 1 vertical
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glutSolidTorus(0.20,40,25,25);
    //
    glPopMatrix();

    //eletron 1 diagonal
    glPushMatrix();
    glRotatef(90.0,1.0,1.0,0.0);
    //glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidTorus(0.2,40,25,25);
    //
    glPopMatrix();

    //eletron 1 diagonal
    glPushMatrix();
    glRotatef(90.0,1.0,-1.0,0.0);
    //glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidTorus(0.2,40,25,25);
    //
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();

    glutPostRedisplay();
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (float)w/(float)h, 200.0, 400.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,-300.0, 0.0,0.0,0.0, 0.0,1.0,0.0);

    printf("%d %d\n", w, h);
}

void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            rot = 1;
            printf("LEFT BUTTON\n");
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN)
        {
            fovy = 25.0;
            reshape(width, height);
            printf("MIDDLE BUTTON\n");
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            rot = 0;
            printf("RIGHT BUTTON\n");
        }
        break;
    case 3:
        fovy *= 0.99;
        reshape(width, height);
        break;
    case 4:
        fovy *= 1.01;
        fovy = (fovy >= 100.0)?100.0:fovy;
        reshape(width, height);
        break;
    default:
        printf("button: %d\n",button);
        break;
    }
}

void motion(int x, int y)
{
    printf("Motion: %d %d\n", x, y);
}

void special(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        printf("LEFT KEY\n");
        move_cx+= 5.0;
        break;
    case GLUT_KEY_DOWN:
        printf("DOWN KEY\n");
        move_cy-= 5.0;
        break;
    case GLUT_KEY_RIGHT:
        printf("RIGHT KEY\n");
        move_cx-= 5.0;
        break;
    case GLUT_KEY_UP:
        printf("UP KEY\n");
        move_cy+= 5.0;
        break;
    case GLUT_KEY_F1:
        printf("F1 KEY\n");
        velo -= 1.0;
        break;
    case GLUT_KEY_F2:
        printf("F2 KEY\n");
        velo += 1.0;
        break;
    default:
        //so printa keys especiais
        printf("KEY: %d\n",key);
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH |GLUT_RGB);
    glutInitWindowSize (927, 560);
    glutInitWindowPosition (0, 0);
    //glutCreateWindow (argv[1]);
    glutCreateWindow ("trabalho 1");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    //glutPassiveMotionFunc(motion);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;   /* ANSI C requires main to return int. */
}

