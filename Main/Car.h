//#pragma once
#ifndef __CAR_H__
#define __CAR_H__
#include <AFMotor.h>
#include <Arduino.h>
class Car
{
public:
	AF_DCMotor m4;
	AF_DCMotor m3;
	AF_DCMotor m2;
	AF_DCMotor m1;
	Car(int mot1, int mot2, int mot3, int mot4);
	void TurnLeft(int t);
	void TurnRight(int t);
	void SetAllSpeed(int v);
	void GoForward(int v);
};

#endif