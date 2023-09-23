#ifndef _PID_H_
#define _PID_H_
#include <stdio.h>
class PID
{
  public:
    float Kp, Ki, Kd, Output;
    float Sample_time;
    float WindupMax, WindupMin;
    float OutMax, OutMin;
    float PTerm, ITerm, DTerm;
    float Last_Error;
  public:
    PID();
    PID(float, float, float, float);
    void setPIDparamters(float, float, float);
    void setWindup(float, float);
    void setOutput(float, float);
    void setSampleTime(float);
    float getPIDcompute(float);
};
#endif //_PID_H_
