/*
	Debug.h - Library for debugging robot.
	Created by Kaden Kujanpaa, January 16, 2015.
*/
#ifndef Debug_h
#define Debug_h

#include "Arduino.h"


	void Debug(String msg)
	{
		Serial.print(msg);
	}
	void Debug(int val)
	{
		Serial.print(val);
	}
	void Debug(float val)
	{
		Serial.print(val);
	}
	void Debug(char letter)
	{
		Serial.print(letter);
	}
	void Debug(int val, int sensorMin, int sensorMax, int outputMin, int outputMax)
	{
		Serial.print(map(val, sensorMin, sensorMax, outputMin, outputMax));
	}
	
	void Status(String currentState, movement movementType, int x_coord, int y_coord, bool hasBall)
	{	
		Serial.print("-----------------------------------------\r\n");
		Serial.print("Current State:\t"); Serial.print(currentState);
		Serial.print("\r\nMovement Type:\t");
		if (movementType == lateral) 
		{
			Serial.print("Lateral");
		} 
		else if (movementType == longitudinal)
		{
			Serial.print("Longitudinal");
		}
		else if (movementType == none)
		{
			Serial.print("None");
		}
		Serial.print("\r\nCurrent Coordinates:\tX = "); Serial.print(x_coord);
		Serial.print("\r\n\t\t\tY = "); Serial.print(y_coord);
		Serial.print("\r\nTarget Coordinates:\tX = "); Serial.print(x_target);
		Serial.print("\r\n\t\t\tY = "); Serial.print(y_target);
		//Serial.print("\r\nH-Bridge Enables:\tH-Bridge 1: A: "); 
		Serial.print("\r\nRobot has ball: "); if (hasBall == true) {Serial.print("True");} else {Serial.print("False");}
		Serial.print("\r\nPhotoresistor Readings:\tSensor A: "); Debug(photoSensorStateA); Debug("\r\n\t\t\tSensor B: "); Debug(photoSensorStateB); Debug("\r\n\t\t\tSensor C: "); Debug(photoSensorStateC); Debug("\r\n\t\t\tSensor D: "); Debug(photoSensorStateD);
		
		Serial.print("\r\n-----------------------------------------");
	}

#endif