// Robotec.h
#ifndef _ROBOTEC_h
#define _ROBOTEC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


class Robotec
{
private:
	int ROBOTEC;
public:
	Robotec();

    void SoundOn(int Frequency, int Duration);
	void SoundOff();
	double GetTemp();
	int GetDistance();

	void MoveRobot(int SpeedWheelLeft, int SpeedWheelRight);
	void MoveRoboTime(int SpeedWheelLeft, int SpeedWheelRight, int time);
	
	int GetXPosition();
	int GetYPosition();	
	int GetZPosition();
	
	int LightSensorRight();
	int LightSensorLeft();
	
	void ColorRGB (int Red, int Green, int Blue );
	
	void TurnOffLED(int LED);
  void TurnOnLED(int LED);
};
extern Robotec ROBOTEC;

#endif

