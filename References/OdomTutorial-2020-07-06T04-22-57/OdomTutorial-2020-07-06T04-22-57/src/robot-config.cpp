#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
encoder LeftEncoder = encoder(Brain.ThreeWirePort.A);
encoder RightEncoder = encoder(Brain.ThreeWirePort.C);
encoder HorizontalEncoder = encoder(Brain.ThreeWirePort.E);
inertial InternalSensor = inertial(PORT1);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}