//
// File: demo3.cc
// Author: Douglas Harms
// Description: This is an enhancement of demo2.  A rotating and moving square
// is placed within the unit square, and several rotating and moving squares
// are placed within this square.
//
//
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <time.h>   // time (for random seed)
#include <stdlib.h> // random number stuff (srand, rand)
#include <iostream>

using namespace std;

// define global state variables

struct squareInfo {
  float size;            // size of square
  float xPos,yPos;       // position of the center of the square
  float deltaX,deltaY;   // velocity of the square
  float psi;             // current orientation of square
  float deltaPsi;        // angular velocity of square
};

squareInfo bouncingSquare;
squareInfo innerSquare [5];
int numInner;

// theta, phi, and dist are the eye position in 3d polar coordinates
float theta=-30;       // eye position to z-axis
float phi=30;          // eye position to x-z plane
float dist=5.0;

bool animate;

void drawSquare()
{
  // draw a 2x2 square in the xy plane centered at the origin
  glBegin(GL_LINE_LOOP);
  glVertex2f (-1.0, -1.0);
  glVertex2f (-1.0, 1.0);
  glVertex2f (1.0, 1.0);
  glVertex2f (1.0, -1.0);
  glEnd ();
}

static double randomf(void)
{
  // return a random value between 0.0 and 1.0
  return ((double)rand())/RAND_MAX;
}

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
  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // shade polygons as flat surfaces
  glShadeModel(GL_FLAT);

  // initialize data structures
  // seed the random number generator
  srand (time (0));

  // initialize bouncing square
  // select a random starting position between -1 and +1
  bouncingSquare.xPos = randomf()*2.0-1.0;
  bouncingSquare.yPos = randomf()*2.0-1.0;
  
  // select random direction between -.1 and +.1
  bouncingSquare.deltaX = randomf()*0.2-0.1;
  bouncingSquare.deltaY = randomf()*0.2-0.1;
  
  // select random orientation between -90 and +90
  bouncingSquare.psi = randomf()*180.0 - 90.0;
  
  // select random rotation between -5 and +5 (degrees)
  bouncingSquare.deltaPsi = randomf()*10.0 - 5.0;
  
  // set the square size
  bouncingSquare.size = 0.1;

  // now initialize inner squares
  for (int i=0;i<5;i++)
    {
      // select a random starting position between -1 and +1
      innerSquare[i].xPos = randomf()*2.0-1.0;
      innerSquare[i].yPos = randomf()*2.0-1.0;
      
      // select random direction between -.1 and +.1
      innerSquare[i].deltaX = randomf()*0.2-0.1;
      innerSquare[i].deltaY = randomf()*0.2-0.1;
      
      // select random orientation between -90 and +90
      innerSquare[i].psi = randomf()*180.0 - 90.0;
      
      // select random rotation between -5 and +5 (degrees)
      innerSquare[i].deltaPsi = randomf()*10.0 - 5.0;
      
      // set the square size
      innerSquare[i].size = 0.2;
    }

  // no animation yet
  animate = false;

  // don't display any inner squares initially
  numInner = 0;

}

