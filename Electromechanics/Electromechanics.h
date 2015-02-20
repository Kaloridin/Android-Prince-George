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
	//analogWrite(enablePinA, speed);
	//analogWrite(enablePinB, speed);
	digitalWrite(enablePin, HIGH);
}
/* A, C return values from [0,1023] ; B, D return values from [0,255] */
void PhotoResistorReading(const int photoSensorPin, int &photoSensorState)
{
	photoSensorState = analogRead(photoSensorPin);
	delay(5);
}
#endif