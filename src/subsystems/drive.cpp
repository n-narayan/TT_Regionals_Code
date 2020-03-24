#include "main.h"

//Some constants
#define WHEEL_DIAM 4.0
#define CIRCUMFERENCE (WHEEL_DIAM * M_PI)

//Initialize PID objects for later use
PID drivePID = initPID(0.195, 0.005, 0.205, 0.05);
PID turnPID = initPID(0.115, 0.004, 0.14, 0.1);
PID gyroPID = initPID(0.13, 0.004, 0.145, 0.1);

static float prevTarget = 0;  //straight line correction
float cubeAmount[] = {2.5, 2.6, 2.65, 2.8, 2.9, 3.1, 3.45, 3.5, 3.6, 4.1, 4.9};

//Helper methods----------------------------------------------------------------
/**
* tare every chassis encoder
*
* @param none
* @return none
*/
void tare(){
  frontLeft.tare_position();
  backLeft.tare_position();
  frontRight.tare_position();
  backRight.tare_position();
}

/**
* switch between brake modes
*
* @param int between 0-2 representing brake mode
* @return none
*/
void brakeMode(int mode){
  pros::motor_brake_mode_e_t brakeMode;
  switch(mode){
    case 0:
      brakeMode=pros::E_MOTOR_BRAKE_HOLD;
      break;
    case 1:
      brakeMode=pros::E_MOTOR_BRAKE_COAST;
      break;
    case 2:
      brakeMode=pros::E_MOTOR_BRAKE_BRAKE;
      break;
  }
  frontLeft.set_brake_mode(brakeMode);
  backLeft.set_brake_mode(brakeMode);
  frontRight.set_brake_mode(brakeMode);
  backRight.set_brake_mode(brakeMode);
}

/**
* return raw encoder values of left side, averaged
*
* @param none
* @return absolute value of average of left encoders
*/
float avgEncoderLeft(){
  return (fabs(frontLeft.get_position()) + fabs(backLeft.get_position())) / 2;
}

/**
* return raw encoder values of right side, averaged
*
* @param none
* @return absolute value of average of right encoders
*/
float avgEncoderRight(){
  return (fabs(frontRight.get_position()) + fabs(backLeft.get_position())) / 2;
}

/**
* return raw encoder values chassis, averaged
*
* @param none
* @return absolute value of average of 4 chassis motors, averaged
*/
float avgEncoderVal(){
  return (fabs(frontLeft.get_position()) + fabs(frontRight.get_position())
          + fabs(backLeft.get_position()) + fabs(backRight.get_position())) / 4;
}

/**
* convert from inches to raw encoder ticks
*
* @param target in inches
* @return value converted to ticks
*/
float toTicks(float target){
  return (target*900)/CIRCUMFERENCE;
}

/**
* convert from degrees to raw encoder ticks
*
* @param target in degrees
* @return value converted to ticks
*/
float toTicksTurn(float target){
  return target * 10;
}

/**
* set each side of the drive to move at given velocities
*
* @param left velocity
* @param right veliocity
* @return none
*/
void setDrive(float left, float right){
  frontLeft.move(left);
  backLeft.move(left);
  frontRight.move(right);
  backRight.move(right);
}

/**
* modify drive velocities to account for drift and slew rate
*
* @param desired left velocity
* @param desired right veliocity
* @return none
*/
void setDriveSlew(float left, float right){
  //just being done in individual functions now
}

//Driver control----------------------------------------------------------------

/**
* dual joystick arcade drive, tailored to Sharva
*
* @param none
* @return none
*/
void arcadeDrive(){

  int straight = TeflonMenace.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
  int turn = TeflonMenace.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

  int leftJoy = straight + turn;
  int rightJoy = straight - turn;

  //deadzone
  if(abs(leftJoy) < 10)
    leftJoy = 0;
  if(abs(rightJoy) < 10)
    rightJoy = 0;

  //send this to motors based on joystick inputs
  setDrive(leftJoy, rightJoy);
}

