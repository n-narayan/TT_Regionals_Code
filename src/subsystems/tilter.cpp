#include "main.h"

//Constants, enums, sensory, etc
#define INTAKING 0
#define DEPOSIT 2950
PID tilterPID = initPID(0.165, 0.1, 0.14, 0.1);

/**
* tares the motor, that's it
*
* @param none
* @return none
*/
void tareTilter(){
  tilterMotor.tare_position();
}

/**
* tilts the tray to the appropriate position using a p_loop
*
* @param desired position
* @return none
*/
//Allow the tilter to move automatically
void tilt(tilterPos x){
  tareTilter();
  bool atPoint = false;
  float power = 0;
  float sensorInput = 0;
  float initTime = (int)pros::millis();
  float setPoint;

  if(x==deposit){
    setPoint = DEPOSIT;
    while(!atPoint){
      sensorInput = tilterMotor.get_position();
      power = calcOutput(tilterPID, setPoint, sensorInput, 80, 30);

      tilterMotor.move(power);

      if(sensorInput == setPoint || sensorInput > setPoint)
        atPoint = true;

      pros::delay(30);
    }
    tilterMotor.move(0);  //afterwards, brake
  }

  else{  //we're going back to the original position
    setPoint = INTAKING;
    while(tilterMotor.get_position() > INTAKING || limit.get_value() != 1){
      tilterMotor.move(-90);
      pros::delay(20);
    }
    tilterMotor.move(0);  //afterwards, brake
  }

  float elapsedTime = ((int)pros::millis() - initTime)/1000;
  pros::lcd::print(7, "Elapsed Time: %f", elapsedTime);
  //tilterMotor.move(0);  //afterwards, brake
}

/**
* should anything go wrong with the main macro, this is our failsafe
* tilts the tray to the appropriate position
*
* @param desired position
* @return none
*/
void tilterTemp(tilterPos x){
  if(x==deposit) { //tilt to outtake cubes
    tareTilter();
    while(tilterMotor.get_position() < DEPOSIT){
      tilterMotor.move(80);

      pros::delay(20);
    }
    tilterMotor.move(0);
  }
  if(x==intake) { //tilt intake again
    while(tilterMotor.get_position() > INTAKING || limit.get_value() != 1){
      tilterMotor.move(-90);
      pros::delay(20);
    }
  }
}

//For driver control------------------------------------------------------------
/**
* driver control using the macro
*
* @param none
* @return none
*/
void tilterDrive(){

  pros::lcd::print(1, "Tilter encoder val: %f", tilterMotor.get_position());

  if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_UP)==1)
    tilt(deposit);
  else if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)==1)
    tilt(intake);
  tilterMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  pros::delay(20);
}

/**
* driver control NOT using the macro
*
* @param none
* @return none
*/
void tilterDriveManual(){

  pros::lcd::print(1, "Tilter encoder val: %f", tilterMotor.get_position());
  pros::lcd::print(2, "Tilter potentiometer: %f", tilterPot);

  if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_L1)==1){
    //while(tilterMotor.get_position() < 2000)
      tilterMotor.move(75);
    //tilterMotor.move(65);
  }
  else if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_L2)==1){
    tilterMotor.move(-85);
  }
  else
    tilterMotor.move(0);
}
