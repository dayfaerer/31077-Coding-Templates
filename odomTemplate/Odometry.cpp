#include <cmath>
#include <vex.h>

class Odometry {

  public:
    //X and Y positions
    float posX;
    float posY;

    //How many milliseconds every cycle
    const int msStepTime = 20;

    //Wait function that returns time actually waited
    //Credit: James Pearman on VEX Forum https://www.vexforum.com/t/vexcode-sleep-help/82706/2?u=connor
    uint32_t wait( uint32_t time_mS ) {
      uint32_t start = Brain.Timer.system();
      this_thread::sleep_for(time_mS);
      return (uint32_t)(Brain.Timer.system() - start);
    }
  ;

  private:
    //Distances from tracking wheels to robot's center of rotation
    const float sR = 0.5;
    const float sS = 1.0;

    //Current sensor readings, changes every loop
    float currentVertical;
    float currentHorizontal;
    float currentTheta;

    //Change (delta) of the positions using current and last readings
    float deltaVertical;
    float deltaHorizontal;
    float deltaTheta;

    //Previous sensor readings
    float prevVertical;
    float prevHorizontal;
    float prevTheta;

    //Local delta values: "error" calculated depending if the robot has turned or not
    float localDeltaX;
    float localDeltaY;

    //Average theta: used to calculate global delta values
    float averageTheta;

    //Global change (delta) of positions
    float globalDeltaX;
    float globalDeltaY;
  ;

  public: 
    Odometry() {
      while (true) {

        //Accounts for error in wait times
        int w = wait(msStepTime);
        
        //Gets current readings and converts to 
        currentVertical = rotationP.position(deg) * (M_PI * 2.75) / 360;
        // currentHorizontal = rotationSensS.position(deg) * (M_PI * 2.75) / 360;
        currentTheta = inertialSens.heading() * M_PI/180;

        //Calculates error of position from current - last position
        deltaVertical = currentVertical - prevVertical;
        deltaHorizontal = currentHorizontal - prevHorizontal;
        deltaTheta = currentTheta - prevTheta;
        
        /* Changes previous values to current values */
        prevVertical = currentVertical;
        prevHorizontal = currentHorizontal;
        prevTheta = currentTheta;

        if (fabs(deltaTheta) < 0.1) {
          /* If there is a negligible amount of rotational movement, local positional changes are set equal to
          the processed change in positions */
          localDeltaX = deltaVertical;
          localDeltaY = deltaHorizontal;
        } else {
          /* If there IS a change in rotational movement, calculates actual positional changes to account for it */
          localDeltaX = 2 * std::sin(deltaTheta / 2) * ((deltaHorizontal / deltaTheta) + sR);
          localDeltaY = 2 * std::sin(deltaTheta / 2) * ((deltaVertical / deltaTheta) + sS);
        }

        averageTheta = currentTheta - (deltaTheta / 2.0);

        globalDeltaX = (localDeltaY * std::cos(averageTheta)) - (localDeltaX * std::sin(averageTheta)) * (w / msStepTime);
        globalDeltaY = (localDeltaY * std::sin(averageTheta)) + (localDeltaX * std::cos(averageTheta)) * (w / msStepTime);

        /* Changes global positions by global position changes */
        posX += globalDeltaX;
        posY -= globalDeltaY;

        Brain.Screen.printAt(10,20, "deltaX: %.2f ", posX);
        Brain.Screen.printAt(10,40, "deltaY: %.2f ", posY);
        Brain.Screen.printAt(10,60, "Heading: %.2f ", inertialSens.rotation(degrees));


        task::sleep(20);
      }
    }

    float getX() {
      return(posX);
    }

    float getY() {
      return(posY);
    }

  ;

};