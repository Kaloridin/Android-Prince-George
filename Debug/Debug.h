/*
	Debug.h - Library for debugging robot.
	Created by Kaden Kujanpaa, January 16, 2015.
*/
#ifndef Debug_h
#define Debug_h

#include "Arduino.h"


	void Debug(String msg)
	{
		Serial.println(msg);
	}
	void Debug(int val)
	{
		Serial.println(val);
	}
	void Debug(float val)
	{
		Serial.println(val);
	}
	void Debug(int val, int sensorMin, int sensorMax, int outputMin, int outputMax)
	{
		Serial.println(map(val, sensorMin, sensorMax, outputMin, outputMax));
	}
	
	void Status(String currentState, movement movementType, int x_coord, int y_coord, bool hasBall )
	{	Serial.print("-----------------------------------------\n");
		Serial.print("Current State:\t"); Serial.print(currentState);
		Serial.print("\nMovement Type:\t"); if (movementType == lateral) {Serial.print("Lateral");} else {Serial.print("Longitudinal");}
		Serial.print("\nCurrent Coordinates:\tX = "); Serial.print(x_coord);
		Serial.print("\n\t\tY = "); Serial.print(y_coord);
		Serial.print("Robot has ball: "); if (hasBall == true) {Serial.print("True");} else {Serial.print("False");}
	
		
		
		Serial.print("\n-----------------------------------------");
	}

#endif