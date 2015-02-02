PlaceBallState()
{	// Set target coordinates to the gameboard's position
	
	x_f = GameBoard_x;
	y_f = GameBoard_y;
	
	ChangeState(Motion);
	
	// When position is reached program flow will return here
	while (stepAngle != startAngle) // Return angle to starting position 
	// (perpendicular to gameboard)
	{
		stepAngle -= 15;
		Set step motor HIGH
	}
	ChangeState(ScanGameBoard);
	ChangeState(RaiseArm);
}