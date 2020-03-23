#include "main.h"

/**
* asynchronous movement of lift and tray for driver control
*
* @param none
* @return none
*/
void setLiftDrive(){

  pros::lcd::print(3, "Lift encoders: %f", fabs(liftMotor.get_position()));

  if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_X)==1){
    liftAsync(1);
  }
  else if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_B)==1){
    liftAsync(-1);
  }
  else
    liftMotor.move(20);
}

/**
* asynchronous movement of lift and tray
*
* @param direction, up or down
* @return none
*/
void liftAsync(int dir){
  if(dir == 1){
    tilterMotor.move(62);
    liftMotor.move(127);
  }
  else{
    tilterMotor.move(-100);
    liftMotor.move(-100);
  }
}

/**
* backup deploy method using the arms rather than just intakes
* less reliable, but we keep it in case
*
* @param none
* @return none
*/
void deploy(){
  liftMotor.tare_position();

  while(fabs(liftMotor.get_position())<2000)
    liftAsync(1);
  pros::delay(500);

  while(fabs(liftMotor.get_position())>50)
    liftAsync(-1);

  pros::delay(100);
  liftMotor.move(0);
  tilterMotor.move(0);

  liftMotor.tare_position();
}

/**
* intake a cube for depositing in a tower autonomously
*
* @param boolean to determine if intaking was already done or still has to be
* @return none
*/
void tower(bool intakeFirst, height x){

  if(intakeFirst){
  //intake the cube
  runIntake(1);
  pros::delay(400);
  stopIntake();
}

  liftMotor.tare_position();
  //Height differs based on low or middle tower
  if(x==low){
    while(fabs(liftMotor.get_position())<1800)
      liftAsync(1);
    pros::delay(100);
  }
  else{
    while(fabs(liftMotor.get_position())<2000)
      liftAsync(1);
    pros::delay(100);
  }

  pros::delay(500);
  slowIntake(-1);
  pros::delay(500);
  stopIntake();

  while(fabs(liftMotor.get_position())>50)
    liftAsync(-1);
  pros::delay(100);
  liftMotor.move(0);
  tilterMotor.move(0);
}
