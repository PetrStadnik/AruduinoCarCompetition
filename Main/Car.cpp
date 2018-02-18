#include "Car.h"



Car::Car(int mot1, int mot2, int mot3, int mot4) : m1(mot1), m2(mot2), m3(mot3), m4(mot4)
{
	m1.setSpeed(200);
	m2.setSpeed(200);
	m3.setSpeed(200);
	m4.setSpeed(200);
	m1.run(RELEASE);
	m2.run(RELEASE);
	m3.run(RELEASE);
	m4.run(RELEASE);
}


void Car::TurnLeft(int t) {
	m4.run(FORWARD);
	m3.run(BACKWARD);
	m2.run(BACKWARD);
	m1.run(FORWARD);

	SetAllSpeed(200);
	delay(t);
	SetAllSpeed(0);
}

void Car::TurnRight(int t) {
	m4.run(BACKWARD);
	m3.run(FORWARD);
	m2.run(FORWARD);
	m1.run(BACKWARD);

	SetAllSpeed(200);
	delay(t);
	SetAllSpeed(0);
}

void Car::SetAllSpeed(int v) {
	m4.setSpeed(v);
	m3.setSpeed(v);
	m2.setSpeed(v);
	m1.setSpeed(v);
}
void Car::GoForward(int v)
{
	m1.run(BACKWARD);
	m2.run(BACKWARD);
	m3.run(BACKWARD);
	m4.run(BACKWARD);

	SetAllSpeed(v);
}