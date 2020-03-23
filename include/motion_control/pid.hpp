#include "main.h"

typedef struct{
  float kP; //the PID constants
  float kI;
  float kD;

  float totalError;
  float lastTime;
  float prevTarget;
  float prevError;
  float errorMargin;

} PID;

PID initPID(float p_const, float i_const, float d_const, float margin);
float calcOutput(PID subsystem, float target, float sensorVal, float maxVoltage, float minVoltage);
