#include "Arduino.h"
#include "Encoder.h"

Encoder* Encoder::instance[3] = {NULL, NULL, NULL};
int Encoder::count = 0; // Pulse

/*----------------Encoder init----------------*/
Encoder::Encoder(int _aPin, int _bPin)
{
  encA_Pin = _aPin;
  encB_Pin = _bPin;
  pinMode(encA_Pin, INPUT_PULLUP);
  pinMode(encB_Pin, INPUT_PULLUP);
}
/*--------------------------------------------*/

/*----------Instance of each ecnRead----------*/
void Encoder::encReadx1Instance()
{
  if (Encoder::instance[0] != NULL)
  instance[0]->encReadx1();
}

void Encoder::encReadx2Instance()
{
  if (Encoder::instance[1] != NULL)
  instance[1]->encReadx2();
}

void Encoder::encReadx4Instance()
{
  if (Encoder::instance[2] != NULL)
  instance[2]->encReadx4();
}
/*--------------------------------------------*/

/*--------------------Set encoder mode to read (1 - 2 - 4)--------------------*/
void Encoder::encSetMode(int _mode){
  switch(_mode){
    case 1:
      instance[0] = this;
      attachInterrupt(digitalPinToInterrupt(encA_Pin), encReadx1Instance, FALLING);
    break;

    case 2:
      instance[1] = this;
      attachInterrupt(digitalPinToInterrupt(encA_Pin), encReadx2Instance, CHANGE);
    break;

    case 4:
      instance[2] = this;

      p_state = (p_state << 1) | digitalRead(encA_Pin);
      p_state = (p_state << 1) | digitalRead(encB_Pin);
      p_state = p_state & 0x03;

      attachInterrupt(digitalPinToInterrupt(encA_Pin), encReadx4Instance, CHANGE);
      attachInterrupt(digitalPinToInterrupt(encB_Pin), encReadx4Instance, CHANGE);
    break;
    
    default:
    	Serial.println("Error mode!");
  }
}
/*----------------------------------------------------------------------------*/

/*--------------------Encoder mode 1--------------------*/
void IRAM_ATTR Encoder::encReadx1()
{
  encA_State = digitalRead(encA_Pin);
  encB_State = digitalRead(encB_Pin);

  if((encA_State == LOW) && (encB_State == HIGH)){
    count++; // CW
  } 
  else if((encA_State == LOW) && (encB_State == LOW)){
    count--; // CCW
  } 
}
/*-----------------------------------------------------*/

/*--------------------Encoder mode 2--------------------*/
void IRAM_ATTR Encoder::encReadx2()
{
  encA_State = digitalRead(encA_Pin);
  encB_State = digitalRead(encB_Pin);

  if
  (
    ((encA_State == 0) && (encB_State == 1)) | 
    ((encA_State == 1) && (encB_State == 0))
  ) count += 1; 
  else if
  ( 
    ((encA_State == 0) && (encB_State == 0)) | 
    ((encA_State == 1) && (encB_State == 1))
  ) count -= 1;
}
/*------------------------------------------------------*/

/*-------------------------------Encoder mode 4-------------------------------*/
void IRAM_ATTR Encoder::encReadx4()
{
  encA_State = digitalRead(encA_Pin);
  encB_State = digitalRead(encB_Pin);

  // Determine channel for interrupt
  int interruptPin = digitalPinToInterrupt(encA_State) == digitalPinToInterrupt(encA_Pin) ? encA_Pin : encB_Pin;
  state = (state << 1) | encA_State;
  state = (state << 1) | encB_State;
  state = state&0x03; // keep two last bit (0->3)
  // Interrupt for channel A
  if (interruptPin == encA_Pin) {
    switch(state){
      case 2:
        if (p_state == 0) count++;
        else count--;
      break;
      case 1:
        if (p_state == 3) count++;
        else count--;
      break;
      case 0:
        if (p_state == 2) count--;
        else count++;
      break;
      case 3:
        if (p_state == 1) count--;
        else count++;
      break;
    }
    p_state = state;
  }

  // Interrupt for channel B
  else if(interruptPin == encB_Pin){
    switch(state){
      case 3:
        if (p_state == 2) count++;
        else count--;
      break;
      case 0:
        if (p_state == 1) count++;
        else count--;
      break;
      case 1:
        if (p_state == 0) count--;
        else count++;
      break;
      case 2:
        if (p_state == 3) count--;
        else count++;
      break;
    }
    p_state = state;
  }
}
/*----------------------------------------------------------------------------*/

/*-----------Get pulse from encoder-----------*/
int Encoder::encGetPulse(){
  return count;
}
/*--------------------------------------------*/

/*-----------Get pulse from encoder-----------*/
void Encoder::encResetCount(){
  count = 0;
}
/*--------------------------------------------*/
