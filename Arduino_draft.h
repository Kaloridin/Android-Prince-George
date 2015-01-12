#include <assert.h>

// Class prototypes
class State;
class Robot;
class Debug;

class Debug
{
	public:
	// Constructor: Opens serial communication port upon instantiation of object
	Debug() 
	{	// Ctrl-Shift-M to open the monitor window (in Arduino client)
		Serial.begin(9600); // Open the port for communication with computer
	} // Constructor
	void print(String line)
	{
		Serial.println(line);
	}
};


class State
{
	public:
	
	// The virtual declaration allows members to be redefined in derived classes
	virtual ~State(){} // Destructor
	
	// This will execute when the state is entered
	virtual void Enter(Robot*)=0;
	
	// This is called by the robot's update (loop?) function each update step
	virtual void Execute(Robot*)=0;
	
	// This will execute when the state is exited
	virtual void Exit(Robot*)=0;
};

class Robot
{
	private:
	
	// A pointer to an instance of a State
	State* r_pCurrentState;
	
	// The coordinates representing the robot's current location
	static float x;
	static float y;
	
	public:
	
	// This must be implemented
	void Update(); // SHOULD THIS BE THE ARDUINO LOOP FUNCTION????
	
	// This method changes the current state to the new state
	void ChangeState(State* pNewState);
	
};

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
void setup()
{
	Debug myDebug;
	myDebug.print("Testing");
}

void loop()
{}