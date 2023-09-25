#include "Arduino.h"
#include "Encoder.h"

ENCODER* ENCODER::BanSao[3] = {NULL, NULL, NULL};
int ENCODER::XungEncoder = 0; 

/*----------Bản sao của mỗi hàm đọc encoder----------*/
void ENCODER::BanSaoDocEncoderX1()
{
  if (ENCODER::BanSao[0] != NULL)
  BanSao[0]->DocEncoderX1();
}

void ENCODER::BanSaoDocEncoderX2()
{
  if (ENCODER::BanSao[1] != NULL)
  BanSao[1]->DocEncoderX2();
}

void ENCODER::BanSaoDocEncoderX4()
{
  if (ENCODER::BanSao[2] != NULL)
  BanSao[2]->DocEncoderX4();
}
/*--------------------------------------------*/

/*----------------Khởi tạo encoder----------------*/
ENCODER::ENCODER(int _ChanEncoderA, int _ChanEncoderB)
{
  ChanEncoderA = _ChanEncoderA;
  ChanEncoderB = _ChanEncoderB;
  pinMode(ChanEncoderA, INPUT_PULLUP);
  pinMode(ChanEncoderB, INPUT_PULLUP);
}
/*--------------------------------------------*/

/*--------------------Xét từng chế độ đọc Encoder--------------------*/
void ENCODER::DatCheDoDocEncoder(int _CheDoDoc){
  switch(_CheDoDoc){
    case 1:
      BanSao[0] = this;
      attachInterrupt(digitalPinToInterrupt(ChanEncoderA), BanSaoDocEncoderX1, FALLING);
    break;

    case 2:
      BanSao[1] = this;
      attachInterrupt(digitalPinToInterrupt(ChanEncoderA), BanSaoDocEncoderX2, CHANGE);
    break;

    case 4:
      BanSao[2] = this;

      TrangThaiTruocCuaEncoderX4 = (TrangThaiTruocCuaEncoderX4 << 1) | digitalRead(ChanEncoderA);
      TrangThaiTruocCuaEncoderX4 = (TrangThaiTruocCuaEncoderX4 << 1) | digitalRead(ChanEncoderB);
      TrangThaiTruocCuaEncoderX4 = TrangThaiTruocCuaEncoderX4 & 0x03;

      attachInterrupt(digitalPinToInterrupt(ChanEncoderA), BanSaoDocEncoderX4, CHANGE);
      attachInterrupt(digitalPinToInterrupt(ChanEncoderB), BanSaoDocEncoderX4, CHANGE);
    break;
  }
}
/*----------------------------------------------------------------------------*/

/*--------------------Đọc Encoder x1--------------------*/
void IRAM_ATTR ENCODER::DocEncoderX1()
{
  TrangThaiChanEncoderA = digitalRead(ChanEncoderA);
  TrangThaiChanEncoderB = digitalRead(ChanEncoderB);

  if((TrangThaiChanEncoderA == LOW) && (TrangThaiChanEncoderB == HIGH)){
    XungEncoder++; // CW
  } 
  else if((TrangThaiChanEncoderA == LOW) && (TrangThaiChanEncoderB == LOW)){
    XungEncoder--; // CCW
  } 
}
/*-----------------------------------------------------*/

/*--------------------Đọc Encoder x2--------------------*/
void IRAM_ATTR ENCODER::DocEncoderX2()
{
  TrangThaiChanEncoderA = digitalRead(ChanEncoderA);
  TrangThaiChanEncoderB = digitalRead(ChanEncoderB);

  if
  (
    ((TrangThaiChanEncoderA == 0) && (TrangThaiChanEncoderB == 1)) | 
    ((TrangThaiChanEncoderA == 1) && (TrangThaiChanEncoderB == 0))
  ) XungEncoder += 1; 
  else if
  ( 
    ((TrangThaiChanEncoderA == 0) && (TrangThaiChanEncoderB == 0)) | 
    ((TrangThaiChanEncoderA == 1) && (TrangThaiChanEncoderB == 1))
  ) XungEncoder -= 1;
}
/*------------------------------------------------------*/

/*-------------------------------Đọc Encoder x4-------------------------------*/
void IRAM_ATTR ENCODER::DocEncoderX4()
{
  TrangThaiChanEncoderA = digitalRead(ChanEncoderA);
  TrangThaiChanEncoderB = digitalRead(ChanEncoderB);

  // Xác định chân đọc ngắt
  int ChanDocNgat = digitalPinToInterrupt(TrangThaiChanEncoderA) == digitalPinToInterrupt(ChanEncoderA) ? ChanEncoderA : ChanEncoderB;
  TrangThaiHienTaiEncoderX4 = (TrangThaiHienTaiEncoderX4 << 1) | TrangThaiChanEncoderA;
  TrangThaiHienTaiEncoderX4 = (TrangThaiHienTaiEncoderX4 << 1) | TrangThaiChanEncoderB;
  TrangThaiHienTaiEncoderX4 = TrangThaiHienTaiEncoderX4&0x03; // giữ cho trạng thái encoder từ 0->3
  // Ngắt kênh A
  if (ChanDocNgat == ChanEncoderA) {
    switch(TrangThaiHienTaiEncoderX4){
      case 2:
        if (TrangThaiTruocCuaEncoderX4 == 0) XungEncoder++;
        else XungEncoder--;
      break;
      case 1:
        if (TrangThaiTruocCuaEncoderX4 == 3) XungEncoder++;
        else XungEncoder--;
      break;
      case 0:
        if (TrangThaiTruocCuaEncoderX4 == 2) XungEncoder--;
        else XungEncoder++;
      break;
      case 3:
        if (TrangThaiTruocCuaEncoderX4 == 1) XungEncoder--;
        else XungEncoder++;
      break;
    }
    TrangThaiTruocCuaEncoderX4 = TrangThaiHienTaiEncoderX4;
  }

  // Ngắt kênh B
  else if(ChanDocNgat == ChanEncoderB){
    switch(TrangThaiHienTaiEncoderX4){
      case 3:
        if (TrangThaiTruocCuaEncoderX4 == 2) XungEncoder++;
        else XungEncoder--;
      break;
      case 0:
        if (TrangThaiTruocCuaEncoderX4 == 1) XungEncoder++;
        else XungEncoder--;
      break;
      case 1:
        if (TrangThaiTruocCuaEncoderX4 == 0) XungEncoder--;
        else XungEncoder++;
      break;
      case 2:
        if (TrangThaiTruocCuaEncoderX4 == 3) XungEncoder--;
        else XungEncoder++;
      break;
    }
    TrangThaiTruocCuaEncoderX4 = TrangThaiHienTaiEncoderX4;
  }
}
/*----------------------------------------------------------------------------*/

/*-----------Đọc xung Encoder-----------*/
int ENCODER::DocXungEncoder() {
  int BienLuuXungEncoder = XungEncoder;
  XungEncoder = 0; 
  return BienLuuXungEncoder;
}
/*--------------------------------------------*/