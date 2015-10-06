//
// File: textureDemo.cc
// Author: Douglas Harms
// Description:  This program demonstrates simple OpenGL textures
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

GLubyte tex[128][128][3];  // texture

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
  int c;

  // define clear color to be black
  glClearColor(1.0, 1.0, 1.0, 0.0);

  // shade polygons as flat surfaces
  glShadeModel(GL_FLAT);

  // do depth testing
  glEnable (GL_DEPTH_TEST);

  // initialize texture
  for (int i=0;i<128;i++)
    for (int j=0;j<128;j++) {
      if (((i/8)+(j/8))%2==0)
      	c = 0;
      else
      	c = 255;
      tex[i][j][0] = (GLubyte) c;
      tex[i][j][1] = (GLubyte) c;
      tex[i][j][2] = (GLubyte) c;
    }
  
  // initialize textures
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB,GL_UNSIGNED_BYTE,
	       tex);
  
  //gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, 64, 64, GL_RGB, GL_UNSIGNED_BYTE,
  //tex);
  glShadeModel (GL_FLAT);
  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setCamera();

  // draw things in the world

  // draw the textured polygons
  glEnable(GL_TEXTURE_2D);
  glColor3f (1.0, 1.0, 1.0);
  glBegin (GL_POLYGON);
  glTexCoord2f (0.0, 0.0); glVertex3f (-1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 0.0); glVertex3f (1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 1.0); glVertex3f (0.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0); glVertex3f (-1.0, 1.0, 0.0);
  glEnd();
 
  // display things  
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.01, 20.0);
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
  }
}

int main(int argc, char** argv)
{
  // initialize glut, create window, etc.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Very Simple!");

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

