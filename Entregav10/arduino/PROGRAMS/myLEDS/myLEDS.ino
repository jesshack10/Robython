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
	robot.TurnOnLED(1);
	tone( 2 ,1046.5 , 1000);
	delay(1000); noTone(2);
	robot.TurnOnLED(2);
	tone( 2 ,1174.65 , 1000);
	delay(1000); noTone(2);
	robot.TurnOnLED(3);
	tone( 2 ,1318.51 , 1000);
	delay(1000); noTone(2);
	robot.TurnOnLED(4);
	tone( 2 ,1396.91 , 1000);
	delay(1000); noTone(2);
	robot.TurnOffLED(1);
	tone( 2 ,523.25 , 1000);
	delay(1000); noTone(2);
	robot.TurnOffLED(2);
	tone( 2 ,587.32 , 1000);
	delay(1000); noTone(2);
	robot.TurnOffLED(3);
	tone( 2 ,659.25 , 1000);
	delay(1000); noTone(2);
	robot.TurnOffLED(4);
	tone( 2 ,698.45 , 1000);
	delay(1000); noTone(2);

}


