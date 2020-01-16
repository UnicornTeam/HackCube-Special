#include "Arduino.h"


#ifndef HackCube_H
#define HackCube_H

#include "Pin.h"

class HackCube{

public:
	Cube(void);
	// Initialisatie module
	void Begin();
	// Stel module io in op de overeenkomstige invoer- en uitvoermodus
	void pinSetup();

private:
	int pin;

};

#endif
