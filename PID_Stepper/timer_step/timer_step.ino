/*******************************************************************/
#include "Timer_Step.h"
#include "Encoder.h"
#include "PID.h"

// Stepper's motor pin
#define dirPin 13
#define pulPin 14
#define enPin 15
#define microStep 32

// Encoder's pin
#define encPinA 16
#define encPinB 17
#define mode 1 // 1 - 2 - 4

TIM_STEP myTIM_STEP;
Encoder myEncoder(encPinA, encPinB);
int pulse = 0;

TimerHandle_t timerPIDMotor = NULL;

/**************************PID**************************/
double kp = 0.5;
double ki = 0.015;

double setPoint = 50.0; // Set the desired motor speed
double e;
double pre_e = 0;
double deltaT = 1.0; // Thời gian lấy mẫu
double speed = 0.0;

// Khai biến khâu tỉ lệ
double up;

// khai biến khâu tích phân 
double ui, ui_p = 0;
int ui_above_limit = 200.0;
int ui_under_limit = 0.0;

// khai biến output
double u;
int u_above_limit = 200.0;
int u_under_limit = 0.0;

float speedfilter;
float speedfilterPrevious;
float alpha = 0.8;

/*********************************************************/

void PID_CAL(TimerHandle_t xTimer) {
  myTIM_STEP.runStep(u);
  
  /******ENCODER*****/
  pulse = myEncoder.encGetPulse();
  Serial.print("pulse before: ");
  Serial.println(pulse);
  speed = (pulse / 400.0) * 60.0;
  Serial.print("speed: ");
  Serial.println(speed);
  
  // pulse = 0;
  Serial.print("pulse after: ");
  Serial.println(pulse);

  // Speed filtering
  speedfilter = (1 - alpha) * speedfilterPrevious + alpha * speed;
  speedfilterPrevious = speedfilter;

  // khoảng cần đáp ứng của hệ thống
  e = setPoint - speed;

  // Khâu tỉ lệ
  up = kp * e;

  // khâu tích phân
  ui = ui_p + ki * e * deltaT;
  // Bão hòa khâu tích phân
  if (ui > ui_above_limit)
    ui = ui_above_limit;
  else if (ui < ui_under_limit)
    ui = ui_under_limit;

  // pre_e = e;
  ui_p = ui;
  // udf_p = udf;

  // Tổng output
  u = up + ui;
  // Bão hòa output
  if (u > u_above_limit)
    u = u_above_limit;
  else if (u < u_under_limit)
    u = u_under_limit;

  Serial.print("u: ");
  Serial.println(u);
}

void setup() {
  Serial.begin(115200);
  myEncoder.encSetMode(mode);
  myTIM_STEP.TIM_STEP_Init(dirPin, pulPin, enPin, microStep);

  timerPIDMotor = xTimerCreate("PID motor",
    1000 / portTICK_PERIOD_MS,
    pdTRUE,
    (void *) 1,
    PID_CAL);

  xTimerStart(timerPIDMotor, portMAX_DELAY);

}

void loop() {}