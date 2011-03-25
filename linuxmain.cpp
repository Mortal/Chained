#include "root.h"
#include "vkeys.h"
#include <unistd.h> /* usleep() */
#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <sys/time.h>

Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

int main(int argc, char *argv[]) {
	Root::init();
	Root::EnableOpenGL();
	Root::loadtexs();
	XSelectInput(dpy, win, KeyPressMask | KeyReleaseMask);
	/* program main loop */
	while(1) {
		/* check for messages */
		if (XCheckMaskEvent(dpy, KeyPressMask|KeyReleaseMask, &xev)) {
			XKeyEvent *key = (XKeyEvent *) &xev;
			printf("Event type: %d\n", key->type);
			KeySym sym = XKeycodeToKeysym(dpy, key->keycode, 0);
			int vk;
			if (sym == XK_Left) vk = VK_LEFT;
			else if (sym == XK_Up) vk = VK_UP;
			else if (sym == XK_Right) vk = VK_RIGHT;
			else if (sym == XK_Down) vk = VK_DOWN;
			else vk = sym;
			if (key->type == KeyPress) {
				printf("Key down: %d -> %d -> %d\n", key->keycode, sym, vk);
				Root::KeyDown(vk);
			} else {
				printf("Key up: %d -> %d -> %d\n", key->keycode, sym, vk);
				Root::KeyUp(vk);
			}
		} else if (XCheckTypedEvent(dpy, ClientMessage, &xev)) {
			break;
		} else {
			/* OpenGL code goes here */
			struct timeval start;
			gettimeofday(&start, NULL);

			XGetWindowAttributes(dpy, win, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);

			glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glPushMatrix ();
			glMatrixMode(GL_PROJECTION);
			int swap = Root::tick() ? 0 : 1;
			glPopMatrix ();

			if (swap) glXSwapBuffers(dpy, win);

			struct timeval end;
			gettimeofday(&end, NULL);

			int elapsed = (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
			int tosleep = 20000-elapsed;
			if (tosleep > 0) usleep(tosleep);
		}
	}
	printf("Bye bye\n");
	Root::DisableOpenGL();
}

void Root::EnableOpenGL() {

	dpy = XOpenDisplay(NULL);

	if(dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(0);
	}

	root = DefaultRootWindow(dpy);

	vi = glXChooseVisual(dpy, 0, att);

	if(vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(0);
	} else {
		printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
	}


	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = KeyPressMask | KeyReleaseMask;

	win = XCreateWindow(dpy, root, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, WINNAME);

	Atom wmDelete=XInternAtom(dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(dpy, win, &wmDelete, 1);

	glc = glXCreateContext(dpy, vi, NULL, GL_FALSE);
	glXMakeCurrent(dpy, win, glc);

	glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void Root::DisableOpenGL() {
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

