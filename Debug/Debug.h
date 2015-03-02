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
	void Debug(unsigned int val)
	{
		Serial.print(val);
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
		Serial.write(27);       // ESC command
        Serial.print("[2J");    // clear screen command
        Serial.write(27);
        Serial.print("[H");     // cursor to home command
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
		Debug("\r\nPressure Switch State: "); Debug(pressureSwitchState);
		//Serial.print("\r\nRobot has ball: "); Debug(hasBall);
		Debug("\r\nClaw Angle: "); Debug(clawAngle); Debug("\t\tPolar Arm Angle: "); Debug(verticalArmAngle); Debug("\t\tAzimuthal Arm Angle: "); Debug(horizontalArmAngle);
		Serial.print("\r\nOpto Sensor Readings:\tSensor A: "); Debug(optoSensorStateA); Debug("\r\n\t\t\tSensor B: "); Debug(optoSensorStateB); Debug("\r\n\t\t\tSensor C: "); Debug(optoSensorStateC); Debug("\r\n\t\t\tSensor D: "); Debug(optoSensorStateD);
		
		Serial.print("\r\n-----------------------------------------");
	}
	
	void DebugCalibrate()
	{
		Serial.print("-----------------------------------------\r\n");
		Debug("CALIBRATION VALUES");
		Debug("\r\nSensor A:\t\tMin:"); Debug(qtrrc.calibratedMinimumOn[0]); Debug("\t\tMax: "); Debug(qtrrc.calibratedMaximumOn[0]);
		Debug("\r\nSensor B:\t\tMin:"); Debug(qtrrc.calibratedMinimumOn[1]); Debug("\t\tMax: "); Debug(qtrrc.calibratedMaximumOn[1]);
		Debug("\r\nSensor C:\t\tMin:"); Debug(qtrrc.calibratedMinimumOn[2]); Debug("\t\tMax: "); Debug(qtrrc.calibratedMaximumOn[2]);
		Debug("\r\nSensor D:\t\tMin:"); Debug(qtrrc.calibratedMinimumOn[3]); Debug("\t\tMax: "); Debug(qtrrc.calibratedMaximumOn[3]);
	}

#endif