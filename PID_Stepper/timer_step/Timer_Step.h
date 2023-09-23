#ifndef _TIM_STEP_H_
#define _TIM_STEP_H_

#include <Arduino.h>

class TIM_STEP 
{
  public:
    void TIM_STEP_Init(int, int, int, uint8_t); // dir, pul, en, microstep
    void runStep(int16_t); // Input rpm, >0 -> CW, <0 -> CCW

  private:
    int dirPin;
    int pulPin;
    int enPin;
    uint8_t microStep_driver;
    
    static int DIR_motor; // The motor's status and direction
    static int Count_TOP;
    static int Count_BOT;
    static int count;
    static TIM_STEP* instance; // Pointer to the instance
    static void ISRInstance();
    void ISR();

    hw_timer_t *timer = NULL;
};

#endif