//Linear motion-----------------------------------------------------------------
/**
* move the chassis toward a set target
* uses IMEs but not PID
*
* @param direction (fwd or rev)
* @param target in inches
* @param desired voltage (positive number)
* @return none
*/
void simpleDrive(direction x, float target, float voltage){
  //step one: tare
  tare();
  //if going backwards, reverse the chassis
  int direction = 1;
  if(x==rev)
      direction = -1;

  //while counts < the target, keep going
  while(fabs(avgEncoderVal()) < fabs(toTicks(target))){
    setDrive(voltage*direction, voltage*direction);
    pros::delay(20);
  }
  //brief brake, then set drive to neutral
  setDrive(-10*direction, -10*direction); //account for possible drift
  pros::delay(50);
  setDrive(0,0);  //return to neutral
}

/**
* move the chassis toward a set target
* uses full PID loop
*
* @param direction (fwd or rev)
* @param target in inches
* @param desired voltage (positive number)
* @param should the robot autocorrect itself for straightness?
         yes for almost every situation, but not all (i.e intaking)
         defaults to true
* @return none
*/
void drive(direction x, float target, float maxVoltage, int cubeNum, bool driveStraight){
  pros::lcd::print(1, "Entering function");

  float lastGyro = gyro.get_rotation(); //use this later to determine if the robot is actually straight
  bool atPoint = false;
  float power = 0;
  float accel = 4, accelCap = 0;
  float sensorInput = 0;
  unsigned int initTime = (int)pros::millis();

  //Straight line correction?
  //float prevGyro = gyro.get_rotation()*-1;
  //prevTarget *= -1;

  //step one: tare
  tare();

  //convert inches to raw ticks
  float setPoint = fabs(toTicks(target));
  pros::lcd::print(1, "Target: %f", setPoint);

  //determine if we need to move forward or backwards
  int direction = 1;
  if(x == rev)
    direction = -1;

  //run PID to determine output --> will bring compiler to PID.hpp
  while(!atPoint){
    sensorInput = avgEncoderVal();
    power = calcOutput(drivePID, setPoint, sensorInput, maxVoltage, accelCap);
    power *= direction;

    //Acceleration control?
    accelCap += accel;
    if(accelCap > maxVoltage)
      accelCap = maxVoltage;

    if(fabs(power) > accelCap)
      power = accelCap * direction;

    /*Compensate for any possible drift
    if(driveStraight && lastGyro >=0)
      setDrive(power - (gyro.get_rotation()-lastGyro), power + (gyro.get_rotation()-lastGyro));
    else if(driveStraight && lastGyro <=0)
      setDrive(power + (gyro.get_rotation()+lastGyro), power - (gyro.get_rotation()-lastGyro));
    else  //If we do not want to auto correct the drive
      setDrive(power * direction, power * direction);*/

    /** Straight line correction
    *   Given the fact that the gyro overshoots nearly every turn,
    *   determine the difference (erro) between the actual gyro value vs desired
    *   and correct the chassis to that position as you drive forward
    */

    float gyroDifference = gyro.get_rotation() + prevTarget;

    if(driveStraight == true)
      //setDrive(voltage + (gyro.get_rotation() + prevGyro), voltage  - (gyro.get_rotation() + prevGyro));
      setDrive(power - gyroDifference * cubeAmount[cubeNum], power  + gyroDifference * cubeAmount[cubeNum]);
    /*else if(driveStraight == true && x==rev)
      setDrive(power - (gyro.get_rotation() + prevTarget)*2.5, power  + (gyro.get_rotation() + prevTarget)*2.5);*/
    else
      setDrive(power, power);

    //setDrive(power * direction, power * direction);

    //If you have reached your target, exit the loop and brake
    if(sensorInput > setPoint - 50 && sensorInput < setPoint + 50)
      atPoint = true;

    pros::delay(20);
  }

  float elapsedTime = ((float)pros::millis() - initTime)/1000;
  pros::lcd::print(7, "Elapsed Time: %f", elapsedTime);
  setDrive(0,0);  //afterwards, brake
}

