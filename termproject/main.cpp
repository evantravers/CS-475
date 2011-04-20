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
int rows;
int maxRows;
int longest;
int worldTime=0;
bool play;
// the below is the array of csv files
struct dataSet* Dataset;
int numDatasets = 0;

struct GPScoord {
  float g_lat;
  float g_long;
};

struct dataSet{
  // the first row will be the titles, then the data
  float* date;
  float* unitSales;
  float* averageSellingPrice;
  float* averageDays;
  float* totalForSale;
  struct GPScoord coordinates;
  int numRows;
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
      if (worldTime > maxRows) {
        worldTime=0;
      }
      glutPostRedisplay();
      break;
    case 110: // N key
      worldTime--;
      if (worldTime < 0) {
        worldTime = maxRows;
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
      _walk += 50.f;
      glutPostRedisplay();
      break;
    case 107: // K key

      _walk -= 50.f;
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
    case 112: // P key
      if (play) {
        play=false;
      }
      else {
        play=true;
      }
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
	glGenTextures(1, &textureId);            // Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); // Tell OpenGL which texture to edit
	                                         // Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,              // Always GL_TEXTURE_2D
				 0,                                // 0 for now
				 GL_RGB,                           // Format OpenGL uses for image
				 image->width, image->height,      // Width and height
				 0,                                // The border of the image
				 GL_RGB,                           // GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE,                 // GL_UNSIGNED_BYTE, because pixels are stored
				                                   // as unsigned numbers
				 image->pixels);                   // The actual pixel data
	return textureId;                        // Returns the id of the texture
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
    glTexCoord2d(0.f,0.f); glVertex2d(-width/2.f,-height/2.f);
    glTexCoord2d(1.f,0.f); glVertex2d(width/2.f,-height/2.f);
    glTexCoord2d(1.f,1.f); glVertex2d(width/2.f,height/2.f);
    glTexCoord2d(0.f,1.f); glVertex2d(-width/2.f,height/2.f);
  glEnd();


  // sets the color to red for the points
  glColor3f(0.f, 0.f, 1.f);
  
  // lets designate a point on the map
  // read in points from a file
  // takes in traditional lat/long

  float x_pos = 0.f;
  float y_pos = 0.f;
  glPointSize(9.f);
  glLineWidth(4.f);

  glBegin(GL_POINTS);
  int i;
  for (i = 0; i < numDatasets; i++) {
    // printf("%f, %f\n", Dataset[i].coordinates.g_lat, Dataset[i].coordinates.g_long);
    // get the difference
    x_pos = (-88.202949f - Dataset[i].coordinates.g_long)/0.007f;
    y_pos = (35.0080284f - Dataset[i].coordinates.g_lat)/0.006f;
    // scale the pixel off the found top left corner. P
    if (worldTime < Dataset[i].numRows) {
      glVertex3f(243.f-x_pos-450.f, 843.f-y_pos-450.f, (Dataset[i].unitSales[worldTime])/30.f);
    }
  }
  glEnd();

  glBegin(GL_LINES);
  for (i = 0; i < numDatasets; i++) {
    x_pos = (-88.202949f - Dataset[i].coordinates.g_long)/0.007f;
    y_pos = (35.0080284f - Dataset[i].coordinates.g_lat)/0.006f;
    // scale the pixel off the found top left corner. :P
    // TODO need to redo this map based on image size, not 900x900
    if (worldTime < Dataset[i].numRows) {
      glVertex3f(243.f-x_pos-width/2.f, 843.f-y_pos-height/2.f, 0.f);
      glVertex3f(243.f-x_pos-width/2.f, 843.f-y_pos-height/2.f, (Dataset[i].unitSales[worldTime])/30.f);
    }
  }
  glEnd();
	glutSwapBuffers();
}

void update(int value) {
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
  if (play) {
    worldTime--;
    if (worldTime==0) {
      worldTime=maxRows;
    }
  }
  printf("%d: Date is: %f\n", worldTime, Dataset[longest-1].date[worldTime]);
}

void data_read(string inputfile) {
  struct GPScoord tmpCoords;
  fstream input;

  // need to get the # of rows
  // this is bastardized code from stack overflow to do that.
  FILE *f=fopen(inputfile.c_str(),"rb");
  int rows=0,b;while ((b=fgetc(f))!=EOF) rows+=(b==10)?1:0;fseek(f,0,SEEK_SET);
  if (rows > maxRows) {
    maxRows = rows;
    longest = numDatasets;
  }

  float * date_b         = new float[rows];
  float * unitsales_b    = new float[rows];
  float * sellingPrice_b = new float[rows];
  float * averageDays_b  = new float[rows];
  float * totalForSale_b = new float[rows];
  input.open(inputfile.c_str());

  // read in the coords
  input >> tmpCoords.g_lat;
  input >> tmpCoords.g_long;

  // fill in the array
  int curRow=0;
  while (curRow<rows+1) {
    // read in the five columns
    input >> date_b[curRow];
    input >> unitsales_b[curRow];
    input >> sellingPrice_b[curRow];
    input >> averageDays_b[curRow];
    input >> totalForSale_b[curRow];
    curRow++;
  }

  // fill the datastructure
  struct dataSet tmpData;
  tmpData.date = date_b;
  tmpData.unitSales = unitsales_b;
  tmpData.averageSellingPrice = sellingPrice_b;
  tmpData.averageDays = averageDays_b;
  tmpData.totalForSale = totalForSale_b;
  tmpData.coordinates = tmpCoords;
  tmpData.numRows = rows;
  Dataset[numDatasets] = tmpData;

  // printf("Done! Last date\n\n value read: %f, unitSales: %f, sellingPrice: %f, totalForSale: %f \n\n", Dataset[numDatasets].date[curRow-1], Dataset[numDatasets].unitSales[curRow-1],Dataset[numDatasets].averageDays[curRow-1],Dataset[numDatasets].totalForSale[curRow-1]);
  input.close();
}

int main(int argc, char** argv) {
  // TODO validate this, add a scaling factor
  Dataset = new struct dataSet[argc];
  if (argc==1) {
      printf("reading data/Birmingham.data...\n");
      data_read("data/Birmingham.data");
    }
  else {
    int i;
    for (i = 1; i < argc; i++) {
      printf("reading in %s...\n", argv[i]);
      data_read(argv[i]);
      numDatasets++;
    }
  }
  numDatasets=argc;

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

