#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"
// #include "marchingcubes.hpp"

using namespace std;

// for storing vertexes
struct vertex {
	double x, y, z, normal_x, normal_y, normal_z;
};

float _angle = 60.0f;
float _angleLat = 60.0f;
float _angleLon = 0.0f;
float _walk = -10.0f;
float _stride = 0.0f;
float _elevate = 0.0f;
int width;
int height;
bool*** voxels;
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
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  glTranslatef(_stride, _elevate, _walk);
  glRotatef(-_angleLat, 0.0f, 1.0f, 0.0f);
  glRotatef(-_angleLon, -_angleLon, 1.0f, 0.0f);
	
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
  // this is where you should draw your objects
  // there had better be code here by tomorrow or I will make you sad.
  // I should take the cheeseMesh that I hopefully created, consisting of two solid objects 
  // derived from the slices of the cheese I took

  glBegin(GL_POINTS);

  // crawl the images
  int i;
  for (i = 0; i < 4; i++) {
    int y = i;
    int x, z;
    int target=1;
    for (x = 0; x < width; x++) {
     for (z = 0; z < height; z++) {
       float xb = (float)(x-(width/2))/100.0f;
       float yb = (float)y/7.0f;
       float zb = (float)(z- (height/2))/100.0f;
       // time to outline the cheese
       if (voxels[y][x][z]) {
         glVertex3f(xb, yb, zb);
         target=0;
       }
     }
   }
  }
  glEnd();
  
  // vector<vertex>::iterator it;
  // glBegin(GL_TRIANGLES);
  //     for(it = vertices.begin(); it < vertices.end(); it++) {
  //         glNormal3d(it->normal_x, it->normal_y, it->normal_z);
  //         glVertex3d(it->x, it->y, it->z);
  //     }
  // glEnd();
  

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
  // vertices = runMarchingCubes(voxels, 500, 500, 4, 1, 1, 1, 1.0);

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

