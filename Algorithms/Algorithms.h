/*
	Algorithms.h - Library for algorithms used by robot states
	Created by Kaden Kujanpaa, February 5, 2015.
*/
#ifndef Algorithms_h
#define Algorithms_h

#include <Arduino.h>

// Updates the current coordinates based on grid detection readings from optosensors on base
void CoordinateAlgorithm(int &x_i, int &y_i, int x_f, int y_f, movement movementType, bool &newIntersection, int currentDirection) // i = current position, f = final (target) position
{	
	// Update sensor readings
	PhotoResistorReading(photoSensorPinA, photoSensorStateA);
	PhotoResistorReading(photoSensorPinB, photoSensorStateB);	
	PhotoResistorReading(photoSensorPinC, photoSensorStateC);	
	PhotoResistorReading(photoSensorPinD, photoSensorStateD);	
	
	switch(movementType) // Movement type is either lateral or longitudinal
	{
		case longitudinal:	
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{ 	// (A:870, B:200) -> Working values in lab
								//if (870 > photoSensorStateA || 200 > photoSensorStateD) // A: 750, D:130 for our prototype
								if (!CheckIfBlack('A', blackValue, whiteValue) || !CheckIfBlack('D', blackValue, whiteValue))
								{	// Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0,0);
									MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0,0);
									if (y_f < y_i) // Final coordinate is south of current position
										y_i -= 1;
									else // Final coordinate is north of current position
										y_i += 1;
								}
							}
							else if (870 < photoSensorStateA && 200 < photoSensorStateD)
							{
								newIntersection = false;
							}
							break;
		}
		case lateral:		
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{	// (A:220, B:850) -> Working values in lab
								//if (220 > photoSensorStateB ||  850 > photoSensorStateC)
								if (!CheckIfBlack('B', blackValue, whiteValue) || !CheckIfBlack('C', blackValue, whiteValue))
								{ // Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
									MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
									if (x_f < x_i) // Final coordinate is west of current position
										x_i -= 1;
									else // Final coordinate is east of current position
										x_i += 1;
								}
							}
							else if (220 < photoSensorStateB && 850 < photoSensorStateC)
							{
								newIntersection = false;
							}
							break;
		}
		// The case of i == f will never occur. When (x_f, y_f) is reached new coordinates
		// will be assigned based on the next state.
	}
}

bool CheckIfBlack(char sensor, int blackValue, int whiteValue)
{	
	int currentValue; // Current sensor reading corresponding to 'sensor'
	switch(sensor) // Update sensor reading
	{
		case 'A':	PhotoResistorReading(photoSensorPinA, currentValue); break;
		case 'B':	PhotoResistorReading(photoSensorPinB, currentValue); break;
		case 'C':	PhotoResistorReading(photoSensorPinC, currentValue); break;
		case 'D':	PhotoResistorReading(photoSensorPinD, currentValue); break;
	}
	if (abs(currentValue - blackValue) < abs(currentValue - whiteValue)) // Check metrics
		return true;
	else
		return false;
}

void Rotate(char sensor, rotation rotationType)
{
	switch(sensor)
	{
		case 'A':	while (!CheckIfBlack('D', blackValue, whiteValue))
					{
						if (rotationType == clockwise)
						{
							motorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0);
							motorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 1);
							motorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable1D, 0);
						}
						else if (rotationType == anticlockwise)
						{
							motorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 1);
							motorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0);
							motorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable1D, 1);
						}
					}
					break;
		case 'B':	while (!CheckIfBlack('C', blackValue, whiteValue))
					{
						if (rotationType == clockwise)
						{
							motorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 1);
							motorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 1);
							motorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable1D, 0);
						}
						else if (rotationType == anticlockwise)
						{
							motorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0);
							motorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0);
							motorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable1D, 1);
						}
					}
					break;
		case 'C':	while (!CheckIfBlack('B', blackValue, whiteValue))
					{
						if (rotationType == clockwise)
						{
							motorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 1);
							motorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0);
							motorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable1D, 0);
						}
						else if (rotationType == anticlockwise)
						{
							motorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0);
							motorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 1);
							motorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable1D, 1);
						}
					}
					break;
		case 'D':	while (!CheckIfBlack('A', blackValue, whiteValue))
					{
						if (rotationType == clockwise)
						{
							motorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 1);
							motorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0);
							motorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 1);
						}
						else if (rotationType == anticlockwise)
						{
							motorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0);
							motorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 1);
							motorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0);
						}
					}
					break;
	}
	// Power down all motors
	MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
	MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
	MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
	MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
}

void OrientationCorrection(char sensor, movement movementType, int currentDirection)
{
	switch (sensor)
	{
		case 'A':	if (currentDirection == 0)
						Rotate('A', clockwise);
					else
						Rotate('A', anticlockwise);
					// Move left until sensors B,C trip
					while (!CheckIfBlack('B', blackValue, whiteValue) && !CheckIfBlack('C', blackValue, whiteValue))
					{
						MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 0);
						MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 0);
					}
					break;
		case 'B':	if (currentDirection == 0)
						Rotate('B', anticlockwise);
					else
						Rotate('B', clockwise);
					// Move down until sensor A,D trip
					while (!CheckIfBlack('A', blackValue, whiteValue) && !CheckIfBlack('D', blackValue, whiteValue))
					{
						MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 0);
						MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 0);
					}
					break;
		case 'C':	if (currentDirection == 0)
						Rotate('C', clockwise);
					else
						Rotate('C', anticlockwise);
					// Move up until sensor A,D trip
					while (!CheckIfBlack('A', blackValue, whiteValue) && !CheckIfBlack('D', blackValue, whiteValue))
					{
						MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 1);
						MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 1);
					}
					break;
		case 'D':	if (currentDirection == 0)
						Rotate('D', anticlockwise);
					else
						Rotate('D', clockwise);
					// Move right until sensor B,C trip
					while (!CheckIfBlack('B', blackValue, whiteValue) && !CheckIfBlack('C', blackValue, whiteValue))
					{
						MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 1);
						MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 1);
					}
					break;
	}
	// Power down all motors
	MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
	MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
	MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
	MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
}

#endif