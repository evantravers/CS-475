#include <iostream>
#include <stdlib.h>
#include <math.h>

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


float _angle = 30.0f;

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
	gluPerspective(50.0, (double)w / (double)h, 1.0, 900.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -950.0f);
	glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
  // this is where you should draw your objects
  // there had better be code here by tomorrow or I will make you sad.
  // I should take the eggMesh that I hopefully created, consisting of two solid objects 
  // derived from the slices of the egg I took 
  
  glBegin(GL_POINTS);
    // crawl the images
    int i=1;
    for (i = 1; i < 10; i++) {
      // sprintf(filename,"data/%i.bmp",i);
      // printf("%s\n", filename);
      Image* image;
      if (i==1) {
        image = loadBMP("data/1.bmp");
      }
      if (i==2) {
        image = loadBMP("data/2.bmp");
      }
      if (i==3) {
        image = loadBMP("data/3.bmp");
      }
      if (i==4) {
        image = loadBMP("data/4.bmp");
      }
      if (i==5) {
        image = loadBMP("data/5.bmp");
      }
      if (i==6) {
        image = loadBMP("data/6.bmp");
      }
      if (i==7) {
        image = loadBMP("data/7.bmp");
      }
      if (i==8) {
        image = loadBMP("data/8.bmp");
      }
      if (i==9) {
        image = loadBMP("data/9.bmp");
      }
      int width = image->width;
      int height = image->height;
      int y = i;
      int x, z;
      for (x = 0; x < width; x++) {
        for (z = 0; z < height; z++) {
          unsigned char redcolor = 0;
          unsigned char bluecolor = 0;
          redcolor = (unsigned char)image ->pixels[3*(z * image->width + x)];
          // unsigned char redcolor = (unsigned char)image ->pixels[i];
          // unsigned char greencolor = (unsigned char)image ->pixels[3*(z * image->width + x)+1];
          bluecolor = (unsigned char)image ->pixels[3*(z * image->width + x)+2];
          // printf("%f, %f, %f\n", (float)redcolor/255.0f, (float)greencolor/255.0f, (float)bluecolor/255.0f);
          // if the color is close to white, then draw egg
          // if red is really low, then background
          // if red is high, and blue is high, then egg
          // if red is high, and blue is around 100, then yolk
          int red = (int)redcolor;
          int blue = (int)bluecolor;
          if (red>220) {
            // then we are inside the egg
            if (blue>110) {
              // we are looking at egg white
              glColor3f(9.5f, 9.5f, 9.5f);
            }
            else {
              // this should be yolk
              glColor3f(0.7f, 0.9f, 0.1f);
            }
            glVertex3i(x, y*12, z);
          }
          else {
            // glColor3f(0.0f, 0.0f, 0.9f);
            //             glVertex3i(x, y*12, z);
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

