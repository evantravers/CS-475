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
float _walk = -1000.0f;
float _stride = 0.0f;
float _elevate = 0.0f;
int width;
int height;
struct GPScoord* coords;
int rows;
int cols;
int worldTime=0;

struct GPScoord {
  float g_lat;
  float g_long;
  float* data;
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
    case 109: // M key
      worldTime++;
      if (worldTime > cols) {
        worldTime=0;
      }
      glutPostRedisplay();
      break;
    case 110: // N key
      worldTime--;
      if (worldTime < 0) {
        worldTime = cols;
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
  width = image->width;
  height = image->height;
	_textureId = loadTexture(image);
	delete image;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (double)w / (double)h, 1.0, 1000000.0);
}

void drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // camera transforms
  glTranslatef(-_stride, _elevate, _walk);
  glRotatef(_angleLat, 0.f, 1.f, 0.f);
  glRotatef(-_angleLon, 1.f, 0.f, 0.f);
      
  // texture stuff
  glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // sets the color back to white for the map
  glColor3f(1.f, 1.f, 1.f);

  // draw the map
  glBegin(GL_QUADS);
    glTexCoord2d(0.f,0.f); glVertex2d(-450.f,-450.f);
    glTexCoord2d(1.f,0.f); glVertex2d(450.f,-450.f);
    glTexCoord2d(1.f,1.f); glVertex2d(450.f,450.f);
    glTexCoord2d(0.f,1.f); glVertex2d(-450.f,450.f);
  glEnd();


  // sets the color to red for the points
  glColor3f(0.f, 0.f, 1.f);
  
  // lets designate a point on the map
  // read in points from a file
  // takes in traditional lat/long

  float x_pos = 0.f;
  float y_pos = 0.f;
  glPointSize(5.f);
  glBegin(GL_POINTS);
  int i;
  for (i = 0; i < rows; i++) {
    // get the difference
    x_pos = (-88.202949f - coords[i].g_long)/0.007f;
    y_pos = (35.0080284f - coords[i].g_lat)/0.006f;
    // scale the pixel off the found top left corner. :P
    // TODO need to redo this map based on image size, not 900x900
    glVertex3f(243.f-x_pos-450.f, 843.f-y_pos-450.f, (coords[i].data[worldTime])/100.f);
  }
  glEnd();

	glutSwapBuffers();
}

void update(int value) {
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

void data_read(string inputfile) {
  string val;
  string val2;
  struct GPScoord tmp;
  fstream input;

  input.open(inputfile.c_str());

  // read in the number of rows and columns
  input >> rows;
  input >> cols;

  // make a structure in memory to hold the coords
  coords = new struct GPScoord[rows];

  // fill in the array
  int counter = 0;
  while (!input.eof()) {
    input >> tmp.g_lat;
    input >> tmp.g_long;
    // embed the data payload
    int data_i = 0;
    float* payload = new float[cols];
    while (data_i < cols) {
      input >> payload[data_i];
      data_i++;
    }
    tmp.data = payload;
    coords[counter] = tmp;
    counter++;
  }
  input.close();
}

int main(int argc, char** argv) {
  // TODO validate this, add a scaling factor
  if (argc > 3) {
    printf("args: text file, optional scaling factor\n");
    return 0;
  }
  else {
    if (argc==1) {
      data_read("data.txt");
    }
    if (argc==2) {
      data_read(argv[1]);
    }
  }

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

