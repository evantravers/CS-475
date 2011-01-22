#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h> //Needed for "exit" function

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;
vector<float> v;
float val;

//Called when a key is pressed
void handleKeypress(unsigned char key, //The key that was pressed
					int x, int y) {    //The current mouse coordinates
	switch (key) {
		case 27: //Escape key
			exit(0); //Exit the program
	}
}
//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
				   (double)w / (double)h, //The width-to-height ratio
				   1.0,                   //The near z clipping coordinate
				   900.0);                //The far z clipping coordinate
}

//Draws the 3D scene
void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective

  glPushMatrix();
  glTranslatef(0.f, 0.f, 5.f);

	glLoadIdentity(); //Reset the drawing perspective
	
	glBegin(GL_QUADS); 
  int i;
  glVertex3f(0.f, 0.f, 1.f);
  glVertex3f(1.f, 0.f, 1.f);
  glVertex3f(0.f, 1.f, 1.f);
  glVertex3f(1.f, 1.f, 1.f);
  /*for (i = 0; i < ((int) v.size())-1; i++) {
    glVertex3f((float) i, v.at(i), v.at(i+1));
  }*/
	glEnd(); //End quadrilateral coordinates
	
  glPopMatrix();
	glutSwapBuffers(); //Send the 3D scene to the screen
}

int main(int argc, char** argv) {
  // read in files
  
  // config file
  fstream config;
  config.open("config.ini");
  config >> val;
  config.close();

  // input
  float temp;
  fstream input;
  input.open("input.txt");
  string line;
  getline(input, line);
  while (!input.eof()) {
    input >> temp;
    // i am currently dividing by 100, may not choose to do that
    temp = temp/100;
    v.push_back(temp);
    printf("%f, %i\n", temp, (int)v.size());
  }

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size
	
	//Create the window
	glutCreateWindow("hw1");
	initRendering(); //Initialize rendering
	
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	
	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}
