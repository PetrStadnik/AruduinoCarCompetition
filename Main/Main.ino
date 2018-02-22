


#include <AFMotor.h>
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Car.h"
Car car(1, 2, 3, 4);
#include "Sonar.h"
Sonar sonar(24, 22);
//#include "Gyroscope.h"
long stopDist = 20;
long savedStopDist = stopDist;
//-------gyroskop
MPU6050 mpu;
// ��slo pinu s LED diodou pro notifikaci
#define LED_PIN 13 

// inicializace prom�nn�ch, do kter�ch se ulo�� data
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

// inicializace prom�nn�ch pro v�po�et
Quaternion q;           // [w, x, y, z] kvaternion
VectorFloat gravity;    // [x, y, z] vektor setrva�nosti
float rotace[3];        // rotace kolem os x,y,z

						// Rutina p�eru�en�
volatile bool mpuInterrupt = false;
void dmpINT() {
	mpuInterrupt = true;
}

//------------------------------------------------------------






//int minMod = 20;
//int minSpeed = 0;
int x = 0;


void setup()
{
	
	//-----gyroskop
	// nastaven� LED jako v�stupn�
	pinMode(LED_PIN, OUTPUT);
	// nastaven� I2C sb�rnice
	Wire.begin();
	// komunikace p�es s�riovou linku rychlost� 115200 baud
	Serial.begin(115200);
	while (!Serial);
	// inicializace akcelerometru a gyroskopu
	Serial.println(F("Inicializace I2C zarizeni.."));
	mpu.initialize();
	Serial.println(F("Test pripojenych zarizeni.."));
	Serial.println(mpu.testConnection() ? F("Modul pripojeni") : F("Pripojeni modulu selhalo"));
	// incializace DMP
	Serial.println(F("Inicializace DMP..."));
	devStatus = mpu.dmpInitialize();
	// kontrola funk�nosti DMP
	if (devStatus == 0) {
		// zapnut� DMP
		Serial.println(F("Povoleni DMP..."));
		mpu.setDMPEnabled(true);
		// nastaven� pinu INT jako p�eru�ovac�ho, interrupt 0 odpov�d� pinu 2
		attachInterrupt(0, dmpINT, RISING);
		mpuIntStatus = mpu.getIntStatus();
		Serial.println(F("DMP pripraveno, cekam na prvni preruseni.."));
		dmpReady = true;
		// na�ten� velikosti zpr�v, kter� bude DMP pos�lat
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else {
		// V p��pad� chyby:
		// 1 : selh�n� p�ipojen� k DMP
		// 2 : selh�n� p�i nastaven� DMP
		Serial.print(F("DMP inicializace selhala (kod "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
	digitalWrite(LED_PIN, LOW);
	//-------------------
	//----------infra led
	pinMode(44, INPUT);
	pinMode(46, INPUT);
	pinMode(48, INPUT);
	pinMode(50, INPUT);
	//---------------
	
	// komunikace p�es s�riovou linku rychlost� 9600 baud
	//Serial.begin(9600);
}

void loop() {
	
	
	if (sonar.Distance()<stopDist+20)
	{

		if (sonar.Distance()<stopDist)
		{
			car.SetAllSpeed(0);
			delay(200);
			Calibration(sonar.Distance());
			MazeTurn();
		}
		else
		{
			car.GoForward(150);
		}
	}
	else
	{
		car.GoForward(200);
	}
}

void Calibration(long realDist)
{
	stopDist = savedStopDist;
	if (realDist > stopDist)
	{
		stopDist = stopDist - (realDist - stopDist);
	}
	else if (realDist < stopDist)
	{
		stopDist = stopDist + (stopDist - realDist);
	}
}

int InfraSensors()
{
	Serial.print("Stav vstupnich pinu: ");

	// jednobitov� prom�nn� pro ulo�en� stavu 
	// na�ten� stavu aktu�ln�ho pinu, nutn�
	// negace, proto�e modul vrac� 0 p�i detekci
	//lev� bo�n�
	Serial.print(!(digitalRead(44)));
	//lev� ��rov�
	Serial.print(!(digitalRead(46)));
	//prav� ��rov�
	Serial.print(!(digitalRead(48)));
	//prav� bo�n�
	Serial.print(!(digitalRead(50)));
	// ukon�en� ��dku na s�riov� lince
	Serial.println();
	// pauza pro p�ehledn�j�� v�pis
	delay(500);
	int infraleds[4] = { !(digitalRead(44)), !(digitalRead(46)), !(digitalRead(48)), !(digitalRead(50)) };
	return infraleds[4];
}

void MazeTurn()
{

	// Serial.print(InfraSensors[3]);
	if ((digitalRead(44) == 1 && digitalRead(50) == 1) || (digitalRead(44) == 0 && digitalRead(50) == 0))
	{
		car.TurnLeft(1500);
		int lDist = sonar.Distance();
		car.TurnRight(3000);
		int rDist = sonar.Distance();
		if (lDist > rDist)
		{
			car.TurnLeft(3000);
		}
		else{}

	}
	else if (!(digitalRead(44)) == 0)
	{
		car.TurnLeft(1500);
	}
	else
	{
		car.TurnRight(1500);
	}
}



void Rotation()
{
	// dokud nepo�le DMP p�eru�en�, m��eme prov�d�t ostatn� p��kazy
	// ve smy�ce while n�e
	if (!dmpReady) return;
	// tato podm�nka �ek� na p��jem p�eru�en� a m��eme v n� prov�d�t
	// ostatn� operace
	while (!mpuInterrupt && fifoCount < packetSize) {
		// m�sto pro ostatn� operace
		// ..
	}

	// z�sk�n� informace o statusu DSP
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();
	// z�sk�n� velikosti z�sobn�ku dat
	fifoCount = mpu.getFIFOCount();
	// kontrola p�ete�en� z�sobn�ku dat
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		mpu.resetFIFO();
		Serial.println(F("Preteceni zasobniku dat!"));
		// v p��pad� p�ete�en� z�sobn�ku je nutn�
		// �ast�ji vy��tat data
	}
	else if (mpuIntStatus & 0x02) {
		// kontrola d�lky dat
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		// �ten� paketu ze z�sobn�ku
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		// p�i v�t��m mno�stv� paket� sn��me po��tadlo
		fifoCount -= packetSize;
		// p�e�ten� dat z DSP a ulo�en� do prom�nn�ch
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(rotace, &q, &gravity);

		// v�pis informac� o rotac�ch kolem jednotliv�ch os
		Serial.print("Rotace \t X ");
		Serial.print(rotace[2] * 180 / M_PI);
		Serial.print("st \t Y ");
		Serial.print(rotace[1] * 180 / M_PI);
		Serial.print("st \t Z ");
		Serial.print(rotace[0] * 180 / M_PI);
		Serial.println("st");
	}
}

