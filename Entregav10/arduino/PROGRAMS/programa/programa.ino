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

void robython(){
float i ; for(i=1;i<400+1; i=i+1){
	if(i==100)
{
	robot.TurnOnLED(1);
	robot.TurnOffLED(2);
	robot.TurnOffLED(3);
	robot.TurnOffLED(4);
	tone( 2 ,440 , 100);
	delay(100); noTone(2);
	Serial.println(i ) ;
	if(i==200)
{
	robot.TurnOnLED(2);
	robot.TurnOffLED(3);
	robot.TurnOffLED(1);
	robot.TurnOffLED(4);
	tone( 2 ,440 , 100);
	delay(100); noTone(2);
	Serial.println(i ) ;

}
	if(i==300)
{
	robot.TurnOnLED(3);
	robot.TurnOffLED(2);
	robot.TurnOffLED(1);
	robot.TurnOffLED(4);
	tone( 2 ,440 , 100);
	delay(100); noTone(2);
	Serial.println(i ) ;

}
	if(i==400)
{
	robot.TurnOnLED(4);
	robot.TurnOffLED(1);
	robot.TurnOffLED(2);
	robot.TurnOnLED(3);
	tone( 2 ,440 , 100);
	delay(100); noTone(2);
	Serial.println(i ) ;

}

}

}

}

void loop()
{
robython ( ) ; 
}


