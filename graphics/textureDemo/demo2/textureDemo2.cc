//
// File: textureDemo2.cc
// Author: Douglas Harms
// Description:  This program demonstrates simple OpenGL textures.  It
// demonstrates texture mapping options, similar to that depicted in 
// Angel Figure 8.32
//
//
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>

using namespace std;

// define global state variables

// theta, phi, and dist are the eye position in 3d polar coordinates
float theta=-30;       // eye position to z-axis
float phi=30;        // eye position to x-z plane
float dist=5.0;

GLubyte tex[256][256][3];  // texture

char option = 'a';

void setCamera(void)
{
  // don't let elevation go past 90 or -90
  if (phi>90.0)
    phi = 90.0;
  else if (phi < -90.0)
    phi = -90.0;
  
  // set the view transformation
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -dist);
  glRotatef(phi, 1.0, 0.0, 0.0);
  glRotatef(theta, 0.0, 1.0, 0.0);
}

void init(void)
{
  int val;
  int c;

  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // shade polygons as flat surfaces
  glShadeModel(GL_FLAT);

  // do depth testing
  glEnable (GL_DEPTH_TEST);

  // initialize texture
  val = 0;
  for (int i=0;i<32;i++)
    {
      val = ~val;
      for (int j=0;j<8;j++)
	{
	  c = 8*i+j;
	  for (int r=0;r<256;r++)
	    {
	      tex[r][c][0] = (GLubyte) val;
	      tex[r][c][1] = (GLubyte) val;
	      tex[r][c][2] = (GLubyte) val;
	    }
	}
    }
  
  // initialize textures
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE,
	       tex);
  
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE,
		     tex);
  glShadeModel (GL_FLAT);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setCamera();

  // draw things in the world

  // let's draw the axes first, but not textured
  glDisable (GL_TEXTURE_2D);

  glBegin(GL_LINES);
  // x in red
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(2.0,0.0,0.0);

  // y in green
  glColor3f (0.0, 10.0, 0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f (0.0,2.0,0.0);

  // z in blue
  glColor3f (0.0, 0.0, 1.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f (0.0,0.0,2.0);
  glEnd();

  // define texture parameters
  switch (option) {
  case 'a':
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    break;
  case 'b':
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
  case 'c':
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		     GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		     GL_NEAREST_MIPMAP_NEAREST);
    break;
  case 'd':
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		     GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		     GL_NEAREST_MIPMAP_LINEAR); 
    break;

  }

  // draw the textured polygons
  glEnable(GL_TEXTURE_2D);
  glColor3f (0.0, 1.0, 0.0);
  glBegin (GL_QUADS);
  glTexCoord2f (0.0, 0.0); glVertex3f (-1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 0.0); glVertex3f (1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 1.0); glVertex3f (1.0, 50.0, 0.0);
  glTexCoord2f (0.0, 1.0); glVertex3f (-1.0, 50.0, 0.0);
  glEnd();
  
  // display things  
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.01, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case '4': // left
    theta += 5;
    glutPostRedisplay();
    break;
  case '6': //right
    theta -= 5;
    glutPostRedisplay();
    break;
  case '2': //down
    phi -= 5;
    glutPostRedisplay();
    break;
  case '8': //up
    phi += 5;
    glutPostRedisplay();
    break;
  case '3': //pgDn (back up)
    dist += 0.5;
    glutPostRedisplay();
    break;
  case '9': //pgUp (move forward)
    dist -= 0.5;
    glutPostRedisplay();
    break;
    
  case 27: /* esc */
  case 'Q':
  case 'q':
    exit(0);
    break;

  case 'a':
  case 'b':
  case 'c':
  case 'd':
    option = key;
    glutPostRedisplay();
    break;

  }
}

int main(int argc, char** argv)
{
  // initialize glut, create window, etc.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(512,512);
  glutInitWindowPosition(100,100);
  glutCreateWindow("TextureDemo2");

  // initialize program data structures
  init();

  // register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // do everything!
  glutMainLoop();

  return 0;

}

