//
// File: tiffTextureDemo.cc
//
// Author: Douglas Harms
//
// Description:  This program demonstrates simple OpenGL textures.  It reads
// a tiff image and uses it as the texture.  The image is scaled to be a
// 256x256 texture map.
//
//
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>
#include <tiffio.h>

using namespace std;

// define global state variables

// theta, phi, and dist are the eye position in 3d polar coordinates
float theta=-30;       // eye position to z-axis
float phi=30;        // eye position to x-z plane
float dist=5.0;

GLubyte tex[256][256][3];  // texture

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
  unsigned int w, h;
  int npixels;
  uint32* raster;
  GLubyte *image;
  int rowWidth;
  char filename[100];
  TIFF *tif;

  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // shade polygons as flat surfaces
  glShadeModel(GL_FLAT);

  // do depth testing
  glEnable (GL_DEPTH_TEST);

  // get the filename from the user and open the file
  do {
    cout << "Please enter the tiff image filename ";
    cin >> filename;
    tif = TIFFOpen(filename, "r");
    if (tif == NULL)
      cout << "That file doesn't exist - please try again" << endl;
  } while (tif==NULL);

  // process the file
  // get the image dimensions
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
  npixels = w * h;
  
  // allocate tiff image buffer
  raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
  
  // calculate the width of a row in the image.  The width (for openGL) must
  // be a multiple of 4 bytes
  rowWidth = 3*w;
  if ((rowWidth & 0x3) != 0)
    rowWidth += 4-(rowWidth & 0x3);
  
  // allocate memory for the openGL image
  image = new GLubyte[h*rowWidth];
  
  // proceed if we got all the memory we need
  if (raster != NULL && image != NULL) {
    // read the tiff image
    if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {
      
      // define the openGL image from the tiff image
      for (int r=0;r<h;r++)
	for (int c=0;c<w;c++) {
	  image[r*rowWidth + c*3 + 0] = TIFFGetR(raster[r*w+c]);
	  image[r*rowWidth + c*3 + 1] = TIFFGetG(raster[r*w+c]);
	  image[r*rowWidth + c*3 + 2] = TIFFGetB(raster[r*w+c]);
	}
    }
    
    // create 256x256 texture from openGL image
    gluScaleImage (GL_RGB,w,h,GL_UNSIGNED_BYTE,image,
		   256,256,GL_UNSIGNED_BYTE,tex);
    
    // free all of the dynamic memory we allocated
    _TIFFfree(raster);
    delete[] image;
  }
  // close the tiff file
  TIFFClose(tif);

  // initialize textures
  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB,GL_UNSIGNED_BYTE,
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

  // draw the textured polygons
  glEnable(GL_TEXTURE_2D);
  glColor3f (0.0, 1.0, 0.0);
  glBegin (GL_QUADS);
  glTexCoord2f (0.0, 0.0); glVertex3f (-1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 0.0); glVertex3f (1.0, -1.0, 0.0);
  glTexCoord2f (1.0, 1.0); glVertex3f (1.0, 1.0, 0.0);
  glTexCoord2f (0.0, 1.0); glVertex3f (-1.0, 1.0, 0.0);
  glEnd();
  
  glColor3f (1.0, 0.0, 0.0);
  glBegin (GL_TRIANGLES);
  glTexCoord2f (0.0, 0.0); glVertex3f (0.0, 0.0, 1.0);
  glTexCoord2f (1.0, 0.0); glVertex3f (0.0, 0.0, 0.0);
  glTexCoord2f (0.5, 1.0); glVertex3f (0.0, 1.0, 0.5);
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

