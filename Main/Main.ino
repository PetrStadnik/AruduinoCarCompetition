


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
// èíslo pinu s LED diodou pro notifikaci
#define LED_PIN 13 

// inicializace promìnných, do kterých se uloží data
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

// inicializace promìnných pro výpoèet
Quaternion q;           // [w, x, y, z] kvaternion
VectorFloat gravity;    // [x, y, z] vektor setrvaènosti
float rotace[3];        // rotace kolem os x,y,z

						// Rutina pøerušení
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
	// nastavení LED jako výstupní
	pinMode(LED_PIN, OUTPUT);
	// nastavení I2C sbìrnice
	Wire.begin();
	// komunikace pøes sériovou linku rychlostí 115200 baud
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
	// kontrola funkènosti DMP
	if (devStatus == 0) {
		// zapnutí DMP
		Serial.println(F("Povoleni DMP..."));
		mpu.setDMPEnabled(true);
		// nastavení pinu INT jako pøerušovacího, interrupt 0 odpovídá pinu 2
		attachInterrupt(0, dmpINT, RISING);
		mpuIntStatus = mpu.getIntStatus();
		Serial.println(F("DMP pripraveno, cekam na prvni preruseni.."));
		dmpReady = true;
		// naètení velikosti zpráv, které bude DMP posílat
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else {
		// V pøípadì chyby:
		// 1 : selhání pøipojení k DMP
		// 2 : selhání pøi nastavení DMP
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
	
	// komunikace pøes sériovou linku rychlostí 9600 baud
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

	// jednobitová promìnná pro uložení stavu 
	// naètení stavu aktuálního pinu, nutná
	// negace, protože modul vrací 0 pøi detekci
	//levý boèní
	Serial.print(!(digitalRead(44)));
	//levý èárový
	Serial.print(!(digitalRead(46)));
	//pravý èárový
	Serial.print(!(digitalRead(48)));
	//pravý boèní
	Serial.print(!(digitalRead(50)));
	// ukonèení øádku na sériové lince
	Serial.println();
	// pauza pro pøehlednìjší výpis
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
	// dokud nepošle DMP pøerušení, mùžeme provádìt ostatní pøíkazy
	// ve smyèce while níže
	if (!dmpReady) return;
	// tato podmínka èeká na pøíjem pøerušení a mùžeme v ní provádìt
	// ostatní operace
	while (!mpuInterrupt && fifoCount < packetSize) {
		// místo pro ostatní operace
		// ..
	}

	// získání informace o statusu DSP
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();
	// získání velikosti zásobníku dat
	fifoCount = mpu.getFIFOCount();
	// kontrola pøeteèení zásobníku dat
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		mpu.resetFIFO();
		Serial.println(F("Preteceni zasobniku dat!"));
		// v pøípadì pøeteèení zásobníku je nutné
		// èastìji vyèítat data
	}
	else if (mpuIntStatus & 0x02) {
		// kontrola délky dat
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		// ètení paketu ze zásobníku
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		// pøi vìtším množství paketù snížíme poèítadlo
		fifoCount -= packetSize;
		// pøeètení dat z DSP a uložení do promìnných
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(rotace, &q, &gravity);

		// výpis informací o rotacích kolem jednotlivých os
		Serial.print("Rotace \t X ");
		Serial.print(rotace[2] * 180 / M_PI);
		Serial.print("st \t Y ");
		Serial.print(rotace[1] * 180 / M_PI);
		Serial.print("st \t Z ");
		Serial.print(rotace[0] * 180 / M_PI);
		Serial.println("st");
	}
}

