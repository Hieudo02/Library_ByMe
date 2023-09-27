#include <Arduino.h>
#include "HBS57.h"

#define ChanDir   13
#define ChanPul   14
#define ChanEn    15
#define TocDoStep 100
#define HuongQuay true
#define ViBuoc    51200
#define SoVong    2

HBS57 myHBS57;

void setup() {
  Serial.begin(115200);
  myHBS57.KhoiTaoHBS57(ChanDir, ChanPul, ChanEn, ViBuoc);
}

void loop() {
  myHBS57.ChayStepVoiSoVongQuay(TocDoStep, SoVong);
  delay(1000);
}


