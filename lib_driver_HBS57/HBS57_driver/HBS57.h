#ifndef _HBS57_H_
#define _HBS57_H_

#include <Arduino.h>

class HBS57
{
    public:
        void KhoiTaoHBS57(uint8_t, uint8_t, uint8_t, int); // Chân DIR, Chân PUL, Chân EN, Số vi bước xung driver
        void ChayStep(int); // Tốc độ RPM (Chiều quay xác định bởi âm dương) (Để trong loop() thì chạy mãi, còn setup() thì chạy 1 vòng)
        void ChayStepVoiSoVongQuay(int, int); // Tốc độ RPM, Số vòng để quay (Để trong loop() thì phải có delay mới chạy đủ vòng rồi dừng rồi lại tiếp tục, còn setup() thì chạy đủ vòng là ngừng)
    private:
        uint8_t _ChanDir;
        uint8_t _ChanPul;
        uint8_t _ChanEn;

        int _TocDoStep = 0;
        int _SoXungTrenVong = 0;
        int _ThoiGianDelay = 0;
};
    
#endif