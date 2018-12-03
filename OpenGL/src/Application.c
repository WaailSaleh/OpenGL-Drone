/*******************************************************************
		   Multi-Part Model Construction and Manipulation
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include "Vector3D.h"
#include "QuadMesh.h"
#include "Building.c"
//#include "CubeMesh.cpp"

const int meshSize = 16;    // Default Mesh Size
const int vWidth = 650;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels
static GLdouble posx = 0;
static GLdouble posy = 0;
static GLdouble posz = 0;
static GLdouble camx = 0;
static GLdouble camy = 10;
static GLdouble camz = 30;
static GLdouble height = 1;
static GLdouble scalex = 1;
static GLdouble scalez = 1;
static GLdouble lift = 1;
static Building x;
static int currentButton;
static unsigned char currentKey;
static BOOLEAN scaledown = false;
static BOOLEAN scaleup = false;
static int engine = 0;
static GLfloat spin = 1.0;
static GLfloat movement = 0;
static GLfloat rotation = 0;
static Building ground;
// Lighting/shading and material properties for drone - upcoming lecture - just copy for now

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat drone_mat_ambient[] = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat drone_mat_specular[] = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat drone_mat_diffuse[] = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat drone_mat_shininess[] = { 0.0F };

// Base properties
static BOOLEAN createBase = false;
static BOOLEAN createBuild = false;
static CubeMesh base;

// interactive method choices, s = scaling(vertical),t = transform, h = scale z,x
static char setmode;
// A quad mesh representing the ground
static QuadMesh groundMesh;

// Structure defining a bounding box, currently unused
//struct BoundingBox {
//    Vector3D min;
//    Vector3D max;
//} BBox;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);

void turn(GLfloat x);
void move(GLfloat x);

Vector3D ScreenToWorld(int x, int y);


int main(int argc, char **argv)
{
	camx = 0; camy = 10; camz = 30;
	// Initialize GLUT
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("511 Assignment 2: Waail Saleh : 500756739");
	CubeMesh g = newCube();
	ground = build(&g, 1, 100, 100, 0, 0);

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);
	
	// Start event loop, never returns
	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
	
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);   // This light is currently off

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	// Set up ground quad mesh
	//Vector3D origin = NewVector3D(-50.0f, 0.0f, 8.0f);
	//Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
	//Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
	//groundMesh = NewQuadMesh(meshSize);
	//InitMeshQM(&groundMesh, meshSize, origin, 100.0, 100.0, dir1v, dir2v);

	//Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
	//Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
	//Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
	//SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);
	
	// Set up the bounding box of the scene
	// Currently unused. You could set up bounding boxes for your objects eventually.
	//Set(&BBox.min, -8.0f, 0.0, -8.0);
	//Set(&BBox.max, 8.0f, 6.0,  8.0);
}

// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw Drone
	
	// Set drone material properties
	//glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);

	// Apply transformations to move drone
	// ...
	if (createBuild) {
		x = build(&base, height,  scalex,  scalez, posx, posz);
		
		createBuild = false;
	}
	if (scaleup) {
		scaleupb(&x);
		scaleup = false;
	}
	if (scaledown) {
		scaledown = false;
		scaledownb(&x);
	}
		

	
	drawBuilding(&x);
	
	glPushMatrix();

	glPushMatrix();
	glTranslatef(posx, -height, posz);
	glScaled(scalex, height, scalez);
	
	
	

		if (createBase)
			base = newCube();
	drawCube(&base);

	
	
	glPopMatrix();
	glTranslatef(0, -2.1, 0);
	drawBuilding(&ground);
	glPopMatrix();
	glRotatef(rotation, 0, 1, 0);
	rotation = 0;

	//reset();
	// Draw ground mesh
	//DrawMeshQM(&groundMesh, meshSize);
;

	glutSwapBuffers();   // Double buffering, swap buffers

	
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 80.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(camx, camy, camz, 0, 0, 0, 0, 1, 0);
	//printf("%1.f %.1f %.1f \n", camx, camy, camz);
}




// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	//printf(key);
	switch (key)
	{
	case 'd':
		scaledown = true;
		break;
	case 'u':
		scaleup = true;
		break;
	case 's':
	case 'h':
	case 't':
		setmode = key;

		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	
	if (key == GLUT_KEY_F1 )
	{
		createBase = true;
	}
	if (key == GLUT_KEY_F2)
	{
		createBuild = true;
	}
	if (key == GLUT_KEY_UP)
	{
		if (setmode == 'h')
			height += 1;
		if (setmode == 's')
		{
			scalez += 1;
		//	printf("%f",(double)  scalez);
		}
		if (setmode == 't')
			posz += -1;
	}
	if (key == GLUT_KEY_DOWN)
	{
		
		if (setmode == 'h')
			height += -1;
		if (setmode == 's')
			scalez += -1;
		if (setmode == 't')
			posz += 1;
	}
	if (key == GLUT_KEY_LEFT)
	{
		if(setmode=='t')
		posx += -1;
		if (setmode == 's')
			scalex += -1;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (setmode == 't')
			posx += 1;
		if (setmode == 's')
			scalex += 1;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

void move(GLfloat x) {
	//printf("%d \n", x);
	movement -= x ;
}
void turn(GLfloat x) {
	rotation = x;
	
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			
		}
		break;
	default:
		break;
		
	}

	glutPostRedisplay();   // Trigger a window redisplay
}
static int prevcamy;
static int prevcamx;

// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		
		if (xMouse - prevcamx > 10 || xMouse - prevcamx < -10)
		{
			if (xMouse > prevcamx)
				turn(1);
			else
				turn(-1);
			
			prevcamx = xMouse;
		}
		

		
		
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
	// you will need to finish this if you use the mouse
	return NewVector3D(0, 0, 0);
}


