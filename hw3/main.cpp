#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"
#include "marchingcubes.cpp"

using namespace std;

// for storing vertexes

float _angle = 60.0f;
float _angleLat = 60.0f;
float _angleLon = 0.0f;
float _walk = -10.0f;
float _stride = 0.0f;
float _elevate = 0.0f;
int width;
int height;
bool *** voxels;
vector<vertex> vertices;
int sample=1;

void cleanup() {
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			cleanup();
			exit(0);
    case 97: // A key
      _angleLat += 2.0f;
      if (_angleLat > 360) {
      _angleLat -= 360;
      }
      glutPostRedisplay();
      break;
    case 100: // D key

      _angleLat -= 2.0f;
      if (_angleLat < 0) {
      _angleLat -= 360;
      }
      glutPostRedisplay();
      break;
    case 119: // W key
      _angleLon += 2.0f;
      if (_angleLon > 360) {
      _angleLon -= 360;
      }
      glutPostRedisplay();
      break;
    case 115: // S key

      _angleLon -= 2.0f;
      if (_angleLon < 0) {
      _angleLon -= 360;
      }
      glutPostRedisplay();
      break;
    case 105: // I key
      _walk += 0.5f;
      glutPostRedisplay();
      break;
    case 107: // K key

      _walk -= 0.5f;
      glutPostRedisplay();
      break;
    case 106: // J key
      _stride += 0.5f;
      glutPostRedisplay();
      break;
    case 108: // L key

      _stride -= 0.5f;
      glutPostRedisplay();
      break;
    case 113: // Q key
      _elevate += 0.5f;
      glutPostRedisplay();
      break;
    case 101: // E key

      _elevate -= 0.5f;
      glutPostRedisplay();
      break;
}
	
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w / (double)h, 1.0, 900.0);
}

void drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
   
  // Create light components
  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };
   
  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
      
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  // Set material properties which will be assigned by glColor
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
  glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glRotatef(_angle, 0.0f, 1.0f, 0.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushMatrix();
  glScalef(0.003f, 0.003f, 0.003f);
  glColor3f(0.0f, 0.6f, 0.0f);
  glTranslatef(0.0f, -128.0f, 32.0f);
  glRotatef(80.0f, 0.0f, 0.0f, 1.0f);
  

  // this is where you should draw your objects
  // there had better be code here by tomorrow or I will make you sad.
  // I should take the cheeseMesh that I hopefully created, consisting of two solid objects 
  // derived from the slices of the cheese I took
  
  vector<vertex>::iterator it;
  glBegin(GL_TRIANGLES);
      for(it = vertices.begin(); it < vertices.end(); it++) {
          glNormal3d(it->normal_x, it->normal_y, it->normal_z);
          glVertex3d(it->x, it->y, it->z);
      }
  glEnd();

	glutSwapBuffers();
}

void update(int value) {
	_angle += .5f;
	if (_angle > 360) {
		_angle -= 360;
	}
	
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
  // build the datatype
  char filename[21];
  int i=1;
  int range = 100;
  // this is where you specify the number of slices
  // rewrite this in xyz form, i think that's the current issue
  voxels = new bool**[4];
  for (i = 0; i < 4; i++) {
    sprintf(filename, "data/blurry/%d.bmp",i+1);
    Image* image;
    image = loadBMP(filename);
    width = image->width;
    height = image->height;
    // need 3d array here.
    int y = i;
    int x, z;
    int target=1;
    // voxels[y].resize(width);
    voxels[y] = new bool*[width];
    for (x = 0; x < width; x+=sample) {
      // voxels[y][x].resize(height);
      voxels[y][x] = new bool[height];
      for (z = 0; z < height; z+=sample) {
        unsigned char redcolor = 0;
        // redcolor = (unsigned char)image ->pixels[3*(z * image->width + x)+2];
        redcolor = (unsigned char)image ->pixels[3*(z * image->width + x)];
        int red = (int)redcolor;
        // time to outline the cheese
        if (target) {
          if (red<range) {
            voxels[y][x][z]=true;
            target=0;
          }
        }
        else {
          if (red>range) {
            voxels[y][x][z]=true;
            target=1;
          }
        }
      }
    }
  }
  vertices = runMarchingCubes(voxels, 4, 500, 500, 1, 1, 1, 31.0);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	
	glutCreateWindow("cheese smells funny");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}

