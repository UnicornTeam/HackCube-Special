#include "Arduino.h"


#ifndef HackCube_H
#define HackCube_H

#include "Pin.h"

class HackCube{

public:
	Cube(void);
	// 初始化模块
	void Begin();
	// 将模块io设置为对应的输入输出模式
	void pinSetup();

private:
	int pin;

};

#endif
