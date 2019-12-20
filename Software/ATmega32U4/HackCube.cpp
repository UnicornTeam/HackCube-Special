#include "HackCube.h"

HackCube::Cube(void) {

};

void HackCube::Begin() {

};

void HackCube::pinSetup() {
    pinMode(nRF_CS_1, OUTPUT);
    pinMode(nRF_CE, OUTPUT);

    pinMode(RF433_CS, OUTPUT);
    pinMode(RF433_GDO0, OUTPUT);
    pinMode(RF433_GDO2, INPUT);

    digitalWrite(RF433_CS, HIGH);
    digitalWrite(RF433_GDO0, LOW);
    digitalWrite(RF315_CS, HIGH);
    digitalWrite(RF315_GDO0, LOW);

    pinMode(RF315_CS, OUTPUT);
    pinMode(RF315_GDO0, OUTPUT);
    pinMode(RF315_GDO2, INPUT);

    pinMode(RF_EN, OUTPUT);
    digitalWrite(RF_EN, HIGH);

    pinMode(ID_OUT, INPUT);
    pinMode(MOD, OUTPUT);
    pinMode(SHD, OUTPUT);

    digitalWrite(MOD, LOW);
    digitalWrite(SHD, LOW);
};
