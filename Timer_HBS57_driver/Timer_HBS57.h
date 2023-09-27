#ifndef _HBS57_H_
#define _HBS57_H_

#include <Arduino.h>

class HBS57
{
    public:
        void KhoiTaoHBS57(uint8_t, uint8_t, uint8_t, int); // Chân DIR, Chân PUL, Chân EN, Số vi bước
        void ChayStep(int); // Tốc độ RPM
    private:
        uint8_t _ChanDir;
        uint8_t _ChanPul;
        uint8_t _ChanEn;

        int _SoViBuoc = 0; // Số vi bước được ghi trên driver

        // Các biến dùng cho timer
        static int KiemTraChieuQuay;
        static int GiaTriDemCuaTimerTOP;
        static int GiaTriDemCuaTimerBOT;
        static int GiaTriDemCuaTimer;

        void DemNgatTimer();
        static void BanSaoHamDemNgatTimer();
        static HBS57* BienBanSao;
        
        hw_timer_t *BienTimer = NULL;
};
    
#endif