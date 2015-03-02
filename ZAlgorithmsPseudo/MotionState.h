// "Motion" state controls the motors and navigates to final input coordinates
MotionState(int x_i, int x_f, int y_i, int y_f) // i = current position, f = final (target) position
{
	if CheckSwitch // This will be present in an "update method", but is included here for readability
		changeState(ContractClaw) // Pressure switch was activated, change to contract claw state
	
	while no obstacle:	
		while x_i != x_f
		{
			Set motor 1 and motor 2 HIGH //drive motors corresponding to lateral motion omni wheels
			movementType = Lateral;
			CoordinateAlgorithm(x_i, x_f, y_i, y_f); // Update current coordinates when gridline detected
		}
		Set motor 1 and motor 2 LOW // Turn off lateral motors
		while y_i != y_f
		{
			Set motor 3 and motor 4 HIGH //drive motors corresponding to longitudinal motion omni wheels
			movementType = Longitudinal;
			CoordinateAlgorithm(x_i, x_f, y_i, y_f); // Update current coordinates when gridline detected
		}
		Set motor 3 and motor 4 LOW // Turn off longitudinal motors
		
		RevertToPreviousState(); // Return to whichever state called "Motion"
	else
		ObstacleAlgorithm(); // Divert path to avoid obstacle
}