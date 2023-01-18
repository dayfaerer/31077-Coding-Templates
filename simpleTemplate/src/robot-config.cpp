#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor frontLeft = motor(PORT1, ratio18_1, false);
motor frontRight = motor(PORT2, ratio18_1, false);
motor backLeft = motor(PORT3, ratio18_1, false);
motor backRight = motor(PORT4, ratio18_1, false);
motor Motor5 = motor(PORT5, ratio18_1, false);
inertial inertialSens = inertial(PORT9);
rotation rotationS = rotation(PORT10, false);
rotation rotationP = rotation(PORT11, false);
motor Motor6 = motor(PORT6, ratio18_1, false);
digital_out pneumatic = digital_out(Brain.ThreeWirePort.A);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}