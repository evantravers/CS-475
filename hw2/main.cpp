#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"

using namespace std;

// for storing vertexes


float _angle = 60.0f;

void cleanup() {
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			cleanup();
			exit(0);
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
	gluPerspective(30.0, (double)w / (double)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	glColor3f(0.3f, 0.9f, 0.0f);

  // this is where you should draw your objects
  // there had better be code here by tomorrow or I will make you sad.
  // I should take the eggMesh that I hopefully created, consisting of two solid objects 
  // derived from the slices of the egg I took 
  
  glBegin(GL_POINTS);
    // crawl the images
    int i;
    for (i = 0; i < 9; i++) {
      char* filename;
      sprintf(filename,"/data/egg%c.bmp",(char)i);
      Image* image = loadBMP(filename);
      int width = image->width;
      int height = image->height;

      int z = 0;
      int x, y;
      for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
          unsigned char redcolor = (unsigned char)image ->pixels[3*(z * image->width + x)];
          unsigned char greencolor = (unsigned char)image ->pixels[3*(z * image->width + x)+1];
          unsigned char bluecolor = (unsigned char)image ->pixels[3*(z * image->width + x)+2];
          printf("%c, %c, %c\n", redcolor, greencolor, bluecolor);
          // if the color is close to white, then draw egg
          if (0) {
            /* code */
            glVertex3f((float)x, (float)y, z);
          }
        }
      }  
    }
  glEnd();


	glutSwapBuffers();
}

void update(int value) {
	_angle += 1.0f;
	if (_angle > 360) {
		_angle -= 360;
	}
	
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	
	glutCreateWindow("eggs are ridiculous");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}

