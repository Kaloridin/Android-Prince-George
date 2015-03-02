#define DebugRobotStatus(); Status(pRobot->currentState, pRobot->movementType, pRobot->x_coord, pRobot->y_coord, pRobot->hasBall);

const int hBridgeEnable1A = 9; // Corresponds to hbridge 1, motor A
const int hBridgeEnable1B = 2; // Corresponds to hbridge 1, motor B
const int hBridgeEnable2C = 3; // Corresponds to hbridge 2, motor C
const int hBridgeEnable2D = 8; // Corresponds to hbridge 2, motor D
unsigned int optoSensorStateA = 0;
unsigned int optoSensorStateB = 0;
unsigned int optoSensorStateC = 0;
unsigned int optoSensorStateD = 0;
// Motor direction 1 = forward/right
// Motor direction 0 = reverse/left
const int motorControlPinA1 = 22;
const int motorControlPinA2 = 24;
const int motorControlPinB1 = 5;
const int motorControlPinB2 = 4;
const int motorControlPinC1 = 7;
const int motorControlPinC2 = 6;
const int motorControlPinD1 = 28;
const int motorControlPinD2 = 26;
const int pressureSwitchPin = 48;
const int clawServoPin = 10;
const int verticalArmAnglePin = 11;
const int horizontalArmAnglePin = 12;

int pressureSwitchState = LOW;

// Hardcoded Coordinates
const int Hopper1_x = 0;
const int Hopper1_y = -3;
const int Hopper2_x = 0;
const int Hopper2_y = 3;
const int gameboard_x = 0;
const int gameboard_y = 5;
int x_target = 0;
int y_target = 0;

int clawAngle = 180;		// Angle of the claw servo motor | 90 = Closed | 180 = Open |
int verticalArmAngle = 0;	// Angle of the vertical arm servo motor | 0 = Raised | 180 = Lowered |
int horizontalArmAngle = 0;
int driveSpeed = 200;		// Driving speed of motors
int directionFR = 1;		// Direction forward/right
int directionBL = 0;		// Direction backward/left
int currentDirection;

//#include <EEPROM.h>
#include "Arduino.h"
#include <QTRSensors.h>
QTRSensorsRC qtrrc((unsigned char[]) {34, 36, 38, 40}, 4); // create an object for four QTR-xRC sensors on digital pins 14,15,16,17 (analog inputs A0 - A3)
#include <FSM.h>
Robot myRobot;
#include <Debug.h>
#include <Servo.h>
Servo myClawServo; // pin 13
Servo myHorizontalArmServo; // pin 12
Servo myVerticalArmServo; // pin 11
#include <Electromechanics.h>
#include <Algorithms.h>
	
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
	
	pinMode(pressureSwitchPin, INPUT);
	
	// Initialize motors as powered off
	digitalWrite(hBridgeEnable1A, LOW);
	digitalWrite(hBridgeEnable1B, LOW);
	digitalWrite(hBridgeEnable2C, LOW);
	digitalWrite(hBridgeEnable2D, LOW);
	MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
	MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
	MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, 0, 0);
	MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, 0, 0);
	
	// Start calibration phase and move the sensors over both reflectance extremes to be encountered
	/*
	int i;
	for (i = 0; i < 250; i++)
	{
		qtrrc.calibrate();
		delay(20);
	}
	EEPROM.write(0, map(qtrrc.calibratedMinimumOn[0], 0, 4000, 0, 255));
	EEPROM.write(1, map(qtrrc.calibratedMaximumOn[0], 0, 4000, 0, 255));
	EEPROM.write(2, map(qtrrc.calibratedMinimumOn[1], 0, 4000, 0, 255));
	EEPROM.write(3, map(qtrrc.calibratedMaximumOn[1], 0, 4000, 0, 255));
	EEPROM.write(4, map(qtrrc.calibratedMinimumOn[2], 0, 4000, 0, 255));
	EEPROM.write(5, map(qtrrc.calibratedMaximumOn[2], 0, 4000, 0, 255));
	EEPROM.write(6, map(qtrrc.calibratedMinimumOn[3], 0, 4000, 0, 255));
	EEPROM.write(7, map(qtrrc.calibratedMaximumOn[3], 0, 4000, 0, 255));
	*/
	
	myClawServo.attach(clawServoPin);
	myHorizontalArmServo.attach(horizontalArmAnglePin);
	myVerticalArmServo.attach(verticalArmAnglePin);
	ChangeServoAngle(myClawServo, clawAngle);
}

