#include <Arduino.h>
#include "Timer_HBS57.h"

#define ChanDir   13
#define ChanPul   14
#define ChanEn    15
#define TocDoStep 60
#define ViBuoc    51200
#define SoVong    2

HBS57 myHBS57;

void setup() {
  Serial.begin(115200);
  myHBS57.KhoiTaoHBS57(ChanDir, ChanPul, ChanEn, ViBuoc);
}

void loop() {
  myHBS57.ChayStep(TocDoStep);
  // delay(1000);
}


