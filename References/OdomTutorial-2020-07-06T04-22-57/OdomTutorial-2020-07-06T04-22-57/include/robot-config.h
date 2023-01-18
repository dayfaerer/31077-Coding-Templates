using namespace vex;

extern brain Brain;

// VEXcode devices
extern encoder LeftEncoder;
extern encoder RightEncoder;
extern encoder HorizontalEncoder;
extern inertial InternalSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );