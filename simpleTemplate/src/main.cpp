/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\theon                                            */
/*    Created:      Tue Oct 11 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// frontLeft            motor         1               
// frontRight           motor         2               
// backLeft             motor         3               
// backRight            motor         4               
// Motor5               motor         5               
// inertialSens         inertial      9               
// rotationS            rotation      10              
// rotationP            rotation      11              
// Motor6               motor         6               
// pneumatic            digital_out   A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

void pre_auton(void) {
  vexcodeInit();

}

//Simple function, "void" means doesn't return a value
void macro1() {
  Motor5.spin(forward, 8, voltageUnits::volt);
  task::sleep(500);
  Motor5.stop();
}


void autonomous(void) {
  //Autonomous code goes here

}

void usercontrol(void) {
  //User control code goes here

  frontLeft.setStopping(coast);
  frontRight.setStopping(coast);
  backLeft.setStopping(coast);
  backRight.setStopping(coast);

  //Float (16bit) variables for drive control
  float axis2;
  float axis4;

  //Booleans (true/false) for buttin toggle
  bool motor6Toggle;      //Should the motor spin?
  bool motor6Spinning;    //Is the motor spinning?
  bool motor6Direction;   //Which direction should it be spinning?

  //Pneumatic piston toggle boolean
  bool pistonToggle = false;

  while (1) {
    
    /////////////////////////////////////////////////////////////////////////
    /* Drive */

    axis2 = Controller1.Axis2.position();
    axis4 = Controller1.Axis4.position();

    frontLeft.spin(forward, ((axis2 + axis4) * 0.12), voltageUnits::volt);
    frontRight.spin(forward, ((axis2 - axis4) * 0.12), voltageUnits::volt);
    backLeft.spin(forward, ((axis2 + axis4) * 0.12), voltageUnits::volt);
    backRight.spin(forward, ((axis2 - axis4) * 0.12), voltageUnits::volt);
    
    /////////////////////////////////////////////////////////////////////////
    /* Button motor control */

    if (Controller1.ButtonR1.pressing()) {
      Motor5.spin(forward, 12, voltageUnits::volt);
    } else if (Controller1.ButtonR2.pressing()) {
      Motor5.spin(reverse, 12, voltageUnits::volt);
    } else {
      Motor5.stop();
    }

    /////////////////////////////////////////////////////////////////////////
    /* Button on/off toggle */

    bool L1 = Controller1.ButtonL1.pressing();
    bool L2 = Controller1.ButtonL2.pressing();

    if (L1 && (motor6Spinning == false)) {
      motor6Spinning = true;
      motor6Toggle = !motor6Toggle;
      motor6Direction = false;
    } else if (L2 && (motor6Spinning == false)) {
      motor6Spinning = true;
      motor6Toggle = !motor6Toggle;
      motor6Direction = true;
    } else if (!L1 && !L2) {
      motor6Spinning = false;
    }

    if (motor6Toggle && (motor6Direction == false)) {
      Motor6.spin(forward, 12, voltageUnits::volt);
    } else if (motor6Toggle && (motor6Direction == true)) {
      Motor6.spin(reverse, 12, voltageUnits::volt);
    } else {
      Motor6.stop();
    }

    /////////////////////////////////////////////////////////////////////////
    /* Button macro */

    Controller1.ButtonA.pressed(macro1);

    /////////////////////////////////////////////////////////////////////////
    /* Pneumatic piston control */

    if (Controller1.ButtonB.pressing()) pistonToggle = !pistonToggle;

    if (pistonToggle) {
      pneumatic.set(true);
    } else {
      pneumatic.set(false);
    }

    /////////////////////////////////////////////////////////////////////////

    task::sleep(20);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    //Prevents the loop from exiting
    task::sleep(100);
  }
  
}
