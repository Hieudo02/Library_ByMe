#ifndef _TIM_STEP_H_
#define _TIM_STEP_H_

#include <Arduino.h>

class TIM_STEP 
{
  public:
    void KhoiTaoStep(int, int, int, uint8_t); // ChanDIR, ChanPUL, ChanEN, CheDoViBuoc
    void ChayStep(int16_t); // Nhập vào tốc độ mong muốn (rpm): > 0 thì CW, < 0 thì CCW, = 0 thì không quay 

  private:
    int ChanDIR; // Chân DIR trên driver
    int ChanPUL; // Chân PUL trên driver
    int ChanEN; // Chân EN trên driver
    uint8_t CheDoViBuoc; // Mức chia vi bước trên driver (1, 2, 4, 8, 16, 32)
    
    static int KiemTraChieuQuay; 
    /* 
    Biến kiểm tra giá trị người dùng nhập.
    Nếu nhập RPM > 0 thì quay CW, RPM < 0 thì CCW, RPM = 0 thì không quay (còn tùy vào cách mắc dây để xác định đúng CW, CCW) 
    */  

    static int GiaTriDemCuaTimerTOP;
    static int GiaTriDemCuaTimerBOT;
    static int GiaTriDemCuaTimer;
    void DemNgatTimer();

    /*
    Hàm "DemNgatTimer" đang ở dạng Non-static function, mà hàm "timerAttachInterrupt" cần truyền vào 1 hàm static.
    Nên để gán được hàm "DemNgatTimer" trong hàm "timerAttachInterrupt", ta sẽ dùng 1 hàm bản sao (instance function) ở dạng static.
    Hàm bản sao này trỏ tới hàm mà ta cần dùng timer counter, cụ thể là "DemNgatTimer".
    Sau đó ta chỉ việc gán hàm bản sao vào hàm "timerAttachInterrupt" là dùng được.  
    */
    static void BanSaoHamDemNgatTimer(); 
    static TIM_STEP* BienBanSao; // Biến bản sao để dùng ngắt trong hàm "timerAttachInterrupt"

    hw_timer_t *BienTimer = NULL; // Biến dùng trong việc khởi tạo các hàm Timer
};

#endif
