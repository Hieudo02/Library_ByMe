#include "HBS57.h"

/*Khởi tạo Driver HBS57*/ 
void HBS57::KhoiTaoHBS57(uint8_t ChanDIR, uint8_t ChanPUL, uint8_t ChanEN){
    _ChanDir = ChanDIR;
    _ChanPul = ChanPUL;
    _ChanEn = ChanEN;

    pinMode(_ChanDir, OUTPUT);
    pinMode(_ChanPul, OUTPUT);
    pinMode(_ChanEn, OUTPUT);
}

/*
Nếu các chân PUL+, DIR+, EN+ nối lên 5V và các chân (-) nối vào VĐK, 
thì mức LOW:
    DIR cùng chiều đồng hồ
    EN được kích để khóa cứng động cơ

Nếu các chân PUL-, DIR-, EN- nối lên xuống Ground (0V) và các chân (+) nối vào VĐK, 
thì mức HIGH:
    DIR cùng chiều đồng hồ
    EN được kích để khóa cứng động cơ

Tuy nhiên, do driver chỉnh được chiều quay nên việc setup này không ảnh hưởng gì đến điều khiển
*/

/*Hàm chạy Step với Driver HBS57*/ 
void HBS57::ChayStep(int TocDoRPM, bool ChieuQuay, int XungDriver){
    if(ChieuQuay == true) digitalWrite(_ChanDir, LOW); // Quay cùng chiều đồng hồ
    else if(ChieuQuay == false) digitalWrite(_ChanDir, HIGH); // Quay ngược chiều đồng hồ

    _TocDoStep = TocDoRPM;
    _SoXungTrenVong = XungDriver;
    _ThoiGianDelay = 30000000 / (_TocDoStep * _SoXungTrenVong);

    for (long i = 1; i <= _SoXungTrenVong; i++) {
      digitalWrite(_ChanPul, HIGH);
      delayMicroseconds(_ThoiGianDelay);
      digitalWrite(_ChanPul, LOW);
      delayMicroseconds(_ThoiGianDelay);
    }
}