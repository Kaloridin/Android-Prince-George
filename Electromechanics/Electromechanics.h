/*
	Electromechanics.h - Library for implementing actuator and sensor control with robot.
	Created by Kaden Kujanpaa, February 6, 2015.
*/
#ifndef Electromechanics_h
#define Electromechanics_h

#include <Arduino.h>

void MotorControl(const int controlPin1, const int controlPin2, const int enablePin, int speed, int direction)
{	
	if (speed == 0) // Turn off motors
	{
		digitalWrite(controlPin1, LOW);
		digitalWrite(controlPin2, LOW);
	}
	else if (direction == 0) // Direction reverse/left
	{
		digitalWrite(controlPin1, LOW);
		digitalWrite(controlPin2, HIGH);
	}
	else // Direction forward/right
	{
		digitalWrite(controlPin1, HIGH);
		digitalWrite(controlPin2, LOW);
	}
	analogWrite(enablePin, speed);
	//digitalWrite(enablePin, HIGH);
}

void ChangeServoAngle(Servo myServo, int angle)
{
	myServo.write(angle);
	delay(15);
}

void OptoSensorReading()
{
	unsigned int sensors[4];
	//qtrrc.readCalibrated(sensors);
	qtrrc.read(sensors);
	optoSensorStateA = sensors[0];
	optoSensorStateB = sensors[1];
	optoSensorStateC = sensors[2];
	optoSensorStateD = sensors[3];
}
#endif