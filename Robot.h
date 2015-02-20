#define DebugRobotStatus(); Status(pRobot->currentState, pRobot->movementType, pRobot->x_coord, pRobot->y_coord, pRobot->hasBall);
// Enumerated types
enum movement {lateral,longitudinal,none};
enum rotation {clockwise,anticlockwise,none};

const int hBridgeEnable1A = 3; // Corresponds to hbridge 1, motor A
const int hBridgeEnable1B = 5; // Corresponds to hbridge 1, motor B
const int hBridgeEnable2C = 6; // Corresponds to hbridge 2, motor C
const int hBridgeEnable2D = 9; // Corresponds to hbridge 2, motor D
const int photoSensorPinA = A1;
const int photoSensorPinB = A2;
const int photoSensorPinC = A3;
const int photoSensorPinD = A0;
int photoSensorStateA = 0;
int photoSensorStateB = 0;
int photoSensorStateC = 0;
int photoSensorStateD = 0;
// Motor direction 1 = forward/right
// Motor direction 0 = reverse/left
int motorControlPinA1 = 12;
int motorControlPinA2 = 13;
int motorControlPinB1 = 7;
int motorControlPinB2 = 8;
int motorControlPinC1 = 4;
int motorControlPinC2 = 2;
int motorControlPinD1 = 10;
int motorControlPinD2 = 11;


float clawAngle = 95;		// Angle of the claw servo motor | 95 = Closed | 170 = Open |
int driveSpeed = 255;		// Driving speed of motors
int directionFR = 1;		// Direction forward/right
int directionBL = 0;		// Direction backward/left
int currentDirection;

int x_target = 0;
int y_target = 0;

#include <FSM.h>
#include <Servo.h>
#include <Electromechanics.h>
#include <Algorithms.h>
#include <Debug.h>

Robot myRobot;
Servo myServo;
	
void setup()
{	
	Serial.begin(9600); // Open serial communication port
	
	pinMode(motorControlPinA1, OUTPUT);
	pinMode(motorControlPinA2, OUTPUT);
	pinMode(motorControlPinB1, OUTPUT);
	pinMode(motorControlPinB2, OUTPUT);
	pinMode(motorControlPinC1, OUTPUT);
	pinMode(motorControlPinC2, OUTPUT);
	pinMode(motorControlPinD1, OUTPUT);
	pinMode(motorControlPinD2, OUTPUT);
	pinMode(hBridgeEnable1A, OUTPUT);
	pinMode(hBridgeEnable1B, OUTPUT);
	pinMode(hBridgeEnable2C, OUTPUT);
	pinMode(hBridgeEnable2D, OUTPUT);
	
	// Initialize motors as powered off
	digitalWrite(hBridgeEnable1A, LOW);
	digitalWrite(hBridgeEnable1B, LOW);
	digitalWrite(hBridgeEnable2C, LOW);
	digitalWrite(hBridgeEnable2D, LOW);
	MotorControl(motorControlPinA1, motorControlPinA2, motorControlPinB1, motorControlPinB2, hBridgeEnable1A, hBridgeEnable1B, 0, 0);
	MotorControl(motorControlPinC1, motorControlPinC2, motorControlPinD1, motorControlPinD2, hBridgeEnable2C, hBridgeEnable2D, 0, 0);

}

void loop()
{	
	myRobot.Update();
}

/* *********** STATE DICTIONARY ************ */
	void RobotGlobalState::Enter(Robot* pRobot){}
	void RobotGlobalState::Execute(Robot* pRobot)
	{	
		//delay(500); // This delay interferes with constant sensor readings
        Serial.write(27);       // ESC command
        Serial.print("[2J");    // clear screen command
        Serial.write(27);
        Serial.print("[H");     // cursor to home command
		DebugRobotStatus();
	}
	void RobotGlobalState::Exit(Robot* pRobot){}
	
	void Rest::Enter(Robot* pRobot)
	{
		pRobot->currentState = "Rest";
		pRobot->movementType = none;
	}
	void Rest::Execute(Robot* pRobot)
	{		
			delay(10000);
			pRobot->GetFSM()->ChangeState(Motion::Instance());	
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
	}
	void Motion::Execute(Robot* pRobot)
	{	
		if (pRobot->x_coord == x_target && pRobot->y_coord == y_target)
		{
			pRobot->GetFSM()->ChangeState(Rest::Instance());	
		}
		if (pRobot->x_coord != x_target) // B,C
		{	
			// Update movement type
			pRobot->movementType = lateral;
			if (pRobot->x_coord < x_target) // Drive forwards (1)
			{	
				currentDirection = directionFR;
			}
			if (pRobot->x_coord > x_target) // Drive reverse (0)
			{	
				currentDirection = directionBL;	
			}		
			// Drive motors corresponding to lateral motion omni wheels
			MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, currentDirection);
			MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, currentDirection);
				
			// Update coordinates
			CoordinateAlgorithm(pRobot->x_coord, pRobot->y_coord, x_target, y_target, pRobot->movementType, pRobot->newIntersection, currentDirection);
		}
		if (pRobot->x_coord == x_target)
		{	// Lateral coordinate reached, power down motors
			MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
			MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
		}
		
		if (pRobot->y_coord != y_target && pRobot->x_coord == x_target) // A,D
		{	
			// Update movement type
			pRobot->movementType = longitudinal;
			if (pRobot->y_coord < y_target) // Drive "right" (1)
			{	
				currentDirection = directionFR;
			}
			if (pRobot->y_coord > y_target) // Drive "left" (0)
			{	
				currentDirection = directionBL;
			}
			// Drive motors corresponding to longitudinal motion omni wheels
			MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, driveSpeed, currentDirection);
			MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, driveSpeed, currentDirection);
				
			// Update coordinates
			CoordinateAlgorithm(pRobot->x_coord, pRobot->y_coord, x_target, y_target, pRobot->movementType, pRobot->newIntersection, currentDirection);
		}
		if (pRobot->y_coord == y_target)
		{	// Longitudinal coordinate reached, power down motors
			MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
			MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
		}
	}
	void Motion::Exit(Robot* pRobot)
	{	// Power down motors
		MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
		MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
		MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
		MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
	}
	