void loop()
{	
	myRobot.Update();
}

/* *********** STATE DICTIONARY ************ */
	void RobotGlobalState::Enter(Robot* pRobot)
	{
		//DebugRobotStatus();
	}
	void RobotGlobalState::Execute(Robot* pRobot)
	{	
		//delay(500); // This delay interferes with constant sensor readings
		//DebugRobotStatus();
		//DebugCalibrate();

	}
	void RobotGlobalState::Exit(Robot* pRobot){}
	
	void Rest::Enter(Robot* pRobot)
	{
		pRobot->currentState = "Rest";
		pRobot->movementType = none;
	}
	void Rest::Execute(Robot* pRobot)
	{		
			delay(3000);
			pRobot->GetFSM()->ChangeState(Motion::Instance());	
	}
	void Rest::Exit(Robot* pRobot){}
	
	void Motion::Enter(Robot* pRobot)
	{	
		pRobot->currentState = "Motion";
	}
	void Motion::Execute(Robot* pRobot)
	{	
		if (pRobot->x_coord == x_target && pRobot->y_coord == y_target)
		{
			pRobot->GetFSM()->RevertToPreviousState(); // Motion finished, transition to previous state
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
	
	void FindBall::Enter(Robot* pRobot)
	{	
		pRobot->currentState = "FindBall";
		// Set coordinates to a hopper
		x_target = 0;
		y_target = 0;
		if (pRobot->x_coord != x_target && pRobot->y_coord != y_target)
		{
			pRobot->GetFSM()->ChangeState(Motion::Instance()); // Travel to specified coordinates
		}
		if (clawAngle != 180)
		{
			clawAngle = 180;
		}
		myClawServo.write(clawAngle);
	}
	void FindBall::Execute(Robot* pRobot)
	{
		// Lower arm to ball grabbing position
		if (verticalArmAngle != 180)
		{
			verticalArmAngle = 180;
			ChangeServoAngle(myVerticalArmServo, verticalArmAngle);
		}
		// Drive until pressure switch trips
		do
		{
			currentDirection = directionFR;
			MotorControl(motorControlPinB1, motorControlPinB2, hBridgeEnable1B, driveSpeed, currentDirection);
			MotorControl(motorControlPinC1, motorControlPinC2, hBridgeEnable2C, driveSpeed, currentDirection);
			pressureSwitchState = digitalRead(pressureSwitchPin);
			delay(5);
			DebugRobotStatus();
		} while (pressureSwitchState != HIGH);
		// Power motors off
		MotorControl(motorControlPinA1, motorControlPinA2, hBridgeEnable1A, 0, 0);
		MotorControl(motorControlPinD1, motorControlPinD2, hBridgeEnable2D, 0, 0);
		// Contract claw
		clawAngle = 90;
		ChangeServoAngle(myClawServo, clawAngle);
		pRobot->hasBall = true;
		// Place ball in gameboard
		if (pRobot->hasBall)
			pRobot->GetFSM()->ChangeState(PlaceBall::Instance());
	}
	void FindBall::Exit(Robot* pRobot)
	{	
		if (verticalArmAngle != 0)
		{
			verticalArmAngle = 0;
			ChangeServoAngle(myVerticalArmServo, verticalArmAngle);
		}
	}
	
	void PlaceBall::Enter(Robot* pRobot)
	{	
		pRobot->currentState = "PlaceBall";
		// Set coordinates to a hopper
		x_target = 0;
		y_target = 5;
		if (pRobot->x_coord != x_target || pRobot->y_coord != y_target)
		{
			pRobot->GetFSM()->ChangeState(Motion::Instance()); // Travel to specified coordinates
		}
		else
		{
			// Open claw to drop ball
			clawAngle = 180;
			ChangeServoAngle(myClawServo, clawAngle);
			pRobot->hasBall = false;
			DebugRobotStatus();
		}
	}
	void PlaceBall::Execute(Robot* pRobot){}
	void PlaceBall::Exit(Robot* pRobot){}

	
