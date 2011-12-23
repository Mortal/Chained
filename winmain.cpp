#include "winmain.h"
#include "root.h"
#include <time.h>
#include <GL/gl.h>

#include <windows.h>

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int iCmdShow);

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
						  WPARAM wParam, LPARAM lParam);


static HWND hWnd;
static HDC hDC;
static HGLRC hRC;

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR lpCmdLine,
					int iCmdShow)
{
	init();
	
	WNDCLASS wc;
	MSG msg;
	BOOL bQuit = FALSE;

	/* register window class */
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = UNIXNAME;
	RegisterClass (&wc);

	/* create main window */
	hWnd = CreateWindow (
	  UNIXNAME, WINNAME, 
	  WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
	  0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
	  NULL, NULL, hInstance, NULL);
	  
	RECT *hellooookitty;
	hellooookitty = new RECT;
	GetClientRect(hWnd, hellooookitty);
	int width = hellooookitty->right-hellooookitty->left;
	int height = hellooookitty->bottom-hellooookitty->top;
	width = 2*SCREEN_WIDTH-width;
	height = 2*SCREEN_HEIGHT-height;
	MoveWindow(hWnd,0,0,width,height,true);
	logfile << width << " " << height << std::endl;

	/* enable OpenGL for the window */
	EnableOpenGL();
	
	/*ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);*/
	
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	loadtexs();

	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage (&msg);
				DispatchMessage (&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */
			
			clock_t endframe = clock() + CLOCKS_PER_FRAME;

			glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
			glClear (GL_COLOR_BUFFER_BIT);

			glPushMatrix ();
			glMatrixMode(GL_PROJECTION);
			int swap = tick() ? 0 : 1;
			glPopMatrix ();

			if (swap) SwapBuffers (hDC);
			while (clock() < endframe) {Sleep(1);}
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL();

	/* destroy the window explicitly */
	DestroyWindow (hWnd);

	return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
						  WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		PostQuitMessage (0);
		return 0;

	case WM_DESTROY:
		return 0;

	case WM_KEYUP:
		KeyUp(wParam);
		return 0;

	case WM_KEYDOWN:
		if (lParam & 0x40000000) return 0;
		KeyDown(wParam);
		/*switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}*/
		return 0;

	default:
		return DefWindowProc (hWnd, message, wParam, lParam);
	}
}

void EnableOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	/* get the device context (DC) */
	hDC = GetDC (hWnd);

	/* set the pixel format for the DC */
	ZeroMemory (&pfd, sizeof (pfd));
	pfd.nSize = sizeof (pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
	  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat (hDC, &pfd);
	SetPixelFormat (hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	hRC = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hRC );

}

void DisableOpenGL ()
{
	wglMakeCurrent (NULL, NULL);
	wglDeleteContext (hRC);
	ReleaseDC (hWnd, hDC);
}
