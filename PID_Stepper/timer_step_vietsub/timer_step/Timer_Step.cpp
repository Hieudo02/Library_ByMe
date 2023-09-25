#include "Timer_Step.h"
#include <Arduino.h>

TIM_STEP* TIM_STEP::BienBanSao = NULL; // Khởi tạo giá trị bản sao là NULL

// Khi dùng biến static, ta cần khai báo ô nhớ dùng chung cho toàn class (mặc định giá trị ô nhớ là 0)
int TIM_STEP::KiemTraChieuQuay;
int TIM_STEP::GiaTriDemCuaTimerTOP;
int TIM_STEP::GiaTriDemCuaTimerBOT;
int TIM_STEP::GiaTriDemCuaTimer;

void TIM_STEP::BanSaoHamDemNgatTimer()
{
  if (TIM_STEP::BienBanSao != NULL) {
    BienBanSao->DemNgatTimer();
  }
}


void IRAM_ATTR TIM_STEP::DemNgatTimer() {
  if (BienBanSao != NULL) {
    if (KiemTraChieuQuay != 0) { 
      GiaTriDemCuaTimer++;
      if(GiaTriDemCuaTimer <= GiaTriDemCuaTimerTOP) digitalWrite(ChanPUL, HIGH); // Chạy step
      else digitalWrite(ChanPUL, LOW); // Ngưng chạy step

      if (GiaTriDemCuaTimer >= GiaTriDemCuaTimerBOT) { // Khi giá trị đếm vượt qua BOT của xung cũng là lúc hết 1 xung, khi đó đã xong 1 đếm
        GiaTriDemCuaTimer = 0; 
      }
    }
  }
}

/*---------------------Hàm khởi tạo động cơ Step---------------------*/
void TIM_STEP::KhoiTaoStep(int _ChanDIR, int _ChanPUL, int _ChanEN, uint8_t _CheDoViBuoc){
  ChanDIR = _ChanDIR;
  ChanPUL = _ChanPUL;
  ChanEN = _ChanEN;
  CheDoViBuoc = _CheDoViBuoc;

  pinMode(ChanDIR, OUTPUT);
  pinMode(ChanPUL, OUTPUT);
  pinMode(ChanEN, OUTPUT);

  BienBanSao = this;

  BienTimer = timerBegin(0, 80, true); 
  timerAttachInterrupt(BienTimer, &TIM_STEP::BanSaoHamDemNgatTimer, true);
  timerAlarmWrite(BienTimer, 5, true);
  timerAlarmEnable(BienTimer);

  /*
  timerBegin(uint8_t num, uint16_t divider, bool countUp);
    + num: Chọn bộ timer (4 bộ: 0, 1, 2, 3)
    + divider: Chọn bộ chia (prescaler) (ESP32 có tần số 80MHz, chọn bộ chia 80 sẽ ra 1MHz => Chu kỳ đếm 1 lần = 1/1MHz = 1 micro giây)
    + countUp: "true" là đếm lên, "false" là đếm xuống

  timerAttachInterrupt(hw_timer_t *timer, void (*fn)(void), bool edge);
    + timer: biến "BienTimer"
    + fn: hàm được gọi khi có ngắt timer xảy ra
    + edge: cạnh ngắt (true cạnh lên/ false cạnh xuống), tuy nhiên thư viện hiện chỉ hỗ trợ LEVEL (không quan tâm cạnh lên hay xuống) 
      => Biến này không ảnh hưởng đến hàm

  timerAlarmWrite(hw_timer_t *timer, uint64_t alarm_value, bool autoreload);
    + timer: biến "BienTimer"
    + alarm_value: số lần đếm của timer (trong chương trình 1 lần đếm 1us => 5x1 = 5us)
    + autoreload: nếu đặt "true" thì timer sẽ reload giá trị khi đếm xong, nếu đặt "false" thì sẽ không reload

  timerAlarmEnable(hw_timer_t *timer);
    + timer: biến "BienTimer"
  */
}

/*--------------------------Hàm chạy động cơ Step--------------------------*/
  /*
    Từ phần khởi tạo, ta có 5 lần đếm ứng với 5us = 0.000005s

    Thời gian của 1 xung: T = x * 0.000005 (s) (với x là số đếm (count))
    
    Số xung trên 1 vòng: ppr = (Số vi bước * 360/1.8) = Số vi bước * 200 (với Số vi bước: 1, 2, 4, 8, 16, 32) 
    
    Suy ra:
      Thời gian quay 1 vòng = ppr * T = Số vi bước * 200 * 0.000005 * x (s) 

      Vậy 1 giây quay được: [(1 vòng) / (Số vi bước * 200 * 0.000005 * x)] => rps
    
    Ta có vận tốc V ở dạng rpm = rps / 60

    => V (rpm) = [1 / (Số vi bước * 200 * 0.000005 * x)] / 60
    => V = 60 / (Số vi bước * 200 * 0.000005 * x)
      
    Từ đó ta suy ra số đếm cần tìm:
      x = 60 / (V * Số vi bước * 200 * 0.000005)
  */
void TIM_STEP::ChayStep(int16_t TocDoRPM) {
  int16_t SoDem = 60 / (TocDoRPM * (CheDoViBuoc*200) * 0.000005);
  GiaTriDemCuaTimerBOT = abs(SoDem);
  GiaTriDemCuaTimerTOP = abs(SoDem) / 2;

/* KiemTraChieuQuay:
  < 0: CCW (Ngược chiều kim đồng hồ)
  > 0: CW (Cùng chiều kim đồng hồ)
  = 0: Stop (Không quay)
*/ 
  if (TocDoRPM < 0) {
    KiemTraChieuQuay = -1; // CCW
    digitalWrite(ChanDIR, HIGH); // CCW
  } 
  else if (TocDoRPM > 0) {
    KiemTraChieuQuay = 1; // CW
    digitalWrite(ChanDIR, LOW); // CW
  } 
  else KiemTraChieuQuay = 0;
}
