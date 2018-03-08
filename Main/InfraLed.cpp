#include "InfraLed.h"
#include <Arduino.h>



InfraLed::InfraLed(int pin1, int pin2, int pin3, int pin4)
{
	pinMode(pin1, INPUT);
	pinMode(pin2, INPUT);
	pinMode(pin3, INPUT);
	pinMode(pin4, INPUT);
}






