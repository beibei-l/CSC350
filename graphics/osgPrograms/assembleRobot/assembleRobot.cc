
// Create a simple robot arm consisting of a cylinder and capsule

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgDB/WriteFile>
#include <string>
#include <stdlib.h>  // exit()
#include <iostream>

using namespace std;

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

int main()
{
  // program to create the robot and write the node tree to a file

  // create the robot
  osg::ref_ptr<osg::Node> root = createRobot("robot");

  std::string filename;

  // make sure the tree was was created OK
  if (!root.valid())
    {
      cout << "Error creating robot" << endl;
      exit(-1);
    }

  // get the file name from the user and create the file
  cout << "Please enter the name of the file to write: ";
  cin >> filename;
  if (!(osgDB::writeNodeFile( *(root.get()), filename)))
    {
      cout << "Error writing file :-(" << endl;
      exit (-1);
    }
  else
    cout << "Success!" << endl;
}


  
