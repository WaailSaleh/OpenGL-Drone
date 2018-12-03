#include <stdbool.h>
#include "Vector3D.h"
#include <GL/glut.h>
#include "CubeMesh.cpp"

const static int floorlimit = 30;



static GLfloat basecube[][3] = { {-1.0, -1.0,-1.0},
						 { 1.0, -1.0,-1.0},
						 { 1.0,  1.0,-1.0},
						 {-1.0,  1.0,-1.0},
						 {-1.0, -1.0, 1.0},
						 { 1.0, -1.0, 1.0},
						 { 1.0,  1.0, 1.0},
						 {-1.0,  1.0, 1.0} };
typedef struct Building
{
	// floorlimit * 8 * 3
	GLfloat vertices[30][8][3];
	//floorlimit * 6
	// four points= one quad
	// six quads = one floor
	//floor limit = which floor
	
	BOOLEAN scaled;
	BOOLEAN Scaledup;

	int   floors;
	GLfloat position[3];


} Building;
char* toString(Building *b) {
	char *s = '2';
	return s;
}
Building build(CubeMesh *cube,GLfloat height, GLfloat scalex, GLfloat scalez, GLfloat posx, GLfloat posz)
{
	 Building b;
	 b.scaled = false;
	 b.position[0] = posx;
	 b.position[1] = 0;
	 b.position[2] = posz;
	 b.floors = height ;
	 for (int i = 0; i < b.floors; i++) {
		 for (int j = 0; j < 8; j++) {
			 for (int k = 0; k < 3; k++) {
				 b.vertices[i][j][k] = basecube[j][k];
			 }
			 b.vertices[i][j][0] *= scalex;
			 b.vertices[i][j][1] += 1 + (i  *2);
			 b.vertices[i][j][2] *= scalez;
		
		 }
	 }
	 
	 return b;
}
void scaleupb(Building *b) 
{
	if (b->scaled)
		return;
	for (int i = 0; i < b->floors; i++) {
		for (int j = 0; j < 8; j++) {
			
			b->vertices[i][j][0] *=  i+1 ;
			b->vertices[i][j][2] *=  i+1 ;
			}
	}
	b->scaled = true;
	b->Scaledup = true;
}
void scaledownb(Building *b)
{	
	if (b->scaled)
		return;
	for (int i = 0; i < b->floors; i++) {
		for (int j = 0; j < 8; j++) {

			b->vertices[i][j][0] *= b->floors - i;

			b->vertices[i][j][2] *= b->floors - i;
		}
	}
	b->scaled = true;
	b->Scaledup = false;
}

void drawBuilding(Building *b) {
	int v0 = 0, v1 = 1, v2 = 2, v3 =3 ,v4 = 4,v5 =5, v6 = 6 ,v7 = 7;
	glPushMatrix();
	glTranslatef(b->position[0], 0, b->position[2]);
	glBegin(GL_QUADS);
	for (int i = 0; i < b->floors; i++) {
		
		// front Face
		glNormal3f(quadNormals[4][0], quadNormals[4][1], quadNormals[4][2]);
		glVertex3f(b->vertices[i][v4][0], b->vertices[i][v4][1], b->vertices[i][v4][2]);
		glVertex3f(b->vertices[i][v5][0], b->vertices[i][v5][1], b->vertices[i][v5][2]);
		glVertex3f(b->vertices[i][v6][0], b->vertices[i][v6][1], b->vertices[i][v6][2]);
		glVertex3f(b->vertices[i][v7][0], b->vertices[i][v7][1], b->vertices[i][v7][2]);
		// back Face

		glNormal3f(quadNormals[1][0], quadNormals[1][1], quadNormals[1][2]);
		glVertex3f(b->vertices[i][v2][0], b->vertices[i][v2][1], b->vertices[i][v2][2]);
		glVertex3f(b->vertices[i][v3][0], b->vertices[i][v3][1], b->vertices[i][v3][2]);
		glVertex3f(b->vertices[i][v6][0], b->vertices[i][v6][1], b->vertices[i][v6][2]);
		glVertex3f(b->vertices[i][v7][0], b->vertices[i][v7][1], b->vertices[i][v7][2]);
		// Left Face
		glNormal3f(quadNormals[2][0], quadNormals[2][1], quadNormals[2][2]);
		glVertex3f(b->vertices[i][v0][0], b->vertices[i][v0][1], b->vertices[i][v0][2]);
		glVertex3f(b->vertices[i][v4][0], b->vertices[i][v4][1], b->vertices[i][v4][2]);
		glVertex3f(b->vertices[i][v7][0], b->vertices[i][v7][1], b->vertices[i][v7][2]);
		glVertex3f(b->vertices[i][v3][0], b->vertices[i][v3][1], b->vertices[i][v3][2]);
		// Right Face
		glNormal3f(quadNormals[3][0], quadNormals[3][1], quadNormals[3][2]);
		glVertex3f(b->vertices[i][v1][0], b->vertices[i][v1][1], b->vertices[i][v1][2]);
		glVertex3f(b->vertices[i][v2][0], b->vertices[i][v2][1], b->vertices[i][v2][2]);
		glVertex3f(b->vertices[i][v6][0], b->vertices[i][v6][1], b->vertices[i][v6][2]);
		glVertex3f(b->vertices[i][v5][0], b->vertices[i][v5][1], b->vertices[i][v5][2]);
		//top face
		glNormal3f(quadNormals[1][0], quadNormals[1][1], quadNormals[1][2]);
		glVertex3f(b->vertices[i][v2][0], b->vertices[i][v2][1], b->vertices[i][v2][2]);
		glVertex3f(b->vertices[i][v3][0], b->vertices[i][v3][1], b->vertices[i][v3][2]);
		glVertex3f(b->vertices[i][v7][0], b->vertices[i][v7][1], b->vertices[i][v7][2]);
		glVertex3f(b->vertices[i][v6][0], b->vertices[i][v6][1], b->vertices[i][v6][2]);
		
	}
	glEnd();
	
	glPopMatrix();
}