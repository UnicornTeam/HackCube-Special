//#include "Pin.h"
#include "Arduino.h"

#ifndef HackRadio_H
#define HackRadio_H


#define HackRadio_MAX_CHANGES 200//67

#define PROTOCOL3_SYNC_FACTOR   71
#define PROTOCOL3_0_HIGH_CYCLES  4
#define PROTOCOL3_0_LOW_CYCLES  11
#define PROTOCOL3_1_HIGH_CYCLES  9
#define PROTOCOL3_1_LOW_CYCLES   6

class HackRadio{
public:
	int nReceiverInterrupt;
	int nTransmitterPin;
    int nPulseLength;
    int nRepeatTransmit;
    char nProtocol;

    static String nkey;
    static String nsn;
    static String action;
	static int nReceiveTolerance;
    static unsigned long nReceivedValue;
    static unsigned int nReceivedBitlength;
    static unsigned int nReceivedDelay;
    static unsigned int nReceivedProtocol;
    static unsigned int timings[HackRadio_MAX_CHANGES];

	HackRadio();
	void Begin();
	void setPulseLength(int nPulseLength);
	void setProtocol(int nProtocol);
    void setProtocol(int nProtocol, int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);
    void enableTransmit(int nTransmitterPin);
    void setReceiveTolerance(int nPercent);
    void enableReceive(int interrupt);
    void enableReceive();
	
	static void handleInterrupt();
    static bool receiveProtocol2(unsigned int changeCount);
    static String dec4bin(String data);
    String getKey();
    String getSN();
    String getAction();

    bool available();
    void resetAvailable();

    void sendPreamble();
    void transmit(int nHighPulses, int nLowPulses);
    void Send0();
    void Send1();
    void Send(unsigned long data,unsigned long sn,int action,int Repeat=4);

;
private:


};

#endif
