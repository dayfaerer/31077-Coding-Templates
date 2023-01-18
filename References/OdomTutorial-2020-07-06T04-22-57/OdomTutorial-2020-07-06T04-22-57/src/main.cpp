/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftEncoder          encoder       A, B            
// RightEncoder         encoder       C, D            
// HorizontalEncoder    encoder       E, F            
// InternalSensor       inertial      1               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;



/////////////////////////////////////////////////////////////////////////
//SETTINGS
/////////////////////////////////////////////////////////////////////////

//Utilize these three variables to make sure they are reading correctly.
//The horizontal encoder should be positive if you push the robot laterally to the right
//The vertical encoders should be positive if you pusht the robot laterally forward
//Rotating the robot clockwise should yield a positive degree value
//NOTE: The recommended position of the vertical encoders(left and right) should be located at center of mass
bool flipLeftEncoder = false;
bool flipRightEncoder = true;
bool flipHorizontalEncoder = true;
bool flipRotations = false;

//This is how many miliseconds every cycle (recommended is 15)
int milisecondStepTime = 15;

//This is something you should utilize to convert the horizontal encoder distance to match the vertical encoder distance
//Grab a ruler, and push the robot 20 inches straight forward, then record the average vertical value (Example: 51269)
//Reset, Grab a ruler, and push the robot 20 inches to the right, then record the horizontal encoder value (Example: 10532)
//You will need to convert it such that the horizontal matches for the same distance, so the multiplier will be horizontal(10532)/vertical(51269)
//And that number will be the ratio (10532/51269 = 0.2054262809885116)
double verticalToHorizontalRatio = 1.0000000;

//Because we can average the vertical encoders while turning, it will not show a change
//But for the horizontal encoders, it will assume the robot is laterally strafing, which is not correct
//We fix this by applying a multiplier compensation such that we will negate it using the inertial sensor
//Reset your robot, make sure everything is calibrated
//Rotate your robot such that the inertial sensor reads 3600 degrees (i.e. rotate the robot 10 times for the sake of accuracy)
//Your horizontal encoder should have a value (Example: 231809)
//Divide that result by 3600 (231809/3600 = 64.3913888), and that number will be the value used
double rotateNegateHorizontalRatio = 50.000000;

//This is utilized to convert the ticks moved to actual inches
//Push the robot 20 inches forward (without turning it or pushing it strafe-wise)
//Get the average reading of the vertical encoders (Example: 253053)
//Divide the reading by 20 (253053/20 = 12,652.65), and put the multipler in this area
//This will convert it to inches
double posTranslationMultiplier = 1;
/////////////////////////////////////////////////////////////////////////







/////////////////////////////////////////////////////////////////////////
//Inclusions added manually
/////////////////////////////////////////////////////////////////////////
// Include additional mathematical operations
#include <cmath>
/////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////
//Functions added manually
/////////////////////////////////////////////////////////////////////////
// Waiting function that returns time actually waited (Credit to James Pearman (jpearman on VEXForum - https://www.vexforum.com/t/vexcode-sleep-help/82706/2?u=connor))
uint32_t wait( uint32_t time_mS ) {
    uint64_t start = Brain.Timer.systemHighResolution();
    this_thread::sleep_for(time_mS);
    return (uint32_t)(Brain.Timer.systemHighResolution() - start); 
}
// This converts degrees to radians
double getRadians(int degrees){
  return ( degrees * M_PI ) / 180 ;
}
/////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////
//Initialized variables for usage
/////////////////////////////////////////////////////////////////////////
// Include additional mathematical operations
double lastVerticalPosLocal = 0.0;
double lastHorizontalPosLocal = 0.0;
double XPos = 0.0;
double YPos = 0.0;
/////////////////////////////////////////////////////////////////////////



/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {

    // Wait an amount of seconds, while returning the actual wait time to the variable "m" 
    //When telling a computer to wait, it may have a load-amount that would result in it yielding earlier or later then the actual time, that's why we do this
    int m = wait(milisecondStepTime);



    /////////////////////////////////////////////////////////////////////////
    //Gets Values of Encoders
    /////////////////////////////////////////////////////////////////////////
    //This gets the position of the sensors (this is local position)
    int leftEnc = LeftEncoder.position(degrees);
    if(flipLeftEncoder) leftEnc *= -1; // If flipLeft, then multiply value by -1

    int rightEnc = RightEncoder.position(degrees);
    if(flipRightEncoder) rightEnc *= -1; // If flipRight, then multiply value by -1

    int horizEnc = HorizontalEncoder.position(degrees);
    if(flipHorizontalEncoder) horizEnc *= -1; // If flipHorizontal, then multiply value by -1

    // Get the rotation of the inertial sensor
    double RobotRotation = InternalSensor.rotation();
    if(flipRotations) RobotRotation *= -1;
    /////////////////////////////////////////////////////////////////////////



    /////////////////////////////////////////////////////////////////////////
    //Does quicc maths to find out an accurate encoder readings of local position change
    /////////////////////////////////////////////////////////////////////////
    //This is for the local position of the robot (THIS IS NOT WORLD POSITION)
    double VerticalPosLocal = (leftEnc + rightEnc) / 2.0;
    double HorizontalPosLocal = horizEnc + (RobotRotation * rotateNegateHorizontalRatio);
    /////////////////////////////////////////////////////////////////////////

    //Applies ratio
    HorizontalPosLocal *= verticalToHorizontalRatio;

    //By getting position - lastposition within 20 miliseconds, this will give us velocity
    //Similar to PID alrogithms, this converts position -> Velocity by gettings its derivative through the continuous loop
    double YV = VerticalPosLocal - lastVerticalPosLocal;
    double XV = HorizontalPosLocal - lastHorizontalPosLocal;

    //Do some relatively-complex maths that converts local velocity to world-space velocity. The (m/milisecondStepTime) at the end is a multiplier 
    //that would scale velocity more appropriately due to computer yields (Line 131 explains) If computer is told to wait 2 second and waits 0.75 instead,
    //the multiplier would then scale the velocity to be 0.375 its value (0.75/2 = 0.375)
    double XVelocityWorldSpace = (XV * std::cos(getRadians(-RobotRotation)) - YV * std::sin(getRadians(-RobotRotation))) * (m/milisecondStepTime);
	  double YVelocityWorldSpace = (XV * std::sin(getRadians(RobotRotation)) - YV * std::cos(getRadians(RobotRotation))) * (m/milisecondStepTime);

    //This would give us the X and Y position in world space
    XPos += XVelocityWorldSpace * posTranslationMultiplier;
    YPos += YVelocityWorldSpace * posTranslationMultiplier;

    //This is applied to be utilized 20 miliseconds after
    lastVerticalPosLocal = VerticalPosLocal;
    lastHorizontalPosLocal = HorizontalPosLocal;
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
