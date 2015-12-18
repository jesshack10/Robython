#include "Robotec.h"
#include <stdlib.h>
int xAng, yAng, zAng;
/* CONFIGURATION INPUTS/OUTPUTS ROBOTEC
A0: TEMPERATURE SENSOR
A1: MULTISWITCH EXTERN
A2: LIGHT SENSOR LEFT
A3: LIGHT SENSOR RIGHT
A4: Z AXIS ACCELEROMETER
A5: Y AXIS ACCELEROMETER
A6: X AXIS ACCELEROMETER
A7: IR SENSOR
D0: TX1
D1: RX1
D2: BUZZER
D3: ENABLE MOTOR LEFT
D4: RED LED
D5: ENABLE MOTOR RIGHT
D6: CONTROL LEFT MOTOR
D7: GREEN RGB
D8: BLUE RGB
D9: CONTROL RIGHT MOTOR
D10: YELLOW LED
D11: YELLOW LED
D12: RED RGB
D13: GREEN LED
*/
Robotec::Robotec()
{

}
//Esta Funci�n hace que el RobotX genere un sonido de una determinada frecuencia y de duraci�n que ingrese el usuario.
void Robotec::SoundOn(int Frequency, int Duration)
{
	digitalWrite(2, Frequency);
	delay(Duration);
	digitalWrite(2, 0);
}

//Apaga cualquier sonido que genere el robot.
void Robotec::SoundOff()
{
	digitalWrite(2, 0);
}

//Obtiene la temperatura ambiente en la que se encuentra el Robot.
double Robotec::GetTemp()
{
	double temp;
	temp=analogRead(0);
    temp=(temp)/(10.0);
	temp=temp+4.0;
	return temp;
}

//Devuelve la distancia entre un objeto y el robot  (10 a 80 cm).
int Robotec::GetDistance()
{
	int distancia;
	distancia=(6762/(analogRead(7)-9))-4;
	return distancia;
}

//Se asigna un valor de:
//1 al 255 para mover cada rueda del motor HACIA ADELANTE. 
//-1 al -255 para mover cada rueda del motor HACIA ATRAS. 
//0 DETIENE EL ROBOT.
void Robotec::MoveRobot(int SpeedWheelLeft, int SpeedWheelRight)
{
	if((SpeedWheelLeft>0)&&(SpeedWheelRight>0))
	{
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, LOW);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, LOW);
	}
	else if((SpeedWheelLeft<0)&&(SpeedWheelRight<0))
	{
    SpeedWheelLeft=abs(SpeedWheelLeft);
	SpeedWheelRight=abs(SpeedWheelRight);
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, HIGH);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, HIGH);
	}
	else if((SpeedWheelLeft>0)&&(SpeedWheelRight<0))
	{
	SpeedWheelRight=abs(SpeedWheelRight);
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, LOW);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, HIGH);
	}
	else if((SpeedWheelLeft<0)&&(SpeedWheelRight>0))
	{
	SpeedWheelLeft=abs(SpeedWheelLeft);
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, HIGH);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, LOW);
	}
	else
	{
    analogWrite(3, 0);
	digitalWrite(6, LOW);
	analogWrite(5, 0);
	digitalWrite(9, LOW);
	}
}


void Robotec::MoveRoboTime(int SpeedWheelLeft, int SpeedWheelRight, int time)
{
//Desplaza al robot por un determinado intervalo de tiempo.
      if((SpeedWheelLeft>0)&&(SpeedWheelRight>0))
	{
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, LOW);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, LOW);
        delay(time);
        MoveRobot(0,0);
	}
	else if((SpeedWheelLeft<0)&&(SpeedWheelRight<0))
	{
        SpeedWheelLeft=abs(SpeedWheelLeft);
	SpeedWheelRight=abs(SpeedWheelRight);
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, HIGH);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, HIGH);
        delay(time);
        MoveRobot(0,0);
	}
	else if((SpeedWheelLeft>0)&&(SpeedWheelRight<0))
	{
	SpeedWheelRight=abs(SpeedWheelRight);
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, LOW);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, HIGH);
         delay(time);
         MoveRobot(0,0);
	}
	else if((SpeedWheelLeft<0)&&(SpeedWheelRight>0))
	{
	SpeedWheelLeft=abs(SpeedWheelLeft);
	analogWrite(3, SpeedWheelLeft);
	digitalWrite(6, HIGH);
	analogWrite(5, SpeedWheelRight);
	digitalWrite(9, LOW);
         delay(time);
        MoveRobot(0,0);
	}
	else
	{
    analogWrite(3, 0);
	digitalWrite(6, LOW);
	analogWrite(5, 0);
	digitalWrite(9, LOW);
	}
}

//Regresa la posici�n del Robot en el eje X.
int Robotec::GetXPosition()
{
	int x;
	int xRead = analogRead(6);
	xAng = map(xRead, 265, 402, -90, 90);
	x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
	return x;
}

//Regresa la posici�n del Robot en el eje Y.  
int Robotec::GetYPosition()
{
	int y;
	int yRead = analogRead(5);
	yAng = map(yRead, 265, 402, -90, 90);  
	y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
	return y;
}

//Regresa la posici�n del Robot en el eje Z.
int Robotec::GetZPosition()
{	
	int z;
	int zRead = analogRead(4);
	zAng = map(zRead, 265, 402, -90, 90);
	z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
	return z;
}


//Devuelve un valor entero con la lectura de luz del sensor derecho.
int Robotec::LightSensorRight()
{
	int LightRight;
	LightRight=analogRead(3);
	return LightRight;
}

//Devuelve un valor entero con la lectura de luz del sensor izquierdo.
int Robotec::LightSensorLeft()
{
	int LightLeft;
	LightLeft=analogRead(2);
	return LightLeft;
}

//Cambia los colores del Led RGB dependiendo de los valores asignados al Rojo, Verde y Azul. 
void Robotec::ColorRGB (int Red, int Green, int Blue )
{
	digitalWrite(12, Red);
	digitalWrite(7, Green);
	digitalWrite(8, Blue);
}

//Enciende el número de LED indicado en el parámetro
void Robotec::TurnOnLED(int LED)
{
	switch(LED)
 {
   case 1:  digitalWrite(4, HIGH);
     break;
   case 2:  digitalWrite(10, HIGH);
     break;
   case 3:  digitalWrite(11, HIGH);
     break;
   case 4:  digitalWrite(13, HIGH);
     break;
   default:
     break;
 }
}

//Apaga el número de LED indicado en el parámetro
void Robotec::TurnOffLED(int LED)
{
  switch(LED)
 {
   case 1:  digitalWrite(4, LOW);
     break;
   case 2:  digitalWrite(10, LOW);
     break;
   case 3:  digitalWrite(11, LOW);
     break;
   case 4:  digitalWrite(13, LOW);
     break;
   default:
     break;
 }
}
