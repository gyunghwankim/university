/* 20231207.0245

<IRremote.h>, tone() 충돌.
->
<IRremote.h> timer 변경.



IRremote.h >v3.0.0
->
privateHeader: IRremoteBoardDefs.h
->
// Leonardo
#elif defined(__AVR_ATmega32U4__) && ! defined(TEENSYDUINO) && ! defined(ARDUINO_AVR_PROMICRO)
#  if !defined(IR_USE_TIMER1) && !defined(IR_USE_TIMER3) && !defined(IR_USE_TIMER4_HS)
//#define IR_USE_TIMER1     // tx = pin 9
#define IR_USE_TIMER3       // tx = pin 5
//#define IR_USE_TIMER4_HS  // tx = pin 13
#  endif
-> 변경
// Leonardo
#elif defined(__AVR_ATmega32U4__) && ! defined(TEENSYDUINO) && ! defined(ARDUINO_AVR_PROMICRO)
#  if !defined(IR_USE_TIMER1) && !defined(IR_USE_TIMER3) && !defined(IR_USE_TIMER4_HS)
#define IR_USE_TIMER1     // tx = pin 9
//#define IR_USE_TIMER3       // tx = pin 5
//#define IR_USE_TIMER4_HS  // tx = pin 13
#  endif
*/
















/* 20231216.0830
<SoftwareSerial.h>, <Servo.h> 충돌.
->
<SoftwareSerial.h>는 데이터 송수신 중에 모든 인터럽트를 중단한다.
<NeoSWSerial.h>는 TX에서 인터럽트가 비활성화된다.
->
<AltSoftSerial.h>로 변경.

<AltSoftSerial.h>, <Servo.h> 충돌.
->
<Servo.h> timer 변경.
->
arduino UNO : 8-bit timer(0, 2), 16-bit timer(1).
두 라이브러리 모두 16비트 타이머를 요구한다.
<Servo.h> -> <ServoTimer2.h> 변경
*/











/*
  꿀팁:

  1.
  핀마다 내부 풀업 저항이 있다.
  pinMode(pinNumber, INPUT_PULLUP);
*/