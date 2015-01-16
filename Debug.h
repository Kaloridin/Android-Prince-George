/*
	Debug.h - Library for debugging robot.
	Created by Kaden Kujanpaa, January 16, 2015.
*/
#ifndef FSM_h
#define FSM_h

#include "Arduino.h"

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