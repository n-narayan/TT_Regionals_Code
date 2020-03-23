#include "main.h"

//Run the intake to be intaking cubes
void runIntake(int direction){
  intakeLeft.move(127 * direction);
  intakeRight.move(127 * direction);

  if(direction == -1){
    intakeLeft.move(127 * direction);
    intakeRight.move(127 * direction);
  }
}

void slowIntake(int direction){
  intakeLeft.move(80 * direction);
  intakeRight.move(80 * direction);
}

//Stop the intake
void stopIntake(){
  intakeLeft.move(0);
  intakeRight.move(0);
}

//Driver Control
void runIntakeDrive(){
  if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_R1)==1)
    runIntake(1);
  else if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_R2)==1)
    slowIntake(-1);
  else
    stopIntake();
}
