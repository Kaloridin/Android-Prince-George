#define DebugRobotStatus(); Status(pRobot->currentState, pRobot->movementType, pRobot->x_coord, pRobot->y_coord, pRobot->hasBall);

#include <FSM.h>
#include <Debug.h>
#include <Servo.h>

Robot myRobot;
Servo myServo;
const int switchPin = 2;	//
const int motorPin = 9; 	// 
int switchState = 0;		// 
float clawAngle = 95;		// Angle of the claw servo motor | 95 = Closed | 170 = Open |
	
void setup()
{	
	Serial.begin(9600); // Open serial communication port
	pinMode(switchPin, INPUT);
	pinMode(motorPin, OUTPUT);
	
}

void loop()
{	
	myRobot.Update();
}

/* *********** STATE DICTIONARY ************ */
	void RobotGlobalState::Enter(Robot* pRobot){}
	void RobotGlobalState::Execute(Robot* pRobot)
	{
		DebugRobotStatus();
	}
	void RobotGlobalState::Exit(Robot* pRobot){}
	
	void Rest::Enter(Robot* pRobot)
	{
		pRobot->currentState = "Rest";
	}
	void Rest::Execute(Robot* pRobot)
	{
		switchState = digitalRead(switchPin);
		if (switchState == HIGH)
		{
			pRobot->GetFSM()->ChangeState(Motion::Instance());
		}
		else
		{
			pRobot->GetFSM()->ChangeState(Rest::Instance());
		}
	}	
	void Rest::Exit(Robot* pRobot){}
	
	
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
	

	
	void Motion::Enter(Robot* pRobot)
	{	
		pRobot->currentState = "Motion";
		digitalWrite(motorPin, HIGH);
	}
	void Motion::Execute(Robot* pRobot)
	{
		switchState = digitalRead(switchPin);
		if (switchState == HIGH)
		{
			pRobot->GetFSM()->ChangeState(Motion::Instance());
		}
		else
		{
			pRobot->GetFSM()->ChangeState(Rest::Instance());
		}
	}
	void Motion::Exit(Robot* pRobot){}
	
