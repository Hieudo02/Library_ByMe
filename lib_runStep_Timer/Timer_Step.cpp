#include "esp32-hal-timer.h"
#include "Timer_Step.h"
#include <Arduino.h>

TIM_STEP* TIM_STEP::instance = NULL; // Initialize the pointer to NULL

int TIM_STEP::DIR_motor;
int TIM_STEP::Count_TOP;
int TIM_STEP::Count_BOT;
int TIM_STEP::count;

void TIM_STEP::ISRInstance()
{
  if (TIM_STEP::instance != NULL) {
    instance->ISR();
  }
}

void IRAM_ATTR TIM_STEP::ISR() {
  if (instance != NULL) {
    if (DIR_motor != 0) { // > or <
      count++;
      if(count <= Count_TOP) digitalWrite(pulPin, HIGH); // run
      else digitalWrite(pulPin, LOW); // stop

      if (count >= Count_BOT) {
        count = 0; // done 1 count
      }
    }
  }
}

/* dirPin:
  < 0: CCW
  > 0: CW
  = 0: Stop
*/ 

/*-----------------------------------Init-----------------------------------*/
void TIM_STEP::TIM_STEP_Init(int _dirPin, int _pulPin, int _enPin, uint8_t _microStepToRun){
  dirPin = _dirPin;
  pulPin = _pulPin;
  enPin = _enPin;
  microStep_driver = _microStepToRun;

  pinMode(dirPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  instance = this;

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &TIM_STEP::ISRInstance, true);
  timerAlarmWrite(timer, 5, true);
  timerAlarmEnable(timer);
}

/*---------------------------------Run step---------------------------------*/
void TIM_STEP::runStep(int16_t rpm) {
  int16_t counterNum = 60 / (rpm * (microStep_driver*200) * 0.000005);
  Count_BOT = abs(counterNum);
  Count_TOP = abs(counterNum) / 2;

  // Check speed (which is typed by user) -> Derive the direction
  if (rpm < 0) {
    DIR_motor = -1; // CCW
    digitalWrite(dirPin, HIGH); // CCW
  } 
  else if (rpm > 0) {
    DIR_motor = 1; // CW
    digitalWrite(dirPin, LOW); // CW
  } 
  else DIR_motor = 0; // This case if user type 0 rpm    
}
