#include <Arduino.h>
#line 1 "C:\\Users\\ADMIN\\OneDrive - Unicorn\\Desktop\\HBS57_driver\\HBS57_driver.ino"
#define dirPin 13
#define pulPin 14
#define enPin 15

// hw_timer_t *My_timer = NULL;

// volatile int8_t DIR_motor; // Variable to indicate the motor's status and direction
// volatile uint16_t Count_TOP, Count_BOT;
// volatile int count;

// void IRAM_ATTR ISR(){
//   if(DIR_motor != 0) {
//     count++;
//     if(count <= Count_TOP) {
//       digitalWrite(pulPin, HIGH); // run
//     }
//     else digitalWrite(pulPin, LOW); // stop

//     if(count >= Count_BOT) { 
//       count = 0;
//     }
//   }
// }

// /* dirPin:
//   < 0: CCW
//   > 0: CW
//   = 0: Stop
// */ 
// void runMotor(int16_t rpm){
//   int16_t _counterNum = 60 / (rpm * 3200 * 0.000005);
//   Count_BOT = abs(_counterNum);
//   Count_TOP = abs(_counterNum) / 2;

//   if(rpm < 0) {
//     DIR_motor = -1; // CCW
//     digitalWrite(dirPin, HIGH); // CCW
//   }
//   else if(rpm > 0) {
//     DIR_motor = 1; // CW
//     digitalWrite(dirPin, LOW); // CW
//   }
//   else DIR_motor = 0;

// }

#line 47 "C:\\Users\\ADMIN\\OneDrive - Unicorn\\Desktop\\HBS57_driver\\HBS57_driver.ino"
void setup();
#line 64 "C:\\Users\\ADMIN\\OneDrive - Unicorn\\Desktop\\HBS57_driver\\HBS57_driver.ino"
void loop();
#line 47 "C:\\Users\\ADMIN\\OneDrive - Unicorn\\Desktop\\HBS57_driver\\HBS57_driver.ino"
void setup() {
  Serial.begin(115200);
  pinMode(dirPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  pinMode(enPin, OUTPUT);

//   My_timer = timerBegin(0, 80, true);
//   timerAttachInterrupt(My_timer, &ISR, true);
//   timerAlarmWrite(My_timer, 5, true);
//   timerAlarmEnable(My_timer);
}
// int temp = 0;
int rpm = 0;
int ppr = 0;
int delayInMicroseconds = 0;
bool checkFlag = false;

void loop() {
  digitalWrite(dirPin, LOW);

  while (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Đọc data cho đến khi enter
    int colonIndex = input.indexOf(':');

    if (colonIndex != -1) {
      // Trích xuất 1 phần của chuỗi đầu vào, từ đầu đến ":"
      String pprString = input.substring(0, colonIndex);
      // Từ ":" đến hết
      String rpmString = input.substring(colonIndex + 1);
      
      int newRPM = rpmString.toInt();
      int newPPR = pprString.toInt();

      if (newRPM != 0 || input.equals("0")) { // Kiểm tra nhập rpm = 0
        rpm = newRPM;
        ppr = newPPR;
        checkFlag = true;
        Serial.printf("Chạy với tốc độ %d RPM, với số xung %d PPR\n", rpm, ppr);
        delayInMicroseconds = 30000000 / (rpm * ppr);
      } 
      else Serial.println("Nhập lỗi, nhập lại!");
    }
    else Serial.println("Nhập lỗi, nhập lại!");
  }

  if(checkFlag){
    for (long i = 1; i <= ppr; i++) {
      digitalWrite(pulPin, HIGH);
      delayMicroseconds(delayInMicroseconds);
      digitalWrite(pulPin, LOW);
      delayMicroseconds(delayInMicroseconds);
    }
  }
}



