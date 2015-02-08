/*
	Algorithms.h - Library for algorithms used by robot states
	Created by Kaden Kujanpaa, February 5, 2015.
*/
#ifndef Algorithms_h
#define Algorithms_h

#include <Arduino.h>

// Updates the current coordinates based on grid detection readings from optosensors on base
void CoordinateAlgorithm(int &x_i, int &y_i, int x_f, int y_f, movement movementType, bool &newIntersection) // i = current position, f = final (target) position
{	
	// Update sensor readings
	PhotoResistorReadings(photoSensorPinA, photoSensorPinB, photoSensorPinC, photoSensorPinD, photoSensorStateA, photoSensorStateB, photoSensorStateC, photoSensorStateD);
	
	switch(movementType) // Movement type is either lateral or longitudinal
	{
		case longitudinal:	
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{ 	
								if (750 > photoSensorStateA && 130 > photoSensorStateD)
								{	// Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinA1, motorControlPinA2, motorControlPinD1, motorControlPinD2, hBridgeEnable1A, hBridgeEnable2D, 0, 0);
									if (y_f < y_i) // Final coordinate is south of current position
										y_i -= 1;
									else // Final coordinate is north of current position
										y_i += 1;
								}
							}
							else
							{
								newIntersection = false;
							}
		}
		case lateral:		
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{	
								if (175 > photoSensorStateB &&  800 > photoSensorStateC)
								{ // Grid intersection detected
									newIntersection = true;
									MotorControl(motorControlPinB1, motorControlPinB2, motorControlPinC1, motorControlPinC2, hBridgeEnable1B, hBridgeEnable2C, 0, 0);
									if (x_f < x_i) // Final coordinate is west of current position
										x_i -= 1;
									else // Final coordinate is east of current position
										x_i += 1;
								}
							}
							else
							{
								newIntersection = false;
							}
		}
		// The case of i == f will never occur. When (x_f, y_f) is reached new coordinates
		// will be assigned based on the next state.
	}
}

#endif