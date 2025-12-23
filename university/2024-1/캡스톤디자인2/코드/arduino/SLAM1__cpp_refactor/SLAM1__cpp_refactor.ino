#include "SystemController.hpp"













// system main clock
uint32_t clkSystemMicros = 0;





constexpr uint8_t PIN_DIR1 = 21;
constexpr uint8_t PIN_STEP1 = 22;
constexpr uint8_t PIN_ENA1 = 23;
constexpr uint8_t TIMER_NUMBER1 = 3;
constexpr uint32_t TIMER_BITS1 = 16;
constexpr uint32_t PRESCALER1 = 8;
constexpr uint32_t MIN_STEP_DELAY = 1500;
constexpr uint32_t DEFAULT_STEP_DELAY = 5000;
MyStepper stepper1(PIN_ENA1, PIN_STEP1, PIN_DIR1, TIMER_NUMBER1, TIMER_BITS1, PRESCALER1, MIN_STEP_DELAY, DEFAULT_STEP_DELAY);

constexpr uint8_t PIN_DIR2 = 24;
constexpr uint8_t PIN_STEP2 = 25;
constexpr uint8_t PIN_ENA2 = 26;
constexpr uint8_t TIMER_NUMBER2 = 4;
constexpr uint32_t TIMER_BITS2 = 16;
constexpr uint32_t PRESCALER2 = 8;
MyStepper stepper2(PIN_ENA2, PIN_STEP2, PIN_DIR2, TIMER_NUMBER2, TIMER_BITS2, PRESCALER2, MIN_STEP_DELAY, DEFAULT_STEP_DELAY);






// { dr1, dr2, df1, df2 }
uint8_t PIN_TRIG[4] = { 4, 5, 6, 7 };
uint8_t PIN_ECHO[2] = { 2, 3 };
uint32_t TRIG_INTERVAL = 100000UL;
Ultrasonic ultrasonic(PIN_TRIG, PIN_ECHO, clkSystemMicros, TRIG_INTERVAL);

constexpr uint32_t MIN_TRIG_INTERVAL_MICROS = 100000UL;






SystemController systemController(stepper1, stepper2, ultrasonic, clkSystemMicros, PIN_ECHO);

















































void echoISR1();
void echoISR2();



void setup()
{
    // dbg
    Serial.begin(9600);
    

    systemController.initUltrasonic(echoISR1, echoISR2);
    systemController.initStepper1();
    systemController.initStepper2();


    // initial delay before mainLoop execution
    cli();
    delay(2000);
    sei();
}




void loop()
{
    clkSystemMicros = micros();

    systemController.mainLoop();
}

























































ISR(TIMER3_COMPA_vect)
{
    stepper1.triggerStepInISR();
    systemController.incrementStepCountInISR();
}


ISR(TIMER4_COMPA_vect)
{
    stepper2.triggerStepInISR();
}




// 초음파 우측 2센서
void echoISR1()
{
    systemController.filterRawDistanceInEchoISR1();
}


// 초음파 전방 2센서
void echoISR2()
{
    systemController.filterRawDistanceInEchoISR2();
}



// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline