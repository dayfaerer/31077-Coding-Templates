/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\theon                                            */
/*    Created:      Tue Apr 12 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftMotor            motor         1               
// rightMotor           motor         2               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

////////////////////////////////////////////////////////////////////////////////

void pre_auton(void) {
  vexcodeInit();

  leftMotor.resetPosition();
  rightMotor.resetPosition();
}

////////////////////////////////////////////////////////////////////////////////
//////////                    Autonomous Functions                    //////////
////////////////////////////////////////////////////////////////////////////////

double vCap;      //Voltage cap
double targetL;   //Target value for left
double targetR;   //Target value for right

int drivePID() {
  double errorL;    //Left side error variable initiation
  double errorR;    //Right side error variable initiation

  double kp = 0.0;        //Proportional terms produce a value proportionate to the size of the error value.
  double kpTurn = 0.0;    //Too high, robot will oscillate at target. Too low, robot will stop moving before reaching target.

  double acc = 0.5;   //Acceleration: how fast robot speeds up when not at target location


  double voltL = 0;   //Left side actual motor voltage
  double voltR = 0;   //Right side actual motor voltage

  double signL;   //Variable to determine forward/reverse direction of left side
  double signR;   //Variable to determine forward/reverse direction of right side

  while (Competition.isAutonomous()) {
    errorL = targetL - leftMotor.position(degrees);     //Error of left side: target left position - actual left position
    errorR = targetR - rightMotor.position(degrees);    //Error of right side: target right position - actual right position

    signL = errorL / fabs(errorL);    //Calculation for determining the sign of the left error (-1, 0, or 1)
    signR = errorR / fabs(errorR);    //Calculation for determining the sign of the right error (-1, 0, or 1)

    if (signL == signR) {
      voltL = (errorL * kp);    //If the signs of both error values are equal,
      voltR = (errorR * kp);    //multiply errors by proportional term
    } else {
      voltL = (errorL * kpTurn);    //If the signs of both error values are unequal,
      voltR = (errorR * kpTurn);    //multiply errors by the proportional turn term
    }

    vCap += acc;    //Increase the volt cap by the acceleration value every tick

    if (vCap > 12) {
      vCap = 12;    //Prevents the volt cap from going over 12, which is a vex motors max voltage
    }

    if (fabs(voltL) > vCap) {   // 
      voltL = (vCap * signL);   //
    }                           //Uses the voltage cap and the sign of the error to prevent the actual
    if (fabs(voltR) > vCap) {   //voltage from increasing past 12, and keeps the voltages sign correct
      voltR = (vCap * signR);   //
    }                           //

    if ((voltL < .5) && (voltL > -.5)) {    //Provides a range of error in the voltage, forcing 
      voltL = 0;                            //any value between -0.5 and 0.5 to be 0
    }

    if ((voltR < .5) && (voltR -.5)) {     //Provides a range of error in the voltage, forcing
      voltR = 0;                           //any value between -0.5 and 0.5 to be 0
    }

    leftMotor.spin(forward, voltL, voltageUnits::volt);     //Actually spins the motors using each sides voltage. 
    rightMotor.spin(forward, voltR, voltageUnits::volt);    //

    task::sleep(20);    //Tick speed; the rate at which this function runs. Can affects acceleration and accuracy
  }
  return (1);
}

void PID(int left, int right) {   //Main drive function.
  targetL += left;                //Used to tell the PID loop what the target values should be.
  targetR += right;               //Because this does not reset the target values, it can be used multiple times in a row.
  vCap = 0;                       //Also resets the volt cap to renew acceleration.
}

void driveReset() {             //
  targetL = 0;                  //
  targetR = 0;                  //Function used to reset both the motor encoders and the target values 
  leftMotor.resetPosition();    //
  rightMotor.resetPosition();   //
}

////////////////////////////////////////////////////////////////////////////////

void autonomous(void) {
  leftMotor.resetPosition();    //Makes sure both motors read 0 degrees at
  rightMotor.resetPosition();   //the start of the autonomous period
  ////////////////////////////////

  
}

////////////////////////////////////////////////////////////////////////////////
//////////                      Drive Functions                       //////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

void usercontrol(void) {
  
  while(1) {

    task::sleep(20);
  }
}

////////////////////////////////////////////////////////////////////////////////

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    task::sleep(100);
  }
}
