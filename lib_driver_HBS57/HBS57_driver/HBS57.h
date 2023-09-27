#ifndef _HBS57_H_
#define _HBS57_H_

#include <Arduino.h>

class HBS57
{
    public:
        void KhoiTaoHBS57(uint8_t, uint8_t, uint8_t); // Chân DIR, Chân PUL, Chân EN
        void ChayStep(int, bool, int); // Tốc độ RPM, Chiều quay (true, false), Số vi bước
        void ChayStepVoiSoVongQuay(int, bool, int, int); // Tốc độ RPM, Chiều quay (true, false), Số vi bước, Số vòng quay
    private:
        uint8_t _ChanDir;
        uint8_t _ChanPul;
        uint8_t _ChanEn;

        int _TocDoStep = 0;
        int _SoXungTrenVong = 0;
        int _ThoiGianDelay = 0;
        int _SoVongQuayStep = 0;
};
    
#endif