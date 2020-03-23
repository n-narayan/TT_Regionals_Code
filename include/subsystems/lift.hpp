#include "main.h"

enum height { low, mid };

//Methods
void setLiftDrive();
void liftAsync(int dir);
void deploy();
void tower(bool intakeFirst, height x);
