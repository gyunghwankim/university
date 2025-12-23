#ifndef MY_STEPPER_HPP
#define MY_STEPPER_HPP

#include <Arduino.h>


#ifndef F_CPU
#define F_CPU 16000000L            // cpu_clk: 16MHz
#endif // F_CPU

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif // ABS

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif // MAX

#ifndef CLAMP
#define CLAMP(x, minValue, maxValue) ((x) < (minValue) ? (minValue) : ((x) > (maxValue) ? (maxValue) : (x)))
#endif // CLAMP





/*
    4-phase stepper motor with stepper driver

    use direct pin, no UART

    use 16-bit timer

    모든 시간 단위는 us이다.

    타이머 설정 조건
    ->
    1. arduino MEGA 2560
    1. 16-bit timer - timer[1, 3, 4, 5]

    baseSpeed, speedRatio 설정으로 stepDelay를 결정한다.



    스텝딜레이 계산 공식  :  stepDelay = DEFAULT_STEP_DELAY * (1 / (baseSpeed * speedRatio))

    DEFAULT_STEP_DELAY  :  HW, 모터에 따른 상수값. 스테퍼 속도와 비율이 1일 때 stepDelay(us) = DEFAULT_STEP_DELAY(us)

    baseSpeed  :  모든 상황에 적용되는 주행 속도

    speedRatio  :  직진/좌회전/우회전에서 양 바퀴 제어에 사용할 속도
*/
class MyStepper
{
public:
    MyStepper(uint8_t _PIN_ENA, uint8_t _PIN_STEP, uint8_t _PIN_DIR
    , uint8_t TIMER_NUMBER, uint32_t _TIMER_BITS, uint32_t _PRESCALER, uint32_t _MIN_STEP_DELAY, uint32_t _DEFAULT_STEP_DELAY);

    void init(double _baseSpeed);

    uint32_t getMaxOCR() const { return MAX_OCR; }
    double getTimerResolution() const { return TIMER_RESOLUTION; }
    uint32_t getMaxStepDelay() const { return MAX_STEP_DELAY; }

    void stop();
    void start();
    bool isStepperActive() const { return stepperActive; }

    // range: [0, inf]
    void setBaseSpeed(double _speed);

    // range: [0.001, 1]
    void setSpeedRatio(double _ratio);

    // forward-HIGH, reverse-LOW
    void setDirection(bool _newDir);

    // call in timer ISR(TIMERn_COMPA_vect) to excute one step
    void triggerStepInISR();

    /*
        비트플래그, 비트필드로 구현해보자.

        1. 설정한 최소 스텝딜레이에 도달
        2. 계산한 최대 스텝딜레이에 도달
        3. ...
    */
    // bool getErrorFlag() const { return error; }


private:
    void initTimer();
    void updateStepDelay();


// private:
// dbg__
public:
// __dbg
    const uint8_t PIN_ENA;
    const uint8_t PIN_STEP;
    const uint8_t PIN_DIR;

    const uint8_t TIMER_NUMBER;

    const uint32_t TIMER_BITS;
    const uint32_t MAX_OCR;
    static uint32_t calculateMaxOCRnA(uint32_t _TIMER_BITS)
    { return (1UL << _TIMER_BITS) - 1UL; }

    const uint32_t PRESCALER;
    const double TIMER_RESOLUTION;
    static double calculateTimerResolution(uint16_t _PRESCALER, uint32_t _CPU_FREQUENCY)
    { return 1000000.0 * static_cast<double>(_PRESCALER) / static_cast<double>(_CPU_FREQUENCY); }

    const uint32_t MAX_STEP_DELAY;
    static uint32_t calculateMaxStepDelay(uint32_t _MAX_OCR, double _TIMER_RESOLUTION)
    { return static_cast<uint32_t>((static_cast<double>(_MAX_OCR) + 1.0) * _TIMER_RESOLUTION); }
    const uint32_t MIN_STEP_DELAY;

    const uint32_t DEFAULT_STEP_DELAY;

    uint32_t ocrValue;
    uint32_t stepDelay;

    double baseSpeed = 1.0;
    static constexpr double MIN_RATIO = 0.001; // stepDelay 계산에서 overflow를 방지한다.
    static constexpr double MAX_RATIO = 1.0;
    double speedRatio = MAX_RATIO;