//Turning stuff-----------------------------------------------------------------
/**
* turn the chassis a set number of degrees
* uses the gyro, but no PID
*
* @param direction (left or right)
* @param target in degrees
* @param desired voltage (positive number)
* @return none
*/
void gyroTest(direction x, int degrees, int voltage){
  //Since gyro does not reset after every turn,
  //Use the last value in order to manually reset later
  float lastGyro = fabs(gyro.get_rotation());
  float power = 0;

  int direction = 1;
  if(x==left)
    direction = -1;

  prevTarget = degrees * direction;

  while(fabs(gyro.get_rotation()) < degrees){
    setDrive(voltage * direction, -voltage * direction);
    pros::delay(20);
  }

  //Lose momentum
  pros::delay(100);
  setDrive(0,0);  //reset to neutral
}

/**
* turn the chassis a set number of degrees
* uses the gyro and PID
*
* @param direction (left or right)
* @param target in degrees
* @param desired voltage (positive number)
* @return none
*/
void gyroTurn(direction x, float degrees, float maxVoltage){


  bool atPoint = false;
  float lastGyro = fabs(gyro.get_rotation());
  float sensorInput = 0;
  float power = 0;
  unsigned int initTime = (int)pros::millis();

  int direction = 1;    //determine direction
  if(x == left)
    direction = -1;

  float setPoint = degrees;
  prevTarget = setPoint * direction;

  while(!atPoint){
    sensorInput = fabs(gyro.get_rotation());// - lastGyro;
    power = calcOutput(gyroPID, setPoint, sensorInput, maxVoltage, 50.0);

    setDrive(power*direction, -power*direction);

    if(sensorInput > setPoint-2 || sensorInput == setPoint)
      atPoint = true;

    pros::delay(20);
    }

    float elapsedTime = ((float)pros::millis() - initTime)/1000;
    pros::lcd::print(7, "Elapsed Time: %f", elapsedTime);
    setDrive(0,0);  //afterwards, brake

    //after PID loop, if robot heading off by half degree, correct
  /*
  if(fabs(gyro.get_rotation()) >  fabs(target)+.5){
    error = target - gyro.get_rotation();
    while(fabs(error) > fabs(target)){
      setDrive(50 * direction , -50 * direction);
      pros::delay(10);
    }
    setDrive(0,0);
  }
  */

  prevTarget *= -1;

}

/**
* turn the chassis a set number of degrees
* uses IME input only
*
* @param direction (left or right)
* @param target in degrees
* @param desired voltage (positive number)
* @return none
*/
void simpleTurn(direction x, float degrees, int voltage){
  tare();

  //Convert from degrees to raw ticks
  double degreestoTurn = fabs(toTicksTurn(degrees));

  int direction = 1;    //determine direction
  if(x == left)
    direction = -1;

    //Turn until degrees is reached (leave margin for error)
    while(fabs(avgEncoderLeft()) < degreestoTurn-200){
      setDrive(voltage * direction, -voltage * direction);
      pros::delay(20);
    }

    //Lose momentum
    pros::delay(100);
    setDrive(0,0);  //reset to neutral
}

/**
* turn the chassis a set number of degrees
* uses IME input and PID
*
* @param direction (left or right)
* @param target in degrees
* @param desired voltage (positive number)
* @return none
*/
void turn(direction x, float degrees, float maxVoltage){
  tare();

  //Convert from degrees to ticks
  bool atPoint = false;
  float sensorInput = 0;
  float power = 0;
  float accel = 4, accelCap = 0;
  unsigned int initTime = (int)pros::millis();
  float degreesToTurn = fabs(toTicksTurn(degrees));
  pros::lcd::print(1, "Target: %f", degreesToTurn);

  int direction = 1;
  if(x == left)
    direction = -1;

  prevTarget = degrees * direction;

  while(!atPoint){
    sensorInput = fabs(avgEncoderLeft());
    power = calcOutput(turnPID, degreesToTurn, sensorInput, maxVoltage, 50.0);

    //Acceleration control?
    accelCap += accel;
    if(accelCap > maxVoltage)
      accelCap = maxVoltage;

    if(fabs(power) > accelCap)
      power = accelCap;

    setDrive(power*direction, -power*direction);

    if(sensorInput == degreesToTurn || sensorInput > degreesToTurn)
      atPoint = true;

    pros::delay(30);
  }

  float elapsedTime = ((float)pros::millis() - initTime)/1000;
  pros::lcd::print(7, "Elapsed Time: %f", elapsedTime);
  setDrive(0,0);  //afterwards, brake
}
