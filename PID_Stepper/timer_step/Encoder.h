#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "Arduino.h"

class Encoder
{
  public:
    Encoder(int, int); // Input channel A and B
    void encSetMode(int); // Mode: x1, x2, x4
    int encGetPulse(); // Get pulse count from encoder

  private:
    void encReadx1();
    void encReadx2();
    void encReadx4();
    static Encoder * instance[3];
    static void encReadx1Instance();
    static void encReadx2Instance();
    static void encReadx4Instance();

    int encA_Pin;
    int encB_Pin;
    int encB_State;
    int encA_State;

    // Used in x4
    int state;
    int p_state;

    static int count; // Pulse count
    
};

#endif