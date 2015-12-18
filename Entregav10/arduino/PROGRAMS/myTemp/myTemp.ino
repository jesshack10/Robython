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

temp = robot.GetTemp();
	 Serial.print("Temperatura: "); 
	Serial.println(temp);
	tone( 2 ,1046.5 , 1000);
	delay(1000); noTone(2);

}


