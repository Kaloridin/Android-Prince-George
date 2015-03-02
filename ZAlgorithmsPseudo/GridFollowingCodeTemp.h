switch(movementType) // Movement type is either lateral or longitudinal
	{
		case longitudinal:	
		{					if (!newIntersection) // Check to be sure this isn't the same intersection (and thus incorrectly changing coordinates)
							{
								if (CheckIfBlack('A') || CheckIfBlack('D'))
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
							else if (!CheckIfBlack('A') || !CheckIfBlack('D'))
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
									//OrientationCorrection('B', movementType, currentDirection);
									//OrientationCorrection('C', movementType, currentDirection);
									if (x_f < x_i) // Final coordinate is west of current position
										x_i -= 1;
									else // Final coordinate is east of current position
										x_i += 1;
								}
							}
							//else if (!CheckIfBlack('B') || !CheckIfBlack('C'))
							{
								newIntersection = false;
							}
							break;
		}
		// The case of i == f will never occur. When (x_f, y_f) is reached new coordinates
		// will be assigned based on the next state.
	}