#include <FSM.h>
#include <Servo.h>
	
	Robot *myRobot;
	Servo myServo;
	const int switchPin = 2;	//
	const int motorPin = 9; 	// 
	int switchState = 0;		// 
	float clawAngle = 95;		// Angle of the claw servo motor | 95 = Closed | 170 = Open |
	
void setup()
{	
	
	pinMode(switchPin, INPUT);
	pinMode(motorPin, OUTPUT);
	
}

void loop()
{	
	myRobot->Update();
}

/* *********** STATE DICTIONARY ************ */
	void RobotGlobalState::Enter(Robot* pRobot){}
	void RobotGlobalState::Execute(Robot* pRobot){}
	void RobotGlobalState::Exit(Robot* pRobot){}
	
	
	void Rest::Execute(Robot* pRobot)
	{
		switchState = digitalRead(switchPin);
		if (switchState == HIGH)
		{
			myRobot->GetFSM()->ChangeState(Move::Instance());
		}
		else
		{
			myRobot->GetFSM()->ChangeState(Rest::Instance());
		}
	}	
	
	
	void ContractClaw::Enter(Robot* pRobot)
	{	
		clawAngle = 85; // Set claw to closed angle
		myServo.write(clawAngle);
	}
	void OpenClaw::Enter(Robot* pRobot)
	{
		clawAngle = 170; // Set claw to open angle
		myServo.write(clawAngle);
	}
	

	
	void Move::Enter(Robot* pRobot)
	{
		digitalWrite(motorPin, HIGH);
	}
	void Move::Execute(Robot* pRobot)
	{
		switchState = digitalRead(switchPin);
		if (switchState == HIGH)
		{
			myRobot->GetFSM()->ChangeState(Move::Instance());
		}
		else
		{
			myRobot->GetFSM()->ChangeState(Rest::Instance());
		}
	}
	
