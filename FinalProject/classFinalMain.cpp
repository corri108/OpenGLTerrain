//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <iostream>
#include <ctime>

#include "Camera.h"
#include "MyColor.h"
#include "BMPLoader.h"

using namespace std;
static GLuint texName2;


//properties and variables
float windowWidth2, windowHeight2, fov2;

//light positions
GLfloat light_pos3[] = { -10,5,0, 1 };
GLfloat light_pos4[] = { 10, 5, 0, 1 };

//init the camera
Camera *camera2 = new Camera(new Vector3(0, 5, 10), new Vector3(0, 0, 0), new Vector3(0, 1, 0));

//lets do some texture loading with BMP
BMPLoader *bmp2 = new BMPLoader();
GLuint tex2;

float randomFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float randomFloat(float max)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / max);
}

void doLighting(MyColor *mc)
{
	//culling and stuff
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
	glEnable(GL_CULL_FACE);     // Cull back facing polygons
	glCullFace(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	//light position
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos3);
	//light ambient
	GLfloat ambient[] = { mc->r,mc->g,mc->b,1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//diffuse
	GLfloat diffuse[] = { mc->r,mc->g,mc->b,1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//spec
	GLfloat specular[] = { mc->r,mc->g,mc->b,1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_EMISSION, specular);

	//second light
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos4);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_EMISSION, specular);
}

void reshape(int w, int h)
{
	fov2 = 60;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera2->UpdatePerspective(fov2, windowWidth2, windowHeight2, 0.01, 1500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera2->update();
}

/*Cretes an OpenGL context so that GLEW can be initialized correctly*/
bool createContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	HWND hwnd; HDC hdc; int pixelFormat;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, "STATIC", "glctx",
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 640, 480, 0, 0, GetModuleHandle(NULL), 0);
	if (!hwnd) return false;
	ShowWindow(hwnd, SW_HIDE);

	hdc = GetDC(hwnd);
	pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);
	wglMakeCurrent(hdc, wglCreateContext(hdc));
	return wglGetCurrentContext() != NULL;
}

/*Initializes objects and variables needed for the project*/
void init()
{
	windowWidth2 = 1600;
	windowHeight2 = 900;
	fov2 = 60;

	//create context
	bool setupContext = createContext();
	cout << "Created context: " + setupContext;

	//init glew
	glewExperimental = TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		// Problem: glewInit failed, something is seriously wrong.
		cout << "glewInit failed: " << glewGetErrorString(err) << endl;
		//exit(1);
	}

	//load textures from BMPloader
	//tex = bmp->loadBMP_custom("grass.bmp");
	camera2->position = new Vector3(0, 0, 10);
	camera2->viewing = new Vector3(0,0,0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

/*Display function for the project*/
void display()
{
	glClearColor(1, 0, 0, 1);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//doLighting(new MyColor(1, 1, 1));

	//cam stuff
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera2->update();

	//draw sphere
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glutSolidSphere(100, 50, 50);
	glPopMatrix();

	glFlush();
	//swap buffers and redisplay
	glutSwapBuffers();
	glutPostRedisplay();
}

int Main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(windowWidth2, windowHeight2);
	glutInitWindowPosition(150, 0);
	glutCreateWindow("CPSC-360 Final");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
