//#pragma once
#ifndef __SONAR_H__
#define __SONAR_H__
#include <Arduino.h>
class Sonar
{
public:
	Sonar(int pinTrig, int pinEcho);
	int Distance();
	int pTrig;
	int pEcho;
};
#endif