    bool stepperActive = false;

    double inputBaseSpeed = baseSpeed;
    double inputSpeedRatio = speedRatio;
    double inputDirection = HIGH;

    // bool error = false;
};

















MyStepper::MyStepper(uint8_t _PIN_ENA, uint8_t _PIN_STEP, uint8_t _PIN_DIR
    , uint8_t _TIMER_NUMBER, uint32_t _TIMER_BITS, uint32_t _PRESCALER, uint32_t _MIN_STEP_DELAY, uint32_t _DEFAULT_STEP_DELAY)
    : PIN_ENA(_PIN_ENA)
    , PIN_STEP(_PIN_STEP)
    , PIN_DIR(_PIN_DIR)
    , TIMER_NUMBER(_TIMER_NUMBER)
    , TIMER_BITS(_TIMER_BITS)
    , MAX_OCR(calculateMaxOCRnA(TIMER_BITS))
    , PRESCALER(_PRESCALER)
    , TIMER_RESOLUTION(calculateTimerResolution(PRESCALER, F_CPU))
    , MAX_STEP_DELAY(calculateMaxStepDelay(MAX_OCR, TIMER_RESOLUTION))
    , MIN_STEP_DELAY(_MIN_STEP_DELAY)
    , DEFAULT_STEP_DELAY(_DEFAULT_STEP_DELAY)
    , ocrValue(MAX_OCR)
    , stepDelay(MAX_STEP_DELAY)
{
}


inline void MyStepper::init(double _baseSpeed)
{
    pinMode(PIN_ENA, OUTPUT);
    pinMode(PIN_STEP, OUTPUT);
    pinMode(PIN_DIR, OUTPUT);

    digitalWrite(PIN_ENA, LOW);
    digitalWrite(PIN_STEP, LOW);
    digitalWrite(PIN_DIR, LOW);

    setBaseSpeed(_baseSpeed);
    updateStepDelay(); // baseSpeed가 초기값과 같을 때 자동 업데이트가 누락될 수 있으므로, 한 번 명시적으로 실행한다.

    initTimer();
}


inline void MyStepper::stop()
{
    if (stepperActive)
    {
        stepperActive = false;

        // disable timer interrupts
        switch (TIMER_NUMBER)
        {
            case 1: TIMSK1 &= ~(1 << OCIE1A); break;
            case 3: TIMSK3 &= ~(1 << OCIE3A); break;
            case 4: TIMSK4 &= ~(1 << OCIE4A); break;
            case 5: TIMSK5 &= ~(1 << OCIE5A); break;
            default: break;
        }

        // disable steppers
        digitalWrite(PIN_ENA, HIGH);
    }
}


inline void MyStepper::start()
{
    if (!stepperActive)
    {
        stepperActive = true;

        // enable timer interrupts
        switch (TIMER_NUMBER)
        {
            case 1: TIMSK1 |= (1 << OCIE1A); break;
            case 3: TIMSK3 |= (1 << OCIE3A); break;
            case 4: TIMSK4 |= (1 << OCIE4A); break;
            case 5: TIMSK5 |= (1 << OCIE5A); break;
            default: break;
        }

        // enable steppers
        digitalWrite(PIN_ENA, LOW);
    }
}


inline void MyStepper::setBaseSpeed(double _newSpeed)
{
    /*
        입력값을 비교하고, 이전과 다르면 적용한다.
    */
    if (inputBaseSpeed != _newSpeed)
    {
        inputBaseSpeed = _newSpeed;
        baseSpeed = MAX(0., _newSpeed);
        updateStepDelay();
    }
}


inline void MyStepper::setSpeedRatio(double _newRatio)
{
    /*
        입력값을 비교하고, 이전과 다르면 적용한다.
    */
    if (inputSpeedRatio != _newRatio)
    {
        inputSpeedRatio = _newRatio;
        speedRatio = CLAMP(_newRatio, MIN_RATIO, MAX_RATIO);
        updateStepDelay();
    }
}


inline void MyStepper::setDirection(bool _newDir)
{
    /*
        입력값을 비교하고, 이전과 다르면 적용한다.
    */
    if (inputDirection != _newDir)
    {
        digitalWrite(PIN_DIR, !_newDir); // direction pin: forward(LOW), reverse(HIGH)
        inputDirection = _newDir;
    }
}


