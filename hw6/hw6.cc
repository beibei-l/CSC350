//
// File: hw6.cc
//
//  Author: 
//
// This program defines the vertices, normals, and triangles for several
// geometric shapes, stores these in GPU buffers, then draws several shapes
// in the world.
//

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
#include <iostream>

using namespace std;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30;       // eye position to z-axis
static float phi=30;          // eye position to x-z plane
static float dist=5.0;

// buffer ids for the cylinder
GLuint sphereVertexBuffer, sphereNormalBuffer, sphereIndexBuffer;
int sphereNumIndices;
GLuint pyramidVertexBuffer, pyramidColorBuffer, pyramidIndexBuffer;
int pyramidNumIndices;

// some global state information
bool animate;
float pyramidTheta;

void setCamera(void)
{
  // sets the camera position in the world

  // set the view transformation
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -dist);
  glRotatef(phi, 1.0, 0.0, 0.0);
  glRotatef(theta, 0.0, 1.0, 0.0);
}

// define the vertices, normals, and triangles for a unit sphere centered
// around the origin, and store these in GPU buffers.  Global variables
// sphereVertexBuffer, sphereNormalBuffer, sphereIndexBuffer, and 
// sphereNumIndices will be set appropriately.
void defineUnitSphere( int numSides, int numStacks )
{
    double theta;
    double deltaTheta = 2.0 * M_PI / numSides;
    double xLeft, yLeft, xRight, yRight;
    double x, y, z;
    double deltaZ = 2.0 / numStacks;
    int pos;
    
    // define the vertices and normals
    GLfloat vertices[numSides*(numStacks+1)][3];
    GLfloat normals[numSides*(numStacks+1)][3];
    
    // the first line will be at theta=0
    theta = 0.0;
    pos = 0;  // start filling array at element 0
    for( int i=0; i<numSides; i++ ) {
        // calculate the x and y coordinates of the vertices along this line
        x = cos( theta );
        y = sin( theta );
        
        // z will start at the top of the cylinder
        z = 1.0;
        
        // define the vertices and normals along this line
        for( int j=0; j<numStacks+1; j++ ) {
            vertices[pos][0] = x;
            vertices[pos][1] = y;
            // make sure last vertex is at z = -1.0
            if( j < numStacks )
                vertices[pos][2] = z;
            else
                vertices[pos][2] = -1.0;
            
            normals[pos][0] = x;
            normals[pos][1] = y;
            normals[pos][2] = 0.0;
            
            pos++;
            
            // calculate z value for next iteration
            z -= deltaZ;
        }
        
        // increment theta for the next line
        theta += deltaTheta;
    }
    
    // save the vertices and normals in the GPU
    glGenBuffers( 1, &sphereVertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, sphereVertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    
    glGenBuffers( 1, &sphereNormalBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, sphereNormalBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW );
    
    // define the indices
    sphereNumIndices = 6 * numStacks * numSides;
    GLuint vertexIndices[sphereNumIndices];
    
    pos = 0;
    for( int i=0; i<numSides; i++ ) {
        // define the indices of the first vertex on the left and right
        int leftBase = i*(numStacks+1);
        int rightBase = i<(numSides-1) ? (i+1)*(numStacks+1) : 0;
        for( int j=0; j<numStacks; j++ ) {
            // define the first triangle
            vertexIndices[pos++] = leftBase;
            vertexIndices[pos++] = leftBase + 1;
            vertexIndices[pos++] = rightBase + 1;
            
            // define the second triangle
            vertexIndices[pos++] = leftBase;
            vertexIndices[pos++] = rightBase + 1;
            vertexIndices[pos++] = rightBase;
            
            // update bases for next stack
            leftBase++;
            rightBase++;
        }
    }
    
    // store indices in GPU
    glGenBuffers( 1, &sphereIndexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sphereIndexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices), vertexIndices,
                 GL_STATIC_DRAW );
}

