#include "InfraLed.h"



InfraLed::InfraLed(int pin1, int pin2, int pin3, int pin4)
{
	pinMode(pin1, INPUT);
	pinMode(pin2, INPUT);
	pinMode(pin3, INPUT);
	pinMode(pin4, INPUT);
}



int InfraLed::GetInfraLed()
{
	
	int infraleds[4] = { !(digitalRead(pin1)), !(digitalRead(pin2)), !(digitalRead(pin3)), !(digitalRead(pin4)) };
	
}
