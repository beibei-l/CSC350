
#include <iostream>
#include <GL/glut.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

using namespace std;

// define global variables
osg::ref_ptr<osgViewer::Viewer> viewer;
osg::observer_ptr<osgViewer::GraphicsWindow> window;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30*M_PI/180.0;       // eye position to z-axis
static float phi=30*M_PI/180.0;          // eye position to x-z plane
static float dist=5.0;

void setCamera(void)
{
  // sets the camera position in the world
  osg::Matrix m;

  // set the view transformation
  m.makeTranslate (0.0, 0.0, -dist);
  m.preMult (osg::Matrix::rotate(phi,osg::Vec3(1.0,0.0,0.0)));
  m.preMult (osg::Matrix::rotate(theta,osg::Vec3(0.0,1.0,0.0)));

  viewer->getCamera()->setViewMatrix (m);
}

void display(void)
{
  // update and render the scene graph
  if (viewer.valid())
    {
      setCamera();
      viewer->frame();
  
      // Swap Buffers
      glutSwapBuffers();
    }
}

void reshape( int w, int h )
{
  // update the window dimensions, in case the window has been resized.
  if (window.valid()) 
    {
      window->resized(window->getTraits()->x, window->getTraits()->y, w, h);
      viewer->getCamera()->setProjectionMatrixAsPerspective
	(60.0, (GLfloat)w/(GLfloat)h, 0.01, 20.0);
    }
}

void keyboard( unsigned char key, int /*x*/, int /*y*/ )
{
  switch( key )
    {
    case '4': // left
      theta += 5*M_PI/180.0;
      glutPostRedisplay();
      break;
    case '6': //right
      theta -= 5*M_PI/180.0;
      glutPostRedisplay();
      break;
    case '2': //down
      phi -= 5*M_PI/180.0;
      if (phi < -M_PI/2.0)
	phi = -M_PI/2.0;
      glutPostRedisplay();
      break;
    case '8': //up
      phi += 5*M_PI/180.0;
      if (phi > M_PI/2.0)
	phi = M_PI/2.0;
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
      
    case 27:
    case 'Q':
    case 'q':
      // clean up the viewer 
      if (viewer.valid())
	viewer = 0;
      glutDestroyWindow(glutGetWindow());
      exit(0);
      break;
    }
}

int main( int argc, char **argv )
{
  glutInit(&argc, argv);
  
  if (argc != 2)
    {
      cout << "Usage:" << argv[0] << " filename" << endl;
      return -1;
    }

  // load the scene.
  osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(argv[1]);
  if (!loadedModel.valid())
    {
      cout << "Error reading file " << argv[1] << endl;
      return -1;
    }
  
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA );
  glutInitWindowPosition( 100, 100 );
  glutInitWindowSize( 800, 600 );
  glutCreateWindow( argv[0] );
  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard );
  
  // create the view of the scene.
  viewer = new osgViewer::Viewer;
  window = viewer->setUpViewerAsEmbeddedInWindow(100,100,800,600);
  viewer->setSceneData(loadedModel.get());
  viewer->setThreadingModel (osgViewer::Viewer::SingleThreaded);
  viewer->realize();
  
  glutMainLoop();
  
  return 0;
}
