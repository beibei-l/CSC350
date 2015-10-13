//
// Program: assembleAndView.cc
// Author: Douglas Harms
//
// Creates a simple robot and displays it
//

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <GL/glut.h>
#include <string>
#include <stdlib.h>  // exit()
#include <iostream>

using namespace std;

// define global variables used for viewing
osg::ref_ptr<osgViewer::Viewer> viewer;
osg::observer_ptr<osgViewer::GraphicsWindow> window;

// theta, phi, and dist are the eye position in 3d polar coordinates
static float theta=-30*M_PI/180.0;       // eye position to z-axis
static float phi=30*M_PI/180.0;          // eye position to x-z plane
static float dist=5.0;

// define glut callback functions

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

// define functions to create the robot
osg::ref_ptr<osg::Node>
createArm(const std::string &name)
{
  osg::ref_ptr<osg::PositionAttitudeTransform> armArtPt = 
    new osg::PositionAttitudeTransform;

  // create the arm articulation point
  armArtPt->addDescription (name);

  {
    // Create the upper arm and attach it to the arm articulation point
    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder;
    cylinder->setRadius (0.3);
    cylinder->setHeight (1.0);
    cylinder->setCenter(osg::Vec3(0.0,0.0,0.5));
    
    osg::ref_ptr<osg::ShapeDrawable> shape;
    shape = new osg::ShapeDrawable(cylinder.get());
    shape->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( shape.get());
    armArtPt->addChild (geode.get());
    }
    {
    // Create a lower arm and attach it to the (upper) arm
    osg::ref_ptr<osg::Capsule> capsule = new osg::Capsule;
    capsule->setRadius (0.3);
    capsule->setHeight (1.0);
    capsule->setCenter (osg::Vec3(0.0, 0.0, 0.5));
     
    osg::ref_ptr<osg::ShapeDrawable> shape = 
      new osg::ShapeDrawable(capsule.get());
    shape->setColor(osg::Vec4(0.0, 0.0, 1.0, 1.0));
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable( shape.get());

    // Create lower arm geode and place it under an articulation point
    osg::ref_ptr<osg::PositionAttitudeTransform> artPt = 
      new osg::PositionAttitudeTransform;
    artPt->addDescription(name+".lowerArm");
    artPt->addChild (geode.get());

    // now attach the lower arm relative to the upper arm
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = 
      new osg::PositionAttitudeTransform;
    pat->setPosition (osg::Vec3(0.0, 0.0, 1.0));
    pat->setAttitude(osg::Quat(M_PI/2.0,osg::Vec3(1.0,0.0,0.0)));
    pat->addChild (artPt.get());

    armArtPt->addChild (pat.get());
    }

    return armArtPt.get();
}

osg::ref_ptr<osg::Node>
createRobot(const std::string &name)
{
  osg::ref_ptr<osg::PositionAttitudeTransform> robot = 
    new osg::PositionAttitudeTransform;

  // set up the articulation point
  robot->addDescription(name);

  {
    // build the torso and attach it to the robot
    osg::ref_ptr<osg::Box> torso = new osg::Box;
    torso->setHalfLengths (osg::Vec3(1.0,1.0,1.0));
    
    osg::ref_ptr<osg::ShapeDrawable> shape = 
      new osg::ShapeDrawable (torso.get());
    shape->setColor (osg::Vec4(1.0, 0.0, 1.0,1.0));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable (shape.get());

    robot->addChild (geode.get());
  }

  {
    // build the first arm and attach it to the robot (+z)
    osg::ref_ptr<osg::Node> arm = createArm(name+".arm1");

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = 
      new osg::PositionAttitudeTransform;
    pat->setPosition (osg::Vec3(0.0, 0.0, 1.0));
    pat->addChild (arm.get());

    robot->addChild (pat.get());
  }

  {
    // build the second arm and attach it to the robot (+x)
    osg::ref_ptr<osg::Node> arm = createArm(name+".arm2");

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = 
      new osg::PositionAttitudeTransform;
    pat->setPosition( osg::Vec3(1.0, 0.0, 0.0));
    pat->setAttitude(osg::Quat(M_PI/2.0,osg::Vec3(0.0,1.0,0.0)));
    pat->addChild (arm.get());

    robot->addChild (pat.get());
  }

  {
    // build the third arm and attach it to the robot (-z)
    osg::ref_ptr<osg::Node> arm = createArm(name+".arm3");

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = 
      new osg::PositionAttitudeTransform;
    pat->setPosition (osg::Vec3(0.0,0.0,-1.0));
    pat->setAttitude(osg::Quat(M_PI,osg::Vec3(0.0,1.0,0.0)));
    pat->addChild (arm.get());

    robot->addChild (pat.get());
  }

  {
    // build the fourth arm and attach it to the robot (-x)
    osg::ref_ptr<osg::Node> arm = createArm(name+".arm4");

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = 
      new osg::PositionAttitudeTransform;
    pat->setPosition (osg::Vec3(-1.0,0.0,0.0));
    pat->setAttitude (osg::Quat(-M_PI/2.0,osg::Vec3(0.0,1.0,0.0)));
    pat->addChild (arm.get());

    robot->addChild (pat.get());
  }

  return robot.get();
}

int main( int argc, char **argv )
{
  glutInit(&argc, argv);

  // create the robot
  osg::ref_ptr<osg::Node> root = createRobot("robot");

  std::string filename;

  // make sure the tree was was created OK
  if (!root.valid())
    {
      cout << "Error creating robot" << endl;
      exit(-1);
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
  viewer->setSceneData(root.get());
  viewer->setThreadingModel (osgViewer::Viewer::SingleThreaded);
  viewer->realize();
  
  glutMainLoop();
  
  return 0;
}


  
