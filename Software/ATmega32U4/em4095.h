#include "HackCube.h"
#include "Arduino.h"

#ifndef EM4095_H
#define EM4095_H

#define DELAYVAL    320   //384 //standard delay for manchster decode
#define TIMEOUT     1000  //standard timeout for manchester decode
#define delay_125     250  //standard timeout for manchester decode

class EM4095{

public:
	int vd;
	int tempdata;
	bool decodeTag(unsigned long &udata,int &ReadVd);
	void Start();
	void bit0();
	void bit1();
	void transmit(int  * data);
	void transmit(unsigned long long  data);
	void  simulation_bak(int vd, unsigned long id);
	void  simulation(int vd, unsigned long id);
	int  *  parity_check(int  * x);

private:

};

#endif
