#define LED_BUILT_IN 2

hw_timer_t *My_timer = NULL; // A pointer variable (named My_timer of the type hw_timer_t) in order to configure the timer

void IRAM_ATTR onTimer(){
  digitalWrite(LED_BUILT_IN, !digitalRead(LED_BUILT_IN));
}

void setup() {
  pinMode(LED_BUILT_IN, OUTPUT);
  My_timer = timerBegin(0, 80, true); 
  /*
  0: number of the timer we want to use (from 0 to 3, since we have 4 hardware timers)
  80: value of the prescaler
  true: count up / false: count down
  */

  timerAttachInterrupt(My_timer, &onTimer, true);
  /* 
  Before enabling the timer, we need to attach it to an ISR, which will be executed when the interrupt is generated.
  In this example, we have attached the ISR function called "onTimer" to the timer interrupt.
  */

/*
ESP32: f = 80MHz
  prescaler: 80 -> f = 1MHz -> T = 1/1000000 = 1us
  count threshold = 1000000 -> 1000000*(1*10^-6) = 1s 
    => Timer count up to 1000000 takes 1 second
*/

  timerAlarmWrite(My_timer, 1000000, true);
  /* -> Specify the counter value
  true: counter will reload and thus the interrupt will be generated periodically
  */
  
  timerAlarmEnable(My_timer);
}


void loop() {
  // put your main code here, to run repeatedly:

}
