#ifndef _KINEMATICS_H
#define _KINEMATICS_H

#include "stm32f10x.h"
#include "pid.h"
#include "pwm_in.h"

#define LengthA 0.31
#define LengthB 0.21
#define LengthR 0.05
extern float velocity[3];

void MatrixInit();
void GetVelocity();
void GetDisire();

#endif
