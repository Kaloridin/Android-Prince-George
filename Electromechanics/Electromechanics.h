/*
	Electromechanics.h - Library for implementing actuator and sensor control with robot.
	Created by Kaden Kujanpaa, February 6, 2015.
*/
#ifndef Electromechanics_h
#define Electromechanics_h

#include <Arduino.h>

void MotorControl(const int controlPinA1,const int controlPinA2,const int controlPinB1,const int controlPinB2,const int enablePinA,const int enablePinB, int speed, int direction)
{	
	if (speed == 0) // Turn off motors
	{
		digitalWrite(controlPinA1, LOW);
		digitalWrite(controlPinB1, LOW);
	}
	else if (direction == 0) // Direction reverse/left
	{
		digitalWrite(controlPinA1, LOW);
		digitalWrite(controlPinA2, HIGH);
		digitalWrite(controlPinB1, LOW);
		digitalWrite(controlPinB2, HIGH);
	}
	else // Direction forward/right
	{
		digitalWrite(controlPinA1, HIGH);
		digitalWrite(controlPinA2, LOW);
		digitalWrite(controlPinB1, HIGH);
		digitalWrite(controlPinB2, LOW);
	}
	//analogWrite(enablePinA, speed);
	//analogWrite(enablePinB, speed);
	digitalWrite(enablePinA, HIGH);
	digitalWrite(enablePinB, HIGH);
}

void PhotoResistorReadings(const int photoSensorPinA, const int photoSensorPinB, const int photoSensorPinC, const int photoSensorPinD, int &photoSensorStateA, int &photoSensorStateB, int &photoSensorStateC, int &photoSensorStateD)
{
	photoSensorStateA = analogRead(photoSensorPinA);
	//photoSensorStateA = map(photoSensorStateA, 0, 1023, 1, 10);
	delay(5);
	photoSensorStateB = analogRead(photoSensorPinB);
	//photoSensorStateB = map(photoSensorStateB, 0, 255, 1, 10);
	delay(5);
	photoSensorStateC = analogRead(photoSensorPinC);
	//photoSensorStateC = map(photoSensorStateC, 0, 1023, 1, 10);
	delay(5);
	photoSensorStateD = analogRead(photoSensorPinD);
	//photoSensorStateD = map(photoSensorStateD, 0 , 255, 1, 10);
	delay(5);
}
#endif