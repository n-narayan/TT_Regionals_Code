#include "main.h"

/**
* initialize a PID object (i.e the chassis) with all necessary constants
*
* @param kP constant for proportional term
* @param kI constant for integral term
* @param kD constant for derivative term
* @param margin of error for this subsystem
*
* @return PID structure
*/
PID initPID(float p_const, float i_const, float d_const, float margin){
  PID pid;
  pid.kP = p_const;
  pid.kI = i_const;
  pid.kD = d_const;
  pid.totalError = 0;
  pid.lastTime = (float)pros::millis();
  pid.prevTarget = 0;
  pid.prevError = 0;
  pid.errorMargin = margin;

  return pid;
}

/**
* given a certain subsystem, generate appropriate output based on
* distance from the desired target
*
* @param subsystem of the robot in question (i.e chassis, tilter)
* @param desired target (in raw units for encoder/sensor in question)
* @param source of feedback/sensor values
* @param maximum output, should not be exceeded
* @param minimum output, should the calculated output be too slow
*
* @return an output between -127 and 127, or between min and max voltage
*/
float calcOutput(PID subsystem, float target, float sensorVal, float maxVoltage, float minVoltage){

  float error = target;
  float pError = 0;
  float deltaTime = 0;

  float pTerm = 0, dTerm = 0, iTerm = 0;
  float power = 0;

  //if the function is being called for a new movement, reset previous values
  if(target != subsystem.prevTarget){
    subsystem.totalError = 0;
    subsystem.prevTarget = target;
  }

  deltaTime = (float)(pros::millis() - subsystem.lastTime) / 1000;  //determine how many seconds have passed
  subsystem.lastTime = pros::millis();

  error = target - sensorVal;

  if(fabs(error) < subsystem.errorMargin)   //compensation for outside forces
    subsystem.totalError += error;
  else
    subsystem.totalError = 0;

  if(deltaTime > 0) //limit the rate of change in voltage
    dTerm = (sensorVal - subsystem.prevError) / deltaTime;
  subsystem.prevError = sensorVal;

  pTerm = error * subsystem.kP;
  iTerm = subsystem.totalError * subsystem.kI;
  dTerm *= subsystem.kD;

  pros::lcd::print(2, "Error: %f", error);  //print for testing purposes
  pros::lcd::print(3, "Proportional: %f", pTerm);
  pros::lcd::print(4, "Integral: %f", iTerm);
  pros::lcd::print(5, "Derivative: %f", dTerm);

  power = pTerm + iTerm + dTerm;  //determine, and then modify voltage as necessary

  if(power > maxVoltage)
    power = maxVoltage;
  if(power < minVoltage)
    power = minVoltage;

  pros::lcd::print(6, "Output: %f", power);

  return power;
}
