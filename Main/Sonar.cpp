#include "Sonar.h"



Sonar::Sonar(int pinTrig, int pinEcho)
{
	pTrig = pinTrig;
	pEcho = pinEcho;
	pinMode(pTrig, OUTPUT);
	pinMode(pEcho, INPUT);
}

int Sonar::Distance()
{
	long odezva, vzdalenost;
	// nastavíme na 2 mikrosekundy výstup na GND (pro jistotu)
	// poté nastavíme na 5 mikrosekund výstup rovný napájení
	// a poté opìt na GND
	digitalWrite(pTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(pTrig, HIGH);
	delayMicroseconds(5);
	digitalWrite(pTrig, LOW);
	// pomocí funkce pulseIn získáme následnì
	// délku pulzu v mikrosekundách (us)
	odezva = pulseIn(pEcho, HIGH);
	// pøepoèet získaného èasu na vzdálenost v cm
	vzdalenost = odezva / 58.31;
	Serial.print("Vzdalenost je ");
	Serial.print(vzdalenost);
	Serial.println(" cm.");
	// pauza 0.5 s pro pøehledné ètení

	return vzdalenost;
}
