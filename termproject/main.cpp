#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;

// for storing vertexes

float _angle = 0.0f;
float _angleLat = 00.0f;
float _angleLon = 0.0f;
float _walk = 0.0f;
float _stride = 400.0f;
float _elevate = -400.0f;
int width;
int height;
int data_length;
struct GPScoord* coords;

struct GPScoord {
  float g_lat;
  float g_long;
};

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
      _angleLat = 0;
      }
      glutPostRedisplay();
      break;
    case 100: // D key

      _angleLat -= 2.0f;
      if (_angleLat < 0) {
      _angleLat = 360;
      }
      glutPostRedisplay();
      break;
    case 119: // W key
      _angleLon += 2.0f;
      if (_angleLon > 360) {
      _angleLon = 0;
      }
      glutPostRedisplay();
      break;
    case 115: // S key

      _angleLon -= 2.0f;
      if (_angleLon < 0) {
      _angleLon = 360;
      }
      glutPostRedisplay();
      break;
    case 105: // I key
      _walk += 5.f;
      glutPostRedisplay();
      break;
    case 107: // K key

      _walk -= 5.f;
      glutPostRedisplay();
      break;
    case 106: // J key
      _stride += 5.f;
      glutPostRedisplay();
      break;
    case 108: // L key

      _stride -= 5.f;
      glutPostRedisplay();
      break;
    case 113: // Q key
      _elevate += 5.f;
      glutPostRedisplay();
      break;
    case 101: // E key

      _elevate -= 5.f;
      glutPostRedisplay();
      break;
    }
	
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the texture

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	
	Image* image = loadBMP("images/alabama.bmp");
	_textureId = loadTexture(image);
	delete image;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w / (double)h, 1.0, 1000.0);
}

void drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.f, 1.f, 1.f);
  // Create light components
  GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };
   
  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
      
  glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // draw objects here
  
  // Set material properties which will be assigned by glColor
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
  glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);

  glLoadIdentity();

  glPushMatrix();
  glScalef(0.0013f, 0.0013f, 0.0013f);

  // printf("%f, %f, %f\n", _stride, _elevate, _walk);
  glTranslatef(-_stride, _elevate, _walk);
  glRotatef(_angleLat, 0.f, 1.f, 0.f);
  glRotatef(-_angleLon, 1.f, 0.f, 0.f);

  glBegin(GL_QUADS);
    glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
    glTexCoord2d(1.0,0.0); glVertex2d(900.0,0.0);
    glTexCoord2d(1.0,1.0); glVertex2d(900.0,900.0);
    glTexCoord2d(0.0,1.0); glVertex2d(0.0,900.0);
  glEnd();


  glColor3f(0.f, 0.f, 1.f);
  // lets designate a point on the map
  // ideally, we should read in points from a file


  float x_pos = 0.f;
  float y_pos = 0.f;
  glPointSize(5.f);
  glBegin(GL_POINTS);
  int i;
  for (i = 0; i < data_length; i++) {
    // get the difference
    x_pos = (-88.202949f - coords[i].g_long)/0.007f;
    y_pos = (35.0080284f - coords[i].g_lat)/0.006f;
    glVertex3f(243.f-x_pos, 843.f-y_pos, -50.f);
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
  // count the number of lines in the text file (number of locations)
  FILE *f=fopen("data.txt","rb");
  int c=0,b;while ((b=fgetc(f))!=EOF) c+=(b==10)?1:0;fseek(f,0,SEEK_SET);

  data_length = c;
  // make a structure in memory to hold the coords
  coords = new struct GPScoord[c];

  string val;
  string val2;
  struct GPScoord tmp;
  fstream input;
  input.open("data.txt");

  // fill in the array
  int counter = 0;
  while (!input.eof()) {
    input >> val;
    input >> val2;
    tmp.g_lat = atof(val.c_str());
    tmp.g_long = atof(val2.c_str());
    coords[counter] = tmp;
    counter++;
  }
  input.close();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	
	glutCreateWindow("housing density");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}
