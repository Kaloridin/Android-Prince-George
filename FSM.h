/*
	FSM.h - Library for implementing state machine with robot.
	Created by Kaden Kujanpaa, January 16, 2015.
*/
#ifndef FSM_h
#define FSM_h

#include "Arduino.h"

class StateMachine // pg 65, Buckland
{
	private:
		// Pointer to the agent that owns this instance
		Robot* r_pOwner;
		
		State* r_pCurrentState;	// Keeps track of the previous state
		State* r_pPreviousState;	// A record of the last state the agent was in
		State* r_pGlobalState;	// This state logic is called every time the FSM is updated
		
	public:
		StateMachine(Robot* owner); // Constructor
		
		// Use these methods to initialize the FSM
		void SetCurrentState(State* s);
		void SetGlobalState(State* s);
		void SetPreviousState(State* s);
		
		// Call this to update the FSM
		void Update();
		void ChangeState(State* pNewState);
		void RevertToPreviousState();
		
		// Accessors
		State* CurrentState();
		State* GlobalState();
		State* PreviousState();
}
class State
{
	public:
		// The virtual declaration allows members to be redefined in derived classes
		virtual ~State();	// Destructor
		virtual void Enter(Robot*);	// This will execute when the state is entered
		virtual void Execute(Robot*);	// This is called by the robot's update (loop?) function each update step
		virtual void Exit(Robot*);	// This will execute when the state is exited
};

class Robot
{
	private:
		StateMachine* r_pStateMachine; // An instance of the state machine class
	
	public:
		Robot();	// Constructor
		~Robot();	// Destructor
		void Update();
		void ChangeState(State* pNewState);	// This method changes the current state to the new state
		StateMachine* GetFSM();
};

class Rest : public State
{
	public:
		virtual void Enter(Robot*);
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
}
class ContractClaw : public State
{
	public:
		virtual void Enter(Robot*);
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
}
#endif