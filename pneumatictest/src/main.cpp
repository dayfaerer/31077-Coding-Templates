/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\theon                                            */
/*    Created:      Mon Oct 10 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// pneumatic            digital_out   A               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

competition Competition;

void pneumaticactivate() {
  pneumatic.set(true);
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(green);
  Brain.Screen.drawRectangle(0,0,500,500);
}

void pneumaticreset() {
  pneumatic.set(false);
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(0,0,500,500);
}

void usercontrol(void) {

  while (1) {
    Controller1.ButtonA.pressed(pneumaticactivate);
    Controller1.ButtonB.pressed(pneumaticreset);
    task::sleep(20);
  }
}

int main() {
  Competition.drivercontrol(usercontrol);
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
}
