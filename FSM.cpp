/*
	FSM.cpp - Library for implementing state machine with robot.
	Created by Kaden Kujanpaa, January 16, 2015.
*/

#include "Arduino.h"
#include "FSM.h"
/* *********** STATE MACHINE CLASS *********** */
	StateMachine::StateMachine(Robot* owner)
	{
		r_pOwner = owner;
		r_pCurrentState = NULL;
		r_pPreviousState = NULL;
		r_pGlobalState = NULL;
	}
	void StateMachine::SetCurrentState(State* s){r_pCurrentState = s;}
	void StateMachine::SetGlobalState(State* s){r_pGlobalState = s;}
	void StateMachine::SetPreviousState(State* s){r_pPreviousState = s;}
	
	void StateMachine::Update()
	{
		// If a global state exists, call its execute method
		if (r_pGlobalState)		r_pGlobalState->Execute(r_pOwner);
		// Same for the current state
		if (r_pCurrentState)	r_pCurrentState->Execute(r_pOwner);
	}
	
	void StateMachine::ChangeState(State* pNewState) // Change to a new state
	{
		// Keep a record of the previous state
		r_pPreviousState = r_pCurrentState;
		// Call the exit method of the existing state
		r_pCurrentState->Exit(r_pOwner);
		// Change state to the new state
		r_pCurrentState = pNewState;
		// Call the entry method of the new state
		r_pCurrentState->Enter(r_pOwner);
	}
	
	void StateMachine::RevertToPreviousState()
	{
		ChangeState(r_pPreviousState);
	}
	
	State* CurrentState(){return r_pCurrentState;}
	State* GlobalState(){return r_pGlobalState;}
	State* PreviousState(){return r_pPreviousState;}
/* *********** STATE CLASS *********** */
	// The virtual declaration allows members to be redefined in derived classes
	virtual State::~State(){} // Destructor
	
	// This will execute when the state is entered
	virtual void State::Enter(Robot*)=0;
	
	// This is called by the robot's update (loop?) function each update step
	virtual void State::Execute(Robot*)=0;
	
	// This will execute when the state is exited
	virtual void State::Exit(Robot*)=0;

/* *********** ROBOT CLASS *********** */
	Robot::Robot()
	{
		// Set up state machine
		r_pStateMachine = new StateMachine(this);
		
		r_pStateMachine->SetCurrentState(/*<state here>::Instance()*/);
		r_pStateMachine->SetGlobalState(RobotGlobalState::Instance());
	}
	
	Robot::~Robot(){delete r_pStateMachine;}
	
	void Robot::Update()
	{
		r_pStateMachine->Update();
	}
	
	// This method changes the current state to the new state
	void Robot::ChangeState(State* pNewState)
	{
		// Make sure both states are valid before attempting to call their methods
		assert(r_pCurrentState && pNewState);
	
		// Call the exit method of the existing state
		r_pCurrentState->Exit(this);
	
		// Change state to the new state
		r_pCurrentState = pNewState;
	
		// Call the entry method of the new state
		r_pCurrentState->Enter(this);
	}
	
	StateMachine* Robot::GetFSM(){return r_pStateMachine;}

/* *********** STATE DICTIONARY ************ */
	virtual void Rest::Enter(Robot* pRobot){}
	virtual void Rest::Execute(Robot* pRobot){}
	virtual void Rest::Exit(Miner* pRobot){}


