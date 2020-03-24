#include "main.h"

/**
 * Run the intake to be intaking cubes
 * 
 * @param direction (intake or outtake)
 * 
 * @return none
 * 
 * */

void runIntake(int direction){
  intakeLeft.move(127 * direction);
  intakeRight.move(127 * direction);

  if(direction == -1){
    intakeLeft.move(127 * direction);
    intakeRight.move(127 * direction);
  }
}

/**
 * Run the intake to be intaking cubes at a slower pace for precision
 * 
 * @param direction (intake or outtake)
 * 
 * @return none
 * 
 * */
void slowIntake(int direction){
  intakeLeft.move(80 * direction);
  intakeRight.move(80 * direction);
}

/**
 * Cut power to the intakes
 * 
 * @param none
 * 
 * @return none
 * 
 * */
void stopIntake(){
  intakeLeft.move(0);
  intakeRight.move(0);
}

/**
 * Operate in opcontrol()
 * 
 * @param none
 * 
 * @return none
 * 
 * */
void runIntakeDrive(){
  if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_R1)==1)
    runIntake(1);
  else if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_R2)==1)
    slowIntake(-1);
  else
    stopIntake();
}
