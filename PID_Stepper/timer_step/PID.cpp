#include "PID.h"


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm khởi tạo đối tượng PID--------//
PID::PID()
{
  this->Kp=1;
  this->Ki=0;
  this->Kd=0;
  this->Sample_time=1000;
  this->WindupMax=0;
  this->WindupMin=0;
  this->OutMax=0;
  this->OutMin=0;
  this->ITerm=0;
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//



//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm khoiwe tạo đối tương PID có truyền thông số---------//
PID::PID(float Kp, float Ki, float Kd, float Sample_time)
{
  this->Kp=Kp;
  this->Ki=Ki;
  this->Kd=Kd;
  this->Sample_time=Sample_time;
  this->WindupMax=0;
  this->WindupMin=0;
  this->OutMax=0;
  this->OutMin=0;   
  this->ITerm=0; 
  this->PTerm=0; 
  this->DTerm=0; 
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm thay đổi hệ số PID------------//
void PID::setPIDparamters(float Kp=NULL, float Ki=NULL, float Kd=NULL)
{
  if(Kp != NULL) this->Kp=Kp;
  else this->Kp=0;
  if(Ki != NULL) this->Ki=Ki;
  else this->Ki=0;
  if(Kd != NULL) this->Kd=Kd;   
  else this->Kd=0;
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm tính toán PID-----------------//
float PID::getPIDcompute(float Error)
{
  // PTERM
  this->PTerm = this->Kp*Error;

  // ITERM
  this->ITerm += this->Ki*Error*(this->Sample_time/1000);
  if(this->ITerm > this->WindupMax) this->ITerm = this->WindupMax;
  else if(this->ITerm < this->WindupMin) this->ITerm = this->WindupMin;

  //DTERM
  this->DTerm = (Error-this->Last_Error)/(this->Sample_time/1000);
  this->Last_Error = Error;
  //OUTPUT
  this->Output = this->PTerm + this->ITerm + this->DTerm*this->Kd;
	if(this->Output > this->OutMax) this->Output = this->OutMax;
	else if(this->Output < this->OutMin) this->Output = this->OutMin;

	return this->Output;   
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//



//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm set bão hòa khâu I------------//
void PID::setWindup(float MinValue, float MaxValue)
{
  this->WindupMax=MaxValue;
  this->WindupMin=MinValue;
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm set bão hòa OUTPUT------------//
void PID::setOutput(float MinValue, float MaxValue)
{
  this->OutMax=MaxValue;
  this->OutMin=MinValue;
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
//----------------Hàm set thời gian lấy mẫu---------//
void PID::setSampleTime(float value) 
{
  this->Sample_time=value;
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM//
