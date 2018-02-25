#pragma once
class InfraLed
{
public:
	InfraLed(int pin1, int pin2, int pin3, int pin4);
	int GetInfraLed();
	int pin[4];
	bool leftSide;
	bool rightSide;

	bool leftLine;
	bool lightLine;
};

