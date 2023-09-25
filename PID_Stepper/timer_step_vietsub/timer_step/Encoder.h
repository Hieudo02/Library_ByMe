#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "Arduino.h"

class ENCODER
{
  public:
    ENCODER(int, int); // Khởi tạo encoder (Chân A, Chân B)
    void DatCheDoDocEncoder(int); // Có các chế độ: x1, x2, x4
    int DocXungEncoder();

  private:
    void DocEncoderX1();
    void DocEncoderX2();
    void DocEncoderX4();
    static ENCODER * BanSao[3];
    static void BanSaoDocEncoderX1();
    static void BanSaoDocEncoderX2();
    static void BanSaoDocEncoderX4();

    int ChanEncoderA;
    int ChanEncoderB;
    int TrangThaiChanEncoderA;
    int TrangThaiChanEncoderB;

    // Used in x4
    int TrangThaiHienTaiEncoderX4;
    int TrangThaiTruocCuaEncoderX4;

    static int XungEncoder; // Pulse count
    
};

#endif