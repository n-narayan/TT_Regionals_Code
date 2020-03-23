#include "main.h"

enum direction {fwd, rev, left, right};

//Helper methods
void tare();
void brakeMode(int mode);
float avgEncoderLeft();
float avgEncoderRight();
float avgEncoderVal();
float toTicks(float target);
float toTicksTurn(float target);
void setDrive(float left, float right);
void setDriveSlew(float left, float right);

//Driver control
void arcadeDrive();

//Linear motion
void simpleDrive(direction x, float target, float voltage);
void drive(direction x, float target, float maxVoltage, int cubeNum, bool driveStraight = true);

//Turning
void gyroTest(direction x, int degrees, int voltage);
void gyroTurn(direction x, float degrees, float maxVoltage);
void simpleTurn(direction x, float degrees, int voltage);
void turn(direction x, float degrees, float maxVoltage);
