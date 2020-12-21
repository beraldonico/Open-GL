/*
 * Main.cpp
 *
 *  Created on: 25/09/2017
 *      Author: gustavo
 */

#include <GL/freeglut.h>

#include "RayTracer.h"

void display() {
	RayTracer::getInstance()->display();
}

void reshape(int w, int h) {
	RayTracer::getInstance()->reshape(w,h);
}

void mouse(int button, int state, int x, int y) {
	RayTracer::getInstance()->mouse(button,state,x,y);
}

void motion(int x, int y) {
	RayTracer::getInstance()->motion(x,y);
}

void keyboard(unsigned char key, int x, int y) {
	RayTracer::getInstance()->keyboard(key, x, y);
}

int main (int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glutCreateWindow("MyRayTracer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

        char *filename = NULL;
        if (argc > 1) filename = argv[1];
	RayTracer::getInstance()->init(filename);

	glutMainLoop();

	return EXIT_SUCCESS;
}
