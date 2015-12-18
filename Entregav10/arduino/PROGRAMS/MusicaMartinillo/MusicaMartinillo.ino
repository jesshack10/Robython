#include "Robotec.h"

Robotec robot;
float distance,temp, lightR, lightL;
void setup()
{
	Serial.begin(9600);
	pinMode(2, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	robot.MoveRoboTime(0,0,0);
}

void loop()
{
	tone( 2 ,523.25 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,587.32 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,659.25 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,523.25 , 500);
	delay(500); noTone(2);
	tone( 2 ,523.25 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,587.32 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,659.25 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,523.25 , 500);
	delay(500); noTone(2);
	tone( 2 ,659.25 , 500);
	delay(500); noTone(2);
	tone( 2 ,698.45 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,783.99 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,659.25 , 500);
	delay(500); noTone(2);
	tone( 2 ,698.45 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,783.99 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,783.99 , 500);
	delay(500); noTone(2);
	tone( 2 ,880 , 500);
	delay(500); noTone(2);
	tone( 2 ,783.99 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,698.45 , 500);
	delay(500); noTone(2);
	tone( 2 ,659.25 , 500);
	delay(500); noTone(2);
	tone( 2 ,523.25 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,783.99 , 500);
	delay(500); noTone(2);
	tone( 2 ,880 , 500);
	delay(500); noTone(2);
	tone( 2 ,783.99 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,698.45 , 500);
	delay(500); noTone(2);
	tone( 2 ,659.25 , 500);
	delay(500); noTone(2);
	tone( 2 ,523.25 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,587.32 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,493.88 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,1046.5 , 1000);
	delay(1000); noTone(2);
	tone( 2 ,1046.5 , 1000);
	delay(1000); noTone(2);

}


