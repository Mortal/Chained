/* vim:set ts=4 sw=4 sts=4 noet: */
#include "root.h"
#include "vkeys.h"
#include "textures.h"
#include <unistd.h> /* usleep() */
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <sys/time.h>

#define FRAMEDURATION (1000000/60)

int window_width, window_height;

int wd; // glut window

static void key_down(unsigned char key, int /*x*/, int /*y*/) {
	KeyDown(key);
}

static void key_up(unsigned char key, int /*x*/, int /*y*/) {
	KeyUp(key);
}

static int translate_key(unsigned char key) {
	switch (key) {
		case GLUT_KEY_F1: return VK_F1;
		case GLUT_KEY_F2: return VK_F2;
		case GLUT_KEY_F3: return VK_F3;
		case GLUT_KEY_F4: return VK_F4;
		case GLUT_KEY_F5: return VK_F5;
		case GLUT_KEY_F6: return VK_F6;
		case GLUT_KEY_F7: return VK_F7;
		case GLUT_KEY_F8: return VK_F8;
		case GLUT_KEY_F9: return VK_F9;
		case GLUT_KEY_F10: return VK_F10;
		case GLUT_KEY_F11: return VK_F11;
		case GLUT_KEY_F12: return VK_F12;
		case GLUT_KEY_LEFT: return VK_LEFT;
		case GLUT_KEY_UP: return VK_UP;
		case GLUT_KEY_RIGHT: return VK_RIGHT;
		case GLUT_KEY_DOWN: return VK_DOWN;
		case GLUT_KEY_PAGE_UP: return 0;
		case GLUT_KEY_PAGE_DOWN: return 0;
		case GLUT_KEY_HOME: return VK_HOME;
		case GLUT_KEY_END: return 0;
		case GLUT_KEY_INSERT: return 0;
		default: return 0;
	}
}

static void special_key_down(int key, int /*x*/, int /*y*/) {
	int vk = translate_key(key);
	if (vk) KeyDown(vk);
}

static void special_key_up(int key, int /*x*/, int /*y*/) {
	int vk = translate_key(key);
	if (vk) KeyUp(vk);
}

static void timer_redisplay(int /*value*/) {
	glutPostRedisplay();
}

static void display() {
	struct timeval start;
	gettimeofday(&start, NULL);

	glViewport(0, 0, window_width, window_height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	int r = tick();
	glPopMatrix();
	glFlush();

	struct timeval end;
	gettimeofday(&end, NULL);

	int elapsed = (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
	int tosleep = FRAMEDURATION-elapsed;
	int msec = tosleep / 1000;
	if (msec > 0) glutTimerFunc(msec, timer_redisplay, 0);
	else glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	init();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	window_width = SCREEN_WIDTH;
	window_height = SCREEN_HEIGHT;
	wd = glutCreateWindow(WINNAME);
	// glutReshapeFunc(reshape);
	// void reshape(int w, int h)
	glutKeyboardFunc(key_down);
	glutKeyboardUpFunc(key_up);
	glutSpecialFunc(special_key_down);
	glutSpecialUpFunc(special_key_up);
	glutDisplayFunc(display);
	init_textures();
	glutMainLoop();
	glutDestroyWindow(wd);
}
