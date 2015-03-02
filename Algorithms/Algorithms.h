/*
	Algorithms.h - Library for algorithms used by robot states
	Created by Kaden Kujanpaa, February 5, 2015.
*/
#ifndef Algorithms_h
#define Algorithms_h

#include <Arduino.h>
// Prototypes
bool CheckIfBlack(char sensor);
void Rotate(char sensor, rotation rotationType);
void OrientationCorrection(char sensor, movement movementType, int currentDirection);


// Updates the current coordinates based on grid detection readings from optosensors on base
void CoordinateAlgorithm(int &x_i, int &y_i, int x_f, int y_f, movement movementType, bool &newIntersection, int currentDirection) // i = current position, f = final (target) position
{	
	// Update sensor readings
	// OptoSensorReading();
	
	switch(movementType)
	{
				case longitudinal:	
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{
								if (CheckIfBlack('A') || CheckIfBlack('D'))
								{	// Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0,0);
									MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0,0);
									OrientationCorrection('A', movementType, currentDirection);
									OrientationCorrection('D', movementType, currentDirection);

									if (y_f < y_i) // Final coordinate is south of current position
										y_i -= 1;
									else // Final coordinate is north of current position
										y_i += 1;
								}						
							}
							else if (!CheckIfBlack('A') && !CheckIfBlack('D'))
							{
								newIntersection = false;
							}
							break;
		}
		case lateral:		
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{	
								if (CheckIfBlack('B') || CheckIfBlack('C'))
								{ // Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
									MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
									OrientationCorrection('B', movementType, currentDirection);
									OrientationCorrection('C', movementType, currentDirection);
									if (x_f < x_i) // Final coordinate is west of current position
										x_i -= 1;
									else // Final coordinate is east of current position
										x_i += 1;
								}
							}
							else if (!CheckIfBlack('B') && !CheckIfBlack('C'))
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

bool CheckIfBlack(char sensor)
{	//int tolerance = 500; // Black is high, white is low
	
	// Update sensor readings
	OptoSensorReading();
	
	switch(sensor) // Update sensor reading
	{ // < 2000 for white lines
		case 'A':	if (optoSensorStateA > 500) // 500
						return true;
					break;
		case 'B':	if (optoSensorStateB > 500) // 1500
						return true;
					break;
		case 'C':	if (optoSensorStateC > 500) // 1500
						return true;
					break;
		case 'D':	if (optoSensorStateD > 500) // 500
						return true;
					break;
	}
	/*
	switch(sensor) // Update sensor reading
	{
		case 'A':	if (abs(optoSensorStateA - qtrrc.calibratedMaximumOn[0]) < abs(optoSensorStateA - qtrrc.calibratedMinimumOn[0]))
						return true;
					break;
		case 'B':	if (abs(optoSensorStateB - qtrrc.calibratedMaximumOn[1]) < abs(optoSensorStateB - qtrrc.calibratedMinimumOn[1]))
						return true;
					break;
		case 'C':	if (abs(optoSensorStateC - qtrrc.calibratedMaximumOn[2]) < abs(optoSensorStateC - qtrrc.calibratedMinimumOn[2]))
						return true;
					break;
		case 'D':	if (abs(optoSensorStateD - qtrrc.calibratedMaximumOn[3]) < abs(optoSensorStateD - qtrrc.calibratedMinimumOn[3]))
						return true;
					break;
	}
	*/
	
	// Default to returning not black -> false
	return false;
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
		case 'A':	while (!CheckIfBlack('D'))
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
		case 'B':	while (!CheckIfBlack('C'))
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
		case 'C':	while (!CheckIfBlack('B'))
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
		case 'D':	while (!CheckIfBlack('A'))
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
					/* while (!CheckIfBlack('B') || !CheckIfBlack('C'))
					{	
						MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 0);
						MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 0);
					} */
					break;
		case 'B':	if (currentDirection == 0)
						Rotate('B', anticlockwise);
					else
						Rotate('B', clockwise);
					// Move down until sensor A,D trip
					/* while (!CheckIfBlack('A') || !CheckIfBlack('D'))
					{	
						MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 0);
						MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 0);
					} */
					break;
		case 'C':	if (currentDirection == 0)
						Rotate('C', clockwise);
					else
						Rotate('C', anticlockwise);
					// Move up until sensor A,D trip
					/* while (!CheckIfBlack('A') || !CheckIfBlack('D'))
					{	
						MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, 1);
						MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, 1);
					} */
					break;
		case 'D':	if (currentDirection == 0)
						Rotate('D', anticlockwise);
					else
						Rotate('D', clockwise);
					// Move right until sensor B,C trip
					/* while (!CheckIfBlack('B') || !CheckIfBlack('C'))
					{	
						MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, 1);
						MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, 1);
					} */
					break;
	}
	// Power down all motors
	MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
	MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
	MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
	MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
}

#endif