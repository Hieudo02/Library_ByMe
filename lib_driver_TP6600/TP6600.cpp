// #include "HardwareSerial.h"
#include "Arduino.h"
#include "TP6600.h"

/*
ppr = 360 / StepAngle
pps = (rpm / 60) * ppr
Tdelay = 1 / pps

--------------------------IN PROGRAM--------------------------
pulsePerRound = microstep * 360.0 / angleStep 
// Use "microstep" in case using different microstep division (2, 4, 8, 16, 32)

pulsePerSecond = (_SpeedToRun / 60.0) * pulsePerRound
// _SpeedToRun / 60 -> cover from rpm to rps

(Tdelay) delayInSeconds = 1.0 / pulsePerSecond

// Because we're using "delayMicroseconds" function, so we've to cover from "Seconds" to "Microseconds" -> multiply by 1000000 (on the other hands is divided by 10^-6)
delayInMicroseconds = (1.0 / pulsePerSecond) * 1000000.0;

// To control stepper motor, we've to use delay in 2 sides (up edge and down edge), so the time in Microseconds that we calculated have to divided by 2 to use 2 delay function
delayInMicroseconds = ((1.0 / pulsePerSecond) * 1000000.0) / 2.0 ;

// In conclusion, we'll have:
desiredSpeed = _SpeedToRun;
delayInMicroseconds = ((1.0 / ((desiredSpeed / 60.0) * stepsPerRound)) * 1000000.0) / 2.0 ;
*/

/*-----TP6600 init modify PORT-----*/
TP6600::TP6600(int DIR_PIN, int PUL_PIN, int EN_PIN, uint32_t _MicroStepToRun)
{
  dirPin = DIR_PIN;
  stepPin = PUL_PIN;
  enPin = EN_PIN;

  angleStep = 1.8;
  microStep = _MicroStepToRun;
  pulsePerRound = (microStep * 360 / angleStep);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
}

/*-----Funtion to run step-----*/
void TP6600::runStep(int _delayToRun, int _pulseToRun)
{
  stepStatus = true;
  for (long i = 1; i <= _pulseToRun; i++) 
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(_delayToRun);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(_delayToRun);
  }
};

/*-----TP6600 speed control-----*/
void TP6600::TP6600_setSpeed(float _SpeedToRun, bool _DirecToRun)
{
  desiredSpeed = _SpeedToRun;
  modifiedPulse = 1 * pulsePerRound;
  delayInMicroseconds = (1 / (desiredSpeed/60 * pulsePerRound)  / 2 * 1000000);

  DIR_ROTATION = _DirecToRun;
  digitalWrite(dirPin, DIR_ROTATION ? HIGH : LOW);

  runStep(delayInMicroseconds, modifiedPulse);
}

/*-----TP6600 speed control, modify round-----*/
void TP6600::TP6600_setSpeed(float _SpeedToRun, float _NumberOfRoundToRun, bool _DirecToRun)
{
  desiredSpeed = _SpeedToRun;
  numberOfRound = _NumberOfRoundToRun;
  modifiedPulse = numberOfRound * pulsePerRound;
  delayInMicroseconds = (1 / (desiredSpeed/60 * pulsePerRound)  / 2 * 1000000);

  DIR_ROTATION = _DirecToRun;
  digitalWrite(dirPin, DIR_ROTATION ? HIGH : LOW);
  
  runStep(delayInMicroseconds, modifiedPulse);
}

/*-----TP6600 input degree - out pulse-----*/
int TP6600::TP6600_degreeToPulse(float _degreeToCalculate)
{
  return pulsePerRound * (_degreeToCalculate / 360.0);
}

/*-----TP6600 input round - out pulse-----*/
int TP6600::TP6600_roundToPulse(float _roundToCalculate)
{
  return pulsePerRound * _roundToCalculate;
}

/*-----TP6600 stop stepper motor-----*/
void TP6600::TP6600_stopStep()
{
  digitalWrite(stepPin, LOW);
  stepStatus = false;
  // runStep(0);
}

/*-----TP6600 running status-----*/
bool TP6600::TP6600_isRunning()
{
  return stepStatus ? true : false;
}

float TP6600::TP6600_accelerate(float _SpeedToRun, float _desiredTime)
{
  // T sample
  desiredSpeed = _SpeedToRun;
  delayInMicroseconds = 1.0 / (desiredSpeed/60 * pulsePerRound)  / 2.0 * 1000000;
  Serial.println(delayInMicroseconds);
  // accel calculate
  accel = ((_SpeedToRun / 60) - preSpeed) / _desiredTime;
  // divided by 60 to cover from rpm to rps

  if(_SpeedToRun > preSpeed){
    accelFlag = 1;
    preSpeed += accel * ((delayInMicroseconds*2)/1000000);
  }
  else if(_SpeedToRun < preSpeed){
    accelFlag = 0;
    preSpeed -= accel * ((delayInMicroseconds*2)/1000000);
  }

  if(preSpeed >= _SpeedToRun && accelFlag == 1) preSpeed = _SpeedToRun;
  else if(preSpeed <= _SpeedToRun && accelFlag == 0) preSpeed = _SpeedToRun;

  return preSpeed * 60;
  // mutiply by 60 to cover back (rps to rpm)
}

void TP6600::TP6600_setHome(int _SpeedToRun, bool _DirecToRun, int _SensorPin)
{
  desiredSpeed = _SpeedToRun;
  delayInMicroseconds = (1 / (desiredSpeed/60 * pulsePerRound)  / 2 * 1000000);

  DIR_ROTATION = _DirecToRun;
  digitalWrite(dirPin, DIR_ROTATION ? HIGH : LOW);

  sensorPin = _SensorPin;
  digitalWrite(sensorPin, INPUT);
  bool homeFlagCheck = true;
  
  while(homeFlagCheck){
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delayInMicroseconds);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delayInMicroseconds);

    // Stop when catched sensor's signal
    if(digitalRead(sensorPin) == 0){
        digitalWrite(stepPin, LOW);
        Serial.println(digitalRead(sensorPin));
        homeFlagCheck = false;
    }
  }
}




