#include "Timer_HBS57.h"

HBS57* HBS57::BienBanSao = NULL;

int HBS57::KiemTraChieuQuay;
int HBS57::GiaTriDemCuaTimerTOP;
int HBS57::GiaTriDemCuaTimerBOT;
int HBS57::GiaTriDemCuaTimer;

void HBS57::BanSaoHamDemNgatTimer(){
    if(HBS57::BienBanSao != NULL){
        BienBanSao->DemNgatTimer();
    }
}

void IRAM_ATTR HBS57::DemNgatTimer(){
    if(BienBanSao != NULL){ // Kiểm tra biến bản sao có bị lỗi gì hay không
        if(KiemTraChieuQuay != 0){ // Kiểm tra biến chiều quay có bằng 0 hay không
            GiaTriDemCuaTimer++;
            if(GiaTriDemCuaTimer <= GiaTriDemCuaTimerTOP) digitalWrite(_ChanPul, HIGH); // Chạy step
            else digitalWrite(_ChanPul, LOW); // Dừng step
        
            if(GiaTriDemCuaTimer >= GiaTriDemCuaTimerBOT){ // Hết 1 xung
                GiaTriDemCuaTimer = 0;
            }
        }
    }
}

/***Khởi tạo Driver HBS57***/ 
void HBS57::KhoiTaoHBS57(uint8_t ChanDIR, uint8_t ChanPUL, uint8_t ChanEN, int ViBuocTrenDriver){
    _ChanDir = ChanDIR;
    _ChanPul = ChanPUL;
    _ChanEn = ChanEN;
    _SoViBuoc = ViBuocTrenDriver;

    pinMode(_ChanDir, OUTPUT);
    pinMode(_ChanPul, OUTPUT);
    pinMode(_ChanEn, OUTPUT);

    BienBanSao = this;

    BienTimer = timerBegin(0, 80, true); 
    timerAttachInterrupt(BienTimer, &HBS57::BanSaoHamDemNgatTimer, true);
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

/***Hàm chạy Step với Driver HBS57***/  
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
void HBS57::ChayStep(int TocDoRPM){
    int16_t SoDem = 60 / (TocDoRPM * _SoViBuoc * 0.000005); // Do driver HBS57 chỉ ghi số vi bước đã quy đổi, nên không cần nhân 200
    GiaTriDemCuaTimerBOT = abs(SoDem);
    GiaTriDemCuaTimerTOP = abs(SoDem) / 2;

    /* KiemTraChieuQuay:
    < 0: CCW (Ngược chiều kim đồng hồ)
    > 0: CW (Cùng chiều kim đồng hồ)
    = 0: Stop (Không quay)
    */ 
    if (TocDoRPM < 0) {
        KiemTraChieuQuay = -1; // CCW
        digitalWrite(_ChanDir, HIGH); // CCW
    } 
    else if (TocDoRPM > 0) {
        KiemTraChieuQuay = 1; // CW
        digitalWrite(_ChanDir, LOW); // CW
    } 
    else KiemTraChieuQuay = 0;
}