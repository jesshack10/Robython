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

float i,index0,index1,n,result;

void mia(){
 for(i=0;i<2; i=i+1){
	Serial.println("HOLA" ) ;

}

}

void loop()
{
n=20;
index0=1;
index1=1;
for(i=2;i<n; i=i+1){
result=index0+index1;
index1=index0;
index0=result;

}
	Serial.println(result ) ;

}


