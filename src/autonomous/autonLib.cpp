#include "main.h"

/* 5 POINT AUTON
*  1. Drive forward, deploying tray as you intake preload + 4 cubes ahead
*  2. Deposit in UNPROTECTED
*/
void fivePoint(color x){
  //Slurp those cubes
  runIntake(1);
  drive(fwd, 44, 80, 0, false);
  pros::delay(1300);
  stopIntake();

  setDrive(0,0);

  pros::delay(100);

  //Reverse, turn towards UNPROTECTED
  simpleDrive(rev, 22, 80);

  if(x==red)
    gyroTurn(right, 120, 90);
  else
    turn(left, 100, 90);

  setDrive(0,0);

  //Drive to unprotected, deposit stack
  simpleDrive(fwd, 10, 75);
  //drive(fwd, 8, 75, 5, false);
  //slowIntake(-1);
  //pros::delay(300);
  //stopIntake();
  tilt(deposit);

  //Release, drive back
  pros::delay(500);
  drive(rev, 10, 80, 0, false);
  setDrive(0,0);
}

/* 6 POINT AUTON
*  1. Drive forward, deploying tray as you intake preload + 4 cubes ahead
*  2. Get that one cube in front of the tower, ya feel me?
*  3. Deposit in UNPROTECTED
*/
void sixPoint(color x){
  //Slurp those cubes
  runIntake(1);
  drive(fwd, 42, 90, 0, false);
  pros::delay(1300);

  //Turn slightly for 6th cube, drive forward
  if(x==red)
    gyroTurn(left, 5, 40);
  else
    gyroTurn(left, 5, 40);

  drive(fwd, 5, 30, 5, false);
  pros::delay(500);
  stopIntake();

  //Turn towards unprotected zone
  if(x==red)
    gyroTurn(right, 130, 90);
  else
    gyroTurn(left, 120, 90);

  //Drive to unprotected, deposit stack
  drive(fwd, 40, 75, 6, true);
  //slowIntake(-1);
  //pros::delay(300);
  //stopIntake();
  tilt(deposit);

  //Release, drive back
  pros::delay(500);
  simpleDrive(rev, 10, 80);
  setDrive(0,0);
}

/* 8 POINT AUTON
*  1. Intake preload + 3 of the 4 cubes ahead
*  2. Reverse, turn and intake the line of four cubes
*  3. Deposit in the unprotected zone
*/

void eightPoint(color x){

  //gyroTurn(right,0,0);  //set prevTarget

  //Slurp those cubes
  runIntake(1);
  drive(fwd, 27, 85, 0, false);
  pros::delay(1000);
  stopIntake();

  pros::delay(300);

  //Reverse, turn towards next stack
  drive(rev, 8, 80, 3);
  setDrive(0,0);
  if(x==red)
    gyroTurn(left, 50, 90);
  else
    gyroTurn(right, 50, 90);

  drive(rev, 20, 80, 3);
  pros::delay(300);
  if(x==red)
    gyroTurn(right, 0, 90);
  else
    gyroTurn(left, 0, 90);

  //Slurp those cubes
  runIntake(1);
  drive(fwd, 32, 85, 3, true);
  pros::delay(1300);
  stopIntake();

  drive(rev, 20, 90, 7);
  setDrive(0,0);

  pros::delay(100);

  if(x==red)
    gyroTurn(right, 105, 85);
  else
    gyroTurn(left, 105, 85);

  setDrive(0,0);
  pros::delay(200);

  //Drive to unprotected, deposit stack
  //simpleDrive(fwd, 12, 75);
  drive(fwd, 12, 75, 7, true);
  //slowIntake(-1);
  //pros::delay(300);
  //stopIntake();
  tilterTemp(deposit);

  //Release, drive back
  pros::delay(500);
  drive(rev, 10, 60, 0, false);
  setDrive(0,0);
}

/* 3 POINT AUTON
*  1. Intake preload + 2 cubes near protected zone
*  2. Deposit in protected
*/

void protectedThree(color x){
  //Preload + cube directly ahead
  runIntake(1);
  drive(fwd, 20, 90, true);
  stopIntake();

  //Turn, get third cube
  if(x==red)
    gyroTurn(left, 90, 90);
  else
    gyroTurn(right, 90, 90);

  runIntake(1);
  drive(fwd, 10, 90, true);
  pros::delay(500);
  stopIntake();

  //Deposit in protected
  if(x==red)
    gyroTurn(left, 30, 50);
  else
    gyroTurn(right, 30, 50);

  //Drive to unprotected, deposit stack
  drive(fwd, 40, 75, true);
  //slowIntake(-1);
  //pros::delay(300);
  //stopIntake();
  tilt(deposit);

  //Release, drive back
  pros::delay(500);
  drive(rev, 10, 80, 0, false);
  setDrive(0,0);
}

/* SKILLLLLLZZZZZZZZZZZZZZZZZZZZZ
*  1. Drive straight, intaking 8-9 cubes
*  2. Deposit in unprotected
*  3. Take one cube from the line of 4, put it in alliance tower
*  4. Continue stacking, put in red side unprotected
*/
void skills(){
  //Preload + 6-8 of the cubes
  runIntake(1);
  drive(fwd, 120, 80, 0);
  //pros::delay(3000);
  stopIntake();

  pros::delay(100);

  //Turn and deposit in blue unprotected
  turn(right, 90, 50);
  setDrive(0,0);
  pros::delay(300);
  drive(fwd, 30, 80, 8);

  pros::delay(500);
  //slowIntake(-1);
  //pros::delay(300);
  //stopIntake();
  tilterTemp(deposit);
  tilterMotor.move(0);

  //Reverse, get one cube from the 4 stack and tower it
  drive(rev, 15, 80, 0);
  setDrive(0,0);
  gyroTurn(right, 30, 50);
  pros::delay(100);

  runIntake(1);
  drive(fwd, 20, 80, 0);
  pros::delay(600);
  stopIntake();

  tower(false, low);
  pros::delay(500);
}