inline void MyStepper::triggerStepInISR()
{
    digitalWrite(PIN_STEP, HIGH);
    digitalWrite(PIN_STEP, LOW);
}


inline void MyStepper::initTimer()
{
    // timer interrupt는 이후 start() 메서드로 활성화한다.
    switch (TIMER_NUMBER)
    {
        case 1:
            TIMSK1 &= ~(1 << OCIE1A);
            TCCR1A = 0;
            TCCR1B = 0;
            TCNT1 = 0;
            OCR1A = ocrValue;
            TCCR1B |= (1 << WGM12);

            switch (PRESCALER)
            {
                case 1: TCCR1B |= (1 << CS10); break;
                case 8: TCCR1B |= (1 << CS11); break;
                case 64: TCCR1B |= (1 << CS11) | (1 << CS10); break;
                case 256: TCCR1B |= (1 << CS12); break;
                case 1024: TCCR1B |= (1 << CS12) | (1 << CS10); break;
                default: break; // exception - invalid prescaler value
            }
            break;

        case 3:
            TIMSK3 &= ~(1 << OCIE3A);
            TCCR3A = 0;
            TCCR3B = 0;
            TCNT3 = 0;
            OCR3A = ocrValue;
            TCCR3B |= (1 << WGM32);

            switch (PRESCALER)
            {
                case 1: TCCR3B |= (1 << CS30); break;
                case 8: TCCR3B |= (1 << CS31); break;
                case 64: TCCR3B |= (1 << CS31) | (1 << CS30); break;
                case 256: TCCR3B |= (1 << CS32); break;
                case 1024: TCCR3B |= (1 << CS32) | (1 << CS30); break;
                default: break; // exception
            }
            break;

        case 4:
            TIMSK4 &= ~(1 << OCIE4A);
            TCCR4A = 0;
            TCCR4B = 0;
            TCNT4 = 0;
            OCR4A = ocrValue;
            TCCR4B |= (1 << WGM42);

            switch (PRESCALER)
            {
                case 1: TCCR4B |= (1 << CS40); break;
                case 8: TCCR4B |= (1 << CS41); break;
                case 64: TCCR4B |= (1 << CS41) | (1 << CS40); break;
                case 256: TCCR4B |= (1 << CS42); break;
                case 1024: TCCR4B |= (1 << CS42) | (1 << CS40); break;
                default: break; // exception
            }
            break;

        case 5:
            TIMSK5 &= ~(1 << OCIE5A);
            TCCR5A = 0;
            TCCR5B = 0;
            TCNT5 = 0;
            OCR5A = ocrValue;
            TCCR5B |= (1 << WGM52);

            switch (PRESCALER)
            {
                case 1: TCCR5B |= (1 << CS50); break;
                case 8: TCCR5B |= (1 << CS51); break;
                case 64: TCCR5B |= (1 << CS51) | (1 << CS50); break;
                case 256: TCCR5B |= (1 << CS52); break;
                case 1024: TCCR5B |= (1 << CS52) | (1 << CS50); break;
                default: break; // exception
            }
            break;

        default:
            break; // exception - invalid timer number
    }
}


inline void MyStepper::updateStepDelay()
{
    // exception: dividing 0
    if (baseSpeed == 0)
        stepDelay = MAX_STEP_DELAY;
    else
        stepDelay = static_cast<uint32_t>(static_cast<double>(DEFAULT_STEP_DELAY) * (1.0 / (ABS(baseSpeed) * speedRatio)));

    stepDelay = CLAMP(stepDelay, MIN_STEP_DELAY, MAX_STEP_DELAY);

    ocrValue = static_cast<uint16_t>((static_cast<double>(stepDelay) / TIMER_RESOLUTION) - 1.0);
    
    switch (TIMER_NUMBER)
    {
        case 1: OCR1A = ocrValue; break;
        case 3: OCR3A = ocrValue; break;
        case 4: OCR4A = ocrValue; break;
        case 5: OCR5A = ocrValue; break;
        default: break;
    }
}

















#endif // MY_STEPPER_HPP