// display the sphere whose attributes have been stored in GPU buffers
// sphereVertexBuffer, sphereNormalBuffer, and sphereIndexBuffer, with
// sphereNumIndices indices.
void displaySphere()
{
    // enable vertices
    glEnableClientState( GL_VERTEX_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, sphereVertexBuffer );
    glVertexPointer( 3, GL_FLOAT, 0, 0 );
    
    // enable normals
    glEnableClientState( GL_NORMAL_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, sphereNormalBuffer );
    glNormalPointer( GL_FLOAT, 0, 0);
    
    // bind the index array
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sphereIndexBuffer );
    
    // now draw the cylinder
    glDrawElements( GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_INT, 0 );
    
    // disable things that we enabled
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    
    // no buffers selected now
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
}

// define the vertices, normals, and triangles for a pyramid centered
// around the origin, and store these in GPU buffers.  Global variables
// pyramidVertexBuffer, pyramidColorBuffer, pyramidIndexBuffer, and
// pyramidNumIndices will be set appropriately.
void definePyramid( )
{
    // Define the vertices, colors, and triangle indices for a pyramid with the apex at (0,1,0) and base verteices at (-1,-1,1) (1,-1,1) (1,-1,-1) and (-1,-1,-1). Normals are not defined for the vertices, but an unique color must be defined for each vertex. The vertices, colors, and triangle indices must be stored in the GPU, and global variables pyramidVertexBuffer, pyramidColorBuffer, pyramidndexBuffer, and pyramidNumIndices must be appropriately defined.
    
    GLfloat vertices[][3] = {
        {0,1,0}, {-1,-1,1}, {1,-1,1}, {1,-1,-1},
        {-1,-1,-1}
    };
    GLfloat colors[][3] = {
        {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
        {1, 0, 0}
    };
    GLuint indices[][3] = {
        {0,1,2}, //front
        {0,4,3}, // back
        {0,1,4}, // left
        {0,2,3}, // right
        {1,2,3}, {3,4,1}  // bottom
    };
    
    // store vertices in GPU
    glGenBuffers( 1, &pyramidVertexBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, pyramidVertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // store vertex colors in GPU
    glGenBuffers( 1, &pyramidColorBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, pyramidColorBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    // store triangle indices in GPU
    glGenBuffers( 1, &pyramidIndexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pyramidIndexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW );
    
    pyramidNumIndices = 18;
    
}

// display the pyramid whose attributes have been stored in GPU buffers
// pyramidVertexBuffer, pyramidColorBuffer, and pyramidIndexBuffer, with
// pyramidNumIndices indices.
void displayPyramid()
{
    // enable vertices
    glEnableClientState( GL_VERTEX_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, pyramidVertexBuffer );
    glVertexPointer( 3, GL_FLOAT, 0, 0 );
    
    // enable vertex colors
    glEnableClientState( GL_COLOR_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, pyramidColorBuffer );
    glColorPointer( 3, GL_FLOAT, 0, 0 );
    
    // bind the index array
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pyramidIndexBuffer );
    
    // now draw the cylinder
    glDrawElements( GL_TRIANGLES, pyramidNumIndices, GL_UNSIGNED_INT, 0 );
    
    // disable things that we enabled
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    
    // no buffers selected now
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


void init(void)
{
  // define material property parameters
  GLfloat matSpecular[]  = {1.0, 1.0, 1.0, 1.0};
  GLfloat matShininess =   30.0;

  // define light property parameters
  GLfloat lightAmbient0[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat lightDiffuse0[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular0[]= {1.0, 1.0, 1.0, 1.0};
  
  GLfloat lightAmbient1[] = {0.0, 0.0, 0.2, 1.0};
  GLfloat lightDiffuse1[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat lightSpecular1[]= {1.0, 1.0, 1.0, 1.0};
  
  // define clear color to be black
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // set up ambient, diffuse, and specular components for the lights
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
  
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);

  // We'll use glColor to set the diffuse and ambient material properties
  glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable (GL_COLOR_MATERIAL);

  // define default material properties
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

  // two-sided
  glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, 1 );

  // let openGL calculate unit vector normals
  glEnable( GL_NORMALIZE );

  // lights 0 and 1 are enables
  glEnable( GL_LIGHT0 );
  glEnable( GL_LIGHT1 );

  // shade polygons smoothly
  glShadeModel(GL_SMOOTH);

  // do hidden surface removal
  glEnable (GL_DEPTH_TEST);

  // create the shapes and store attributes in teh GPU
  defineUnitSphere( 50, 50 );
  definePyramid();

  // define initial global state
  animate = false;
  pyramidTheta = 0.0;
}

void display(void)
{
  // define light positions
  GLfloat light0Pos[] = { 0.0, 2.0, 0.0, 1.0 };
  GLfloat light1Pos[] = { 0.5, 1.0, 0.0, 1.0 };

  // clear frame buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set viewpoint
  glLoadIdentity();

  // light1 is in camera frame
  glLightfv( GL_LIGHT1, GL_POSITION, light1Pos );

  // set the camera
  setCamera();

  // light0 is in world frame
  glLightfv( GL_LIGHT0, GL_POSITION, light1Pos );

  // let's draw the axes first
  // axes won't be lighted
  glDisable( GL_LIGHTING );

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

  // other objects in the world will be lighted
  glEnable( GL_LIGHTING );

  // draw some spheres and pyramids
  glColor3f( 1.0, 0.0, 0.0 );
  displaySphere();

  glPushMatrix();
  glRotatef( -90.0, 1, 0, 0 );
  glTranslatef( 1.5, 1.0, -0.5 );
  glScalef( 0.5, 0.5, 1.0 );
  glColor3f( 0.0, 0.0, 1.0 );
  displaySphere();
  glPopMatrix();

  // Normals weren't defined for the pyramid so turn lighting off
  glDisable( GL_LIGHTING );
  glPushMatrix();
  glRotatef( pyramidTheta, 0, 1, 0 );
  glTranslatef( 2.0, 0, 0 );
  glRotatef( pyramidTheta*10, 0, 1, 0 );
  displayPyramid();
  glPopMatrix();

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

void timer( int val )
{
  if( !animate )
    return;

  glutTimerFunc( 100, timer, 0 );

  pyramidTheta += 2.0;
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 27: /* esc */
  case 'Q':
  case 'q':
    exit(0);
    break;

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
    // don't let elevation go past 90 or -90
    if (phi>90.0)
      phi = 90.0;
    else if (phi < -90.0)
      phi = -90.0;
    glutPostRedisplay();
    break;
  case '8': //up
    phi += 5;
    // don't let elevation go past 90 or -90
    if (phi>90.0)
      phi = 90.0;
    else if (phi < -90.0)
      phi = -90.0;
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

  case 'a':
  case 'A':
    animate = !animate;
    if( animate )
      glutTimerFunc( 100, timer, 0 );
    break;
  }
}

// onExit will be invoked when the user quits the program
void onExit( void ) 
{
  // clear buffers from gpu
  glDeleteBuffers( 1, &sphereVertexBuffer );
  glDeleteBuffers( 1, &sphereNormalBuffer );
  glDeleteBuffers( 1, &sphereIndexBuffer );
  glDeleteBuffers( 1, &pyramidVertexBuffer );
  glDeleteBuffers( 1, &pyramidColorBuffer );
  glDeleteBuffers( 1, &pyramidIndexBuffer );

}

int main(int argc, char** argv)
{
  // initialize glut, create window, etc.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Color Cube!");

  // initialize glew
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    cout << "Error: " << glewGetErrorString(glew_status) << endl;
    return 1;
  }

  // initialize program data structures
  init();

  // register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // callback to clean up things on exit
  atexit( onExit );

  // do everything!
  glutMainLoop();

  return 0;

}

