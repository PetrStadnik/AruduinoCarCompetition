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
	// nastav�me na 2 mikrosekundy v�stup na GND (pro jistotu)
	// pot� nastav�me na 5 mikrosekund v�stup rovn� nap�jen�
	// a pot� op�t na GND
	digitalWrite(pTrig, LOW);
	delayMicroseconds(2);
	digitalWrite(pTrig, HIGH);
	delayMicroseconds(5);
	digitalWrite(pTrig, LOW);
	// pomoc� funkce pulseIn z�sk�me n�sledn�
	// d�lku pulzu v mikrosekund�ch (us)
	odezva = pulseIn(pEcho, HIGH);
	// p�epo�et z�skan�ho �asu na vzd�lenost v cm
	vzdalenost = odezva / 58.31;
	Serial.print("Vzdalenost je ");
	Serial.print(vzdalenost);
	Serial.println(" cm.");
	// pauza 0.5 s pro p�ehledn� �ten�

	return vzdalenost;
}