void display(void)
{
  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setCamera();

  // draw things in the world

  // let's draw the axes first
  glBegin(GL_LINES);
  // x in red
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(2.0,0.0,0.0);

  // y in green
  glColor3f (0.0, 1.0, 0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f (0.0,2.0,0.0);

  // z in blue
  glColor3f (0.0, 0.0, 1.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f (0.0,0.0,2.0);
  glEnd();

  // draw all squares in white
  glColor3f (1.0, 1.0, 1.0);

  // draw the bouncing square
  // set the transformation matrix after saving it
  glPushMatrix();
  glTranslatef (bouncingSquare.xPos, bouncingSquare.yPos, 0.0);
  glRotatef (bouncingSquare.psi, 0.0, 0.0, 1.0);
  glScalef (bouncingSquare.size, bouncingSquare.size, 1.0);
  
  // draw the inner squares
  for (int i=0;i<numInner;i++)
    {
      // set the transformation matrix after saving it
      glPushMatrix();
      glTranslatef (innerSquare[i].xPos, innerSquare[i].yPos, 0.0);
      glRotatef (innerSquare[i].psi, 0.0, 0.0, 1.0);
      glScalef (innerSquare[i].size, innerSquare[i].size, 1.0);
      
      // draw the inner square and restore the transformation matrix
      drawSquare ();
      glPopMatrix();
    }

  // draw the bouncing square and restore the transformation matrix
  drawSquare();
  glPopMatrix();
  
  // draw the bounding square
  drawSquare();
 
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

void timer(int val)
{
  // nothing to do if animation stopped
  if (!animate)
    return;

  // register timer to tick in 1/50th second
  glutTimerFunc (20,timer,0);

  // move the bouncing square
  bouncingSquare.xPos += bouncingSquare.deltaX;
  bouncingSquare.yPos += bouncingSquare.deltaY;
  
  // change the orientation
  bouncingSquare.psi += bouncingSquare.deltaPsi;
  
  // change the direction (and possibly speed) of the square if it is out
  // of bounds
  if (bouncingSquare.xPos+bouncingSquare.size > 1.0 ||
      bouncingSquare.xPos-bouncingSquare.size < -1.0)
    {
      bouncingSquare.xPos -= bouncingSquare.deltaX;
      bouncingSquare.deltaX = -bouncingSquare.deltaX;
      if (randomf()*100 < 15)  // 15% chance of change
	bouncingSquare.deltaX = randomf()*0.2-0.1;
      if (randomf()*100 < 15)  // 15% chance of change
	bouncingSquare.deltaPsi = randomf()*10.0-5.0;
    }
  if (bouncingSquare.yPos+bouncingSquare.size > 1.0 ||
      bouncingSquare.yPos-bouncingSquare.size < -1.0)
    {
      bouncingSquare.yPos -= bouncingSquare.deltaY;
      bouncingSquare.deltaY = -bouncingSquare.deltaY;
      if (randomf()*100 < 15)  // 15% chance of change
	bouncingSquare.deltaY = randomf()*0.2-0.1;
      if (randomf()*100 < 15)  // 15% chance of change
	bouncingSquare.deltaPsi = randomf()*10.0-5.0;
    }
  
  // adjust all displayed inner squares
  for (int i=0;i<numInner;i++)
    {
      // move the inner square
      innerSquare[i].xPos += innerSquare[i].deltaX;
      innerSquare[i].yPos += innerSquare[i].deltaY;
      
      // change the orientation
      innerSquare[i].psi += innerSquare[i].deltaPsi;
      
      // change the direction (and possibly speed) of the square if it is out
      // of bounds
      if (innerSquare[i].xPos+innerSquare[i].size > 1.0 ||
	  innerSquare[i].xPos-innerSquare[i].size < -1.0)
	{
	  innerSquare[i].xPos -= innerSquare[i].deltaX;
	  innerSquare[i].deltaX = -innerSquare[i].deltaX;
	  if (randomf()*100 < 15)  // 15% chance of change
	    innerSquare[i].deltaX = randomf()*0.2-0.1;
	  if (randomf()*100 < 15)  // 15% chance of change
	    innerSquare[i].deltaPsi = randomf()*10.0-5.0;
	}
      if (innerSquare[i].yPos+innerSquare[i].size > 1.0 ||
	  innerSquare[i].yPos-innerSquare[i].size < -1.0)
	{
	  innerSquare[i].yPos -= innerSquare[i].deltaY;
	  innerSquare[i].deltaY = -innerSquare[i].deltaY;
	  if (randomf()*100 < 15)  // 15% chance of change
	    innerSquare[i].deltaY = randomf()*0.2-0.1;
	  if (randomf()*100 < 15)  // 15% chance of change
	    innerSquare[i].deltaPsi = randomf()*10.0-5.0;
	} 
    }
  // we need to redraw it now
  glutPostRedisplay();
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
    
  case 'A': // toggle animation
  case 'a':
    animate = !animate;
    // register callback if we just turned animation on
    if (animate)
      glutTimerFunc (20,timer,0);
    break;

  case '+':
    bouncingSquare.size += 0.05;
    if (bouncingSquare.size > 0.75)
      bouncingSquare.size = 0.75;
    glutPostRedisplay ();
    break;
  case '-':
    bouncingSquare.size -= 0.05;
    if (bouncingSquare.size < 0.05)
      bouncingSquare.size = 0.05;
    glutPostRedisplay();
    break;

  case '!':  //shift 1
    numInner = 1;
    glutPostRedisplay ();
    break;
  case '@':  //shift 2
    numInner = 2;
    glutPostRedisplay ();
    break;
  case '#':  //shift 3
    numInner = 3;
    glutPostRedisplay ();
    break;
  case '$':  //shift 4
    numInner = 4;
    glutPostRedisplay ();
    break;
  case '%':  //shift 5
    numInner = 5;
    glutPostRedisplay ();
    break;
  case ')':  //shift 0
    numInner = 0;
    glutPostRedisplay ();
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

