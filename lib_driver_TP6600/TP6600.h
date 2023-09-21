#ifndef _TP6600_H_
#define _TP6600_H_

#include <stdio.h>
#include "Arduino.h"

class TP6600
{
  public:
    TP6600(int, int, int, uint32_t);
    void TP6600_setSpeed(float, bool);
    void TP6600_setSpeed(float, float, bool); 
    int TP6600_degreeToPulse(float);
    int TP6600_roundToPulse(float);
    void TP6600_stopStep();
    bool TP6600_isRunning(); 
    float TP6600_accelerate(float, float);
    void TP6600_setHome(int, bool, int);

  private:
    void runStep(int, int);

    uint32_t microStep; 
    float angleStep; 
    unsigned long pulsePerRound;
    float desiredSpeed;
    uint32_t delayInMicroseconds;
    long modifiedPulse;
    float numberOfRound;
    float preSpeed;
    float accel;

    unsigned long step_delay;
    unsigned long last_step_time ;
    bool event;

    // PORT
    int dirPin;
    int stepPin;
    int enPin;

    // DIRECTION VAR
    bool DIR_ROTATION;

    // Flag to check Step's status (Run or Not)
    bool stepStatus = false;

    // Flag to use in accelaration's equation
    bool accelFlag = 0;

    // Pin of sensor (used to set stepper's home)
    int sensorPin;
};

#endif //_TP6600_H_