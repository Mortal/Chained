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
	Root::init();
    
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
    Root::logfile << width << " " << height << std::endl;

    /* enable OpenGL for the window */
    Root::EnableOpenGL();
    
    /*ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);*/
    
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	                 //012345678901234
    char filename[] = "block - -  .raw";
    for (int i = 1; i <= 9; ++i) {
        if (i == 8) continue;
        if (i < 10)
            filename[5] = (char) (i+'0');
        else
            filename[5] = (char) ((i-10)+'A');
        for (int j = 1; j <= 3; ++j) {
            switch (j) {
                case 1:
                    filename[7] = '0';
                break;
                case 2:
                    filename[7] = 'a';
                break;
                case 3:
                    filename[7] = 'b';
                break;
                default:
                    Root::logfile << __LINE__ << " fffffff";
                break;
            }
            for (int frameid = 0; frameid < BLOCK_MAXFRAMES; ++frameid) {
                filename[9] = '0' + frameid / 10;
                filename[10] = '0' + frameid % 10;
                int imgidx = (j == 1 ? IMGMEM_OFFSET0 : (j == 2 ? IMGMEM_OFFSETA : IMGMEM_OFFSETB)) + IMGMEM__FRAMES * i + frameid;
                Root::logfile << filename << "->" << imgidx << std::endl;
				std::fstream fin;
				fin.open(filename, std::ios::in | std::ios::binary);
                if (!fin.is_open()) break;
                
                int size = BLOCK_SRCWIDTH*BLOCK_SRCHEIGHT*3;
                char *image = new char[size];
                
				fin.read(image, size);
                fin.close();
            
                glBindTexture(GL_TEXTURE_2D, imgidx);
                
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
                
                glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, BLOCK_SRCWIDTH, BLOCK_SRCHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

				delete image;
                
            }
        }
    }

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
            
            clock_t endframe = clock() + Root::CLOCKS_PER_FRAME;

            glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
            glClear (GL_COLOR_BUFFER_BIT);

            glPushMatrix ();
            glMatrixMode(GL_PROJECTION);
            int swap = Root::tick() ? 0 : 1;
            glPopMatrix ();

            if (swap) SwapBuffers (hDC);
            while (clock() < endframe) {Sleep(1);}
        }
    }

    /* shutdown OpenGL */
    Root::DisableOpenGL();

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
        Root::KeyUp(wParam);
        return 0;

    case WM_KEYDOWN:
        if (lParam & 0x40000000) return 0;
        Root::KeyDown(wParam);
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

void Root::EnableOpenGL()
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

void Root::DisableOpenGL ()
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}
