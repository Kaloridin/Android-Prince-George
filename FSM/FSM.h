/*
	FSM.h - Library for implementing state machine with robot.
	Created by Kaden Kujanpaa, January 16, 2015.
*/
#ifndef FSM_h
#define FSM_h

// Class prototypes
class StateMachine;
class State;
class Robot;

// pg 65 Buckland
class StateMachine 
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
};
class State
{
	public:
		// The virtual declaration allows members to be redefined in derived classes
		virtual ~State();	// Destructor
		virtual void Enter(Robot*) = 0;	// This will execute when the state is entered
		virtual void Execute(Robot*) = 0;	// This is called by the robot's update (loop?) function each update step
		virtual void Exit(Robot*) = 0;	// This will execute when the state is exited
};

class Robot
{
	private:
		StateMachine* r_pStateMachine; // An instance of the state machine class
	
	public:
		Robot();	// Constructor
		~Robot();	// Destructor
		void Update();
		StateMachine* GetFSM();
};

/* Template State
class myState : public State // This is a state derived from the base state class
{
	public:
		virtual void Enter(Robot*); // These should be defined for each new state 
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
		static myState* Instance();
		
		// A state's members/methods can be accessed with int num = myState::Instance()->GetVal();
		// Shortcut: #define MySta MyState::Instance() now you can do int num = MySta->GetVal();
	
}; */

class RobotGlobalState : public State
{
	public:
		virtual void Enter(Robot*);
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
		static RobotGlobalState* Instance();
};
class Rest : public State
{
	public:
		virtual void Enter(Robot*);
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
		static Rest* Instance();
};
class ContractClaw : public State
{
	public:
		virtual void Enter(Robot*);
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
		static ContractClaw* Instance();
};

class Move : public State
{
	public:
		virtual void Enter(Robot*);
		virtual void Execute(Robot*);
		virtual void Exit(Robot*);
		static Move* Instance();
};
#endif