/*
	Algorithms.h - Library for algorithms used by robot states
	Created by Kaden Kujanpaa, February 5, 2015.
*/
#ifndef Algorithms_h
#define Algorithms_h

#include <Arduino.h>
// Prototypes
bool CheckIfBlack(char sensor, int blackValue, int whiteValue);
void Rotate(char sensor, rotation rotationType);
void OrientationCorrection(char sensor, movement movementType, int currentDirection);


// Updates the current coordinates based on grid detection readings from optosensors on base
void CoordinateAlgorithm(int &x_i, int &y_i, int x_f, int y_f, movement movementType, bool &newIntersection, int currentDirection) // i = current position, f = final (target) position
{	
	/* Update sensor readings */
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
								if (CheckIfBlack('A', blackValueA, whiteValueA) || CheckIfBlack('D', blackValueD, whiteValueD))
								{	// Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0,0);
									MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0,0);
									//OrientationCorrection('A', movementType, currentDirection);
									//OrientationCorrection('D', movementType, currentDirection);
									if (y_f < y_i) // Final coordinate is south of current position
										y_i -= 1;
									else // Final coordinate is north of current position
										y_i += 1;
								}
							}
							// if (870 < photoSensorStateA && 200 < photoSensorStateD)
							else if (!CheckIfBlack('A', blackValueA, whiteValueA) && !CheckIfBlack('D', blackValueD, whiteValueD))
							{
								newIntersection = false;
							}
							break;
		}
		case lateral:		
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{	// (A:220, B:850) -> Working values in lab
								//if (220 > photoSensorStateB ||  850 > photoSensorStateC)
								if (CheckIfBlack('B', blackValueB, whiteValueB) || CheckIfBlack('C', blackValueC, whiteValueC))
								{ // Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
									MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
									//OrientationCorrection('B', movementType, currentDirection);
									//OrientationCorrection('C', movementType, currentDirection);
									if (x_f < x_i) // Final coordinate is west of current position
										x_i -= 1;
									else // Final coordinate is east of current position
										x_i += 1;
								}
							}
							// if (220 < photoSensorStateB && 850 < photoSensorStateC)
							else if (!CheckIfBlack('B', blackValueB, whiteValueB) && !CheckIfBlack('C', blackValueC, whiteValueC))
							{
								newIntersection = false;
							}
							break;
		}
		// The case of i == f will never occur. When (x_f, y_f) is reached new coordinates
		// will be assigned based on the next state.
	}
}
/*
void ObstacleAlgorithm(int x_f, int y_f)
{
	// Save target coordinates
	int target_x = x_f;
	int target_y = y_f;
	
	// Set temporary new coordinates for path diversion
	// Sends robot to closest edge (dodging obstacles)
	if (x_i < target_x)
		x_f = x_i - 1; // Towards west edge
	else
		x_f = x_i + 1; // Towards east edge
	if (y_i < target_y)
		y_f = y_i - 1; // Towards south edge
	else
		y_f = y_i + 1; // Towards top edge
	
	// Move to diverted location
} */

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
	if (blackValue - 30 < currentValue && currentValue > blackValue + 30)
		return true;
	else if (whiteValue - 30 < currentValue && currentValue > whiteValue + 30)
		return false;
	else
	{	Debug("Photoresistor Value Error\nSensor:");
		Debug(sensor);
		Debug(currentValue);
	}
	
	/*
	if (abs(currentValue - blackValue) < abs(currentValue - whiteValue)) // Check metrics
		return true;
	else
		return false;
	*/
}

void Rotate(char sensor, rotation rotationType)
{
	switch(sensor)
	{
		case 'A':	while (!CheckIfBlack('D', blackValueD, whiteValueD))
					{
						if (rotationType == clockwise)
						{
							MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 0);
							MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 1);
							MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 0);
						}
						else if (rotationType == anticlockwise)
						{
							MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 1);
							MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 0);
							MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 1);
						}
					}
					break;
		case 'B':	while (!CheckIfBlack('C', blackValueC, whiteValueC))
					{
						if (rotationType == clockwise)
						{
							MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 1);
							MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 1);
							MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 0);
						}
						else if (rotationType == anticlockwise)
						{
							MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 0);
							MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 0);
							MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 1);
						}
					}
					break;
		case 'C':	while (!CheckIfBlack('B', blackValueB, whiteValueB))
					{
						if (rotationType == clockwise)
						{
							MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 1);
							MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 0);
							MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 0);
						}
						else if (rotationType == anticlockwise)
						{
							MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 0);
							MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 1);
							MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 1);
						}
					}
					break;
		case 'D':	while (!CheckIfBlack('A', blackValueA, whiteValueA))
					{
						if (rotationType == clockwise)
						{
							MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 1);
							MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 0);
							MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 1);
						}
						else if (rotationType == anticlockwise)
						{
							MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 0);
							MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 1);
							MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 0);
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
					while (!CheckIfBlack('B', blackValueB, whiteValueB) && !CheckIfBlack('C', blackValueC, whiteValueC))
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
					while (!CheckIfBlack('A', blackValueA, whiteValueA) && !CheckIfBlack('D', blackValueD, whiteValueD))
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
					while (!CheckIfBlack('A', blackValueA, whiteValueA) && !CheckIfBlack('D', blackValueD, whiteValueD))
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
					while (!CheckIfBlack('B', blackValueB, whiteValueB) && !CheckIfBlack('C', blackValueC, whiteValueC))
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