#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {

	pros::lcd::initialize();

	//Sensor init period
	gyro.reset();
	pros::delay(5000);

	//selectAuton();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {

//Sensor init period
	gyro.reset();
	pros::delay(5000);

 //selectAuton();

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

const bool nilaOp = false;

void autonomous() {
	switch(auton){
		case 1:	//red protected
			protectedThree(red);
			break;
		case 2:	//blue proteced
			protectedThree(blue);
			break;
		case 3:	//red unprotected
			eightPoint(red);
			break;
		case 4:	//blue unprotected
			eightPoint(blue);
			break;
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	while (true) {

		pros::lcd::print(6, "rot: %f", gyro.get_rotation());

		arcadeDrive();

		tilterDrive();

		tilterDriveManual();

		runIntakeDrive();

		setLiftDrive();

		if(TeflonMenace.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)==1){
			//fivePoint(blue);
			eightPoint(blue);
			//skills();
			//gyroTurn(left, 90, 80);
			//drive(fwd, 30, 100, 0);
		}
			//skills();//gyroTurn(left, 90, 100);//turn(left, 90, 100);//drive(fwd, 10, 100);

		pros::delay(20);
	}
}
