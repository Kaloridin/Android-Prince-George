// Updates the current coordinates based on grid detection readings from optosensors on base
CoordinateAlgorithm(int &x_i, int x_f, int &y_i, int y_f) // i = current position, f = final (target) position
{	
	switch(movementType) // Movement type is either lateral or longitudinal
	{
		case Longitudinal:	if sensorC and sensorD != White
							{ // Grid intersection detected
								if y_f < y_i // Final coordinate is south of current position
									y_i -= 1;
								else // Final coordinate is north of current position
									y_i += 1;
							}
		case Lateral:		if sensorA and sensorB != White
							{ // Grid intersection detected
								if x_f < x_i // Final coordinate is west of current position
									x_i -= 1;
								else // Final coordinate is east of current position
									x_i += 1;
							}
							// The case of i == f will never occur. When (x_f, y_f) is reached new coordinates
							// will be assigned based on the next state. 
	}
}