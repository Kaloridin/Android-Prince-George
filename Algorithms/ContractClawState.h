void ContractClaw::Enter(Robot* pRobot)
{	
	clawAngle = 85; // Set claw to closed angle
	myServo.write(clawAngle);
}