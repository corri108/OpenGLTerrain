//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\glut.h>
#include <iostream>
#include <ctime>

#include "Camera.h"
#include "MyColor.h"
#include "Terrain.h"
#include "PerlinNoise.h"
#include "newCamera.h"
#include "BMPLoader.h"

using namespace std;

#define checkImageWidth 128
#define checkImageHeight 128
#define subImageWidth 16
#define subImageHeight 16
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte subImage[subImageHeight][subImageWidth][4];

static GLuint texName;

void makeCheckImages(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)0;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	for (i = 0; i < subImageHeight; i++) {
		for (j = 0; j < subImageWidth; j++) {
			c = ((((i & 0x4) == 0) ^ ((j & 0x4)) == 0)) * 255;
			subImage[i][j][0] = (GLubyte)c;
			subImage[i][j][1] = (GLubyte)0;
			subImage[i][j][2] = (GLubyte)0;
			subImage[i][j][3] = (GLubyte)255;
		}
	}
}

//keyboard control struct
struct ControlStates
{
	bool forward,backward,left,right,//translation
		yawLeft,yawRight,//y rotation
		pitchUp,pitchDown;//x rotation
} controls;//instance

//colors for determining heights of the terrain
MyColor *ColorHeights[4]
{
	//water
	new MyColor(0.03, 0.1, 1),
	//grass
	new MyColor(0.2, 0.9, 0.3),
	//rock
	new MyColor(0.9, 0.6, 0.4),
	//snow 
	new MyColor(0.9, 0.9, 0.9),
};

//heights for corresponding colors
float Heights[4]
{
	//water
	-3,
	//grass
	-1,
	//rock
	5.5,
	//snow
	10000
};

//properties and variables
float windowWidth, windowHeight, fov;

//light positions
GLfloat light_pos[] = { 200,400,0,1.0 };
GLfloat light_pos2[] = { -200,400,0,1.0 };

//terrain vars
float TERRAIN_WIDTH, TERRAIN_HEIGHT = 128;
float TERRAIN_SIZE = 10;
float TERRAIN_SCALE = 30;

//perlin noise stuff
double persistance, frequency, amplitude, octaves, seed = 0;
float MAX_SEED = 1000;//maximum seed number

//init the camera
//Camera *camera = new Camera(new Vector3(-10, 200, 100), new Vector3(0, -40, 10), new Vector3(0, 1, 0));
NewCamera *newCamera;
Terrain *terrain;
PerlinNoise *noiseGenerator;

//lets do some texture loading with BMP
BMPLoader *bmp = new BMPLoader();
GLuint tex;

Terrain *LoadTerrain(int width, int height, float size, float heightScale)
{
	float lowestT = 10000;
	float highestT = -10000;
	Terrain *T = new Terrain(width, height, size, heightScale);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			float h = noiseGenerator->GetHeight(x, y);
			T->SetHeight(x, y, h);

			if (h > highestT)
			{
				highestT = h;
			}
			else if (h < lowestT)
			{
				lowestT = h;
			}
		}
	}

	//cout << "Lowest value: " << lowestT << endl << "Highest value: " << highestT << endl;

	T->ComputeNormals();
	return T;
}

float RandomFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float RandomFloat(float max)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / max);
}

void RandomizeColors()
{
	for (int i = 0; i < 4; ++i)
	{
		ColorHeights[i] = new MyColor(RandomFloat(), RandomFloat(), RandomFloat());
	}
}

void SetEarthColors()
{
		//water
		ColorHeights[0] = new MyColor(0.03, 0.1, 1);
		//grass
		ColorHeights[1] = new MyColor(0.1, 0.55, 0.15);
		//rock
		ColorHeights[2] = new MyColor(0.8, 0.5, 0.25);
		//snow 
		ColorHeights[3] = new MyColor(0.9, 0.9, 0.9);
}

void DoLightingDouble(MyColor *mc, MyColor *mc2)
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
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	//light ambient
	GLfloat ambient1[] = { mc->r,mc->g,mc->b,1.0 };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambient1);
	glLightfv(GL_LIGHT0, GL_EMISSION, ambient1);


	////second light
	//GLfloat ambient2[] = { mc2->r,mc2->g,mc2->b,1.0 };
	//glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	////glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, ambient2);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, ambient2);
	//glLightfv(GL_LIGHT1, GL_EMISSION, ambient2);
}

void DoLighting(MyColor *mc)
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
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
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
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_EMISSION, specular);
}

void Reshape(int w, int h)
{
	//fov = 60;
	glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//newCamera->UpdatePerspective();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//newCamera->UpdateView();
}

void CameraControls()
{
	//translation
	if (controls.forward) newCamera->forward();		
	if (controls.backward) newCamera->backward();
	if (controls.left) newCamera->left();
	if (controls.right) newCamera->right();
	//yaw
	if (controls.yawLeft) newCamera->yawLeft();
	if (controls.yawRight) newCamera->yawRight();
	//pitch
	if (controls.pitchUp) newCamera->pitchUp();
	if (controls.pitchDown) newCamera->pitchDown();
}

/*Cretes an OpenGL context so that GLEW can be initialized correctly*/
bool CreateContext()
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

MyColor *GetColorFromHeight(float height)
{
	for (int i = 0; i < 4; ++i)
	{
		if (height < Heights[i])
		{
			return ColorHeights[i];
		}
	}

	//this should never really happen
	return ColorHeights[-1];
}

void SetColorForHeight(float height)
{
	MyColor *mc = GetColorFromHeight(height);
	glColor3f(mc->r, mc->g, mc->b);
}

void InitTestImage()
{
	glShadeModel(GL_SMOOTH);
	makeCheckImages();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

/*Initializes objects and variables needed for the project*/
void Init()
{
	windowWidth = 1600;
	windowHeight = 900;

	//create context
	bool setupContext = CreateContext();
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

	//init camera variables
	controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.pitchDown = false;
	controls.pitchUp = false;
	controls.yawLeft = false;
	controls.yawRight = false;
	//create camera pointer
	newCamera = new NewCamera(60, windowWidth, windowHeight, 0.01, 3000);

	persistance = 0;
	frequency = 0.08;
	amplitude = 15;
	octaves = 3;
	seed = 438;

	TERRAIN_HEIGHT = 128;
	TERRAIN_WIDTH = 128;
	TERRAIN_SCALE = 30;
	TERRAIN_SIZE = 10;

	//init terrain and perlin noise objects
	noiseGenerator = new PerlinNoise(persistance, frequency, amplitude, octaves, seed);
	terrain = LoadTerrain(TERRAIN_WIDTH, TERRAIN_HEIGHT, TERRAIN_SIZE, TERRAIN_SCALE);
	terrain->position = new Vector3(-100, 0, -100);
	//camera->viewing = terrain->position;

	//load textures from BMPloader
	tex = bmp->loadBMP_custom("grass.bmp");

	SetEarthColors();
	glClearColor (0.0, 0.0, 0.0, 0.0);
}

void DrawSun()
{

}

/*Display function for the project*/
void Display()
{
	//check camera controls
	CameraControls();

	//glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DoLighting(new MyColor(0.01, 0.01, 0.01));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	newCamera->UpdatePerspective();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//camera->update();

	// perform the camera orientation transform
	newCamera->transformOrientation();

	// perform the camera translation transform
	newCamera->transformTranslation();

	glPushMatrix();
	glTranslatef(terrain->position->x, terrain->position->y, terrain->position->z);
	//glColor3f(1, 1, 1);
	float size = terrain->size;
	for (int z = 0; z < terrain->length - 1; ++z)
	{
		//textures
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, tex);
		//begin strips
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < terrain->width; ++x)
		{
			///lets try this tex coord stuff out
			glTexCoord2f(0.0, 1.0);
			//get normal
			Vector3 norm = terrain->GetNormal(x, z);
			glNormal3f(norm.x, norm.y, norm.z);
			//get height, set color
			float h1 = terrain->GetHeight(x, z);
			SetColorForHeight(h1);
			//set vertex
			glVertex3f(x * size, h1 * terrain->heightScale, z * size);
			//repeat for next vertex
			///tex coord
			glTexCoord2f(1.0, 1.0);
			norm = terrain->GetNormal(x, z + 1);
			glNormal3f(norm.x, norm.y, norm.z);
			float h2 = terrain->GetHeight(x, z);
			SetColorForHeight(h2);
			glVertex3f(x * size, terrain->GetHeight(x, z + 1) * terrain->heightScale, (z + 1) * size);

			glDisable ( GL_TEXTURE_2D );
		}
		glEnd();
		glFlush();
	}
	glPopMatrix();

	//try tex coords
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(400, 700, 1000); //(400, 700, 1000)
	glTexCoord2f(0.0, 1.0); glVertex3f(400, 702, 1000); //(400, 702, 1000)
	glTexCoord2f(1.0, 1.0); glVertex3f(398, 702, 1000); //(398, 702, 1000)
	glTexCoord2f(1.0, 0.0); glVertex3f(398, 700, 1000);//(398, 700, 1000)
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
	glutPostRedisplay();
}

float rotY = 0.0;
void FinalDisplay()
{
	//check camera controls
	CameraControls();

	//glClearColor(0, 0, 0, 1);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	DoLightingDouble(new MyColor(01, 0, 0), new MyColor(0, 0, 01));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	newCamera->UpdatePerspective();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//camera->update();
	gluLookAt(0, 10, 300, 0, 0, 0, 0, 1, 0);

	rotY += 0.1;
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(rotY, 0.0, 1.0, 0.0);

	//glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexCoord2f(0.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glutSolidSphere(100, 50, 50);
	glDisable(GL_TEXTURE_2D);

	glFlush();
	glPopMatrix();

	//try tex coords
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(400, 700, 1000); //(400, 700, 1000)
	glTexCoord2f(0.0, 1.0); glVertex3f(400, 702, 1000); //(400, 702, 1000)
	glTexCoord2f(1.0, 1.0); glVertex3f(398, 702, 1000); //(398, 702, 1000)
	glTexCoord2f(1.0, 0.0); glVertex3f(398, 700, 1000);//(398, 700, 1000)
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
	glutPostRedisplay();
}

//key down function
void KeyDown(unsigned char key, int x, int y)
{

	switch (key)
	{
	case ',':
		newCamera->slowDown(); // slow down camera
		break;
	case '.':
		newCamera->speedUp(); // speed up camera
		break;
		// these are all camera controls
	case 'w':
		controls.forward = true;
		break;
	case 's':
		controls.backward = true;
		break;
	case 'a':
		controls.left = true;
		break;
	case 'd':
		controls.right = true;
		break;
	case 'i':
		controls.pitchDown = true;
		break;
	case 'k':
		controls.pitchUp = true;
		break;
	case 'q':
		controls.yawLeft = true;
		break;
	case 'e':
		controls.yawRight = true;
		break;
	}

}

void KeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		controls.forward = false;
		break;
	case 's':
		controls.backward = false;
		break;
	case 'a':
		controls.left = false;
		break;
	case 'd':
		controls.right = false;
		break;
	case 'i':
		controls.pitchDown = false;
		break;
	case 'k':
		controls.pitchUp = false;
		break;
	case 'q':
		controls.yawLeft = false;
		break;
	case 'e':
		controls.yawRight = false;
		break;
	}
}

void processMenuEvents(int option)
{
	if (option == 1)
	{
		//srand(static_cast <unsigned> (time(NULL)));
		seed = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / MAX_SEED);
		noiseGenerator = new PerlinNoise(persistance, frequency, amplitude, octaves, seed);
		terrain = LoadTerrain(TERRAIN_WIDTH, TERRAIN_HEIGHT, TERRAIN_SIZE, TERRAIN_SCALE);
	}
	else if (option == 2)
	{
		RandomizeColors();
	}
	else if (option == 3)
	{
		SetEarthColors();
	}
	else if (option == 4) glShadeModel(GL_SMOOTH);
	else if(option == 5) glShadeModel(GL_FLAT);
}

//to create the menu to select from
void CreateGlutMenu()
{
	// handle the events
	int menu = glutCreateMenu(processMenuEvents);

	//add entries to our menu
	glutAddMenuEntry("Generate Random Seed", 1);
	glutAddMenuEntry("Randomize Terrain Colors", 2);
	glutAddMenuEntry("Set Earth Colors", 3);
	glutAddMenuEntry("Smooth Rendering", 4);
	glutAddMenuEntry("Flat Rendering", 5);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	Init();
	InitTestImage();
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(150, 0);
	glutCreateWindow("CPSC-360 Final");	
	CreateGlutMenu();
	srand(static_cast <unsigned> (time(NULL)));
	seed = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / MAX_SEED);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMainLoop();
	return 0;
}
