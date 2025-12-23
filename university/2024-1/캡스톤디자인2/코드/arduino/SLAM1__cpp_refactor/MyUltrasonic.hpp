#ifndef MY_ULTRASONIC_HPP
#define MY_ULTRASONIC_HPP

#include <Arduino.h>


#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif // MAX

#ifndef CLAMP
#define CLAMP(x, minValue, maxValue) ((x) < (minValue) ? (minValue) : ((x) > (maxValue) ? (maxValue) : (x)))
#endif // CLAMP




/*
    4개의 초음파 센서를 운용한다.

    trig pins count: 4, echo pins count: 2(external interrupt pins)

    2개의 trig 핀은 1개의 echo 핀을 공유한다.

    각 trig 핀을 하나씩 일정한 간격으로 트리거한다.

    각 센서의 최근 5개의 거리값을 저장하며, 저장된 배열의 주소를 받아 거리를 확인할 수 있다.

    default triggering, logging: false

    

    static constexpr int32_t INVALID_DISTANCE = -1L;

    echo 핀의 인터럽트에 대한 ISR은 init() 메서드에서 사용자에게 받는다.

    echo 핀의 ISR 설정
    ->
    attachInterrupt(digitalPinToInterrupt(echoPins[0]), echoISR1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(echoPins[1]), echoISR2, CHANGE);

    // 어떤 ISR을 사용할 것인지는 HW 핀 연결로 결정한다. (센서를 어떤 echo 핀에 꽂는지)
*/
class Ultrasonic
{
public:
    Ultrasonic(const uint8_t _trigPins[4], const uint8_t _echoPins[2], uint32_t& _clkSystemMicros, uint32_t _trigIntervalMicros);
    ~Ultrasonic();

    void init(void (*_echoISR1)(), void (*_echoISR2)());

    // polling, 각 trig 핀을 하나씩 일정한 간격으로 트리거한다.
    // 내부 index 값은 해당하는 센서의 trigger 이후 다음 trigger 전까지 유지되는 post-increment 방식이다.
    void trig();

    // mininum: 100000us
    void setTrigInterval(uint32_t _micros) { trigIntervalMicros = MAX(_micros, MIN_TRIG_INTERVAL_MICROS); }
    uint32_t getTrigInterval() const { return trigIntervalMicros; }

    void setTriggerSwitch(bool _on) { triggerEnabled = _on; }
    bool isTriggerOn() const { return triggerEnabled; }

    // If disabled, stores an invalid value (-1) instead.
    void setLogDistanceSwitch(bool _on) { logDistanceEnabled = _on; }
    bool isLogDistanceOn() const { return logDistanceEnabled; }

    void setNextTrigIndex(uint8_t _nextIndex);
    uint8_t getCurrentTrigIndex() const { return trigPinIndex; }

    const int32_t* getDistanceArray(uint8_t _trigPinIndex) const;

    // call in user echoISR1
    void calculateDistanceInEchoISR1();
    // call in user echoISR2
    void calculateDistanceInEchoISR2();


private:
    static uint8_t* copyAndReturnArray(const uint8_t* _src, size_t _count);


private:
    static constexpr uint8_t TRIG_PINS_COUNT = 4;
    static constexpr uint8_t ECHO_PINS_COUNT = 2;
    const uint8_t* const PIN_TRIG;
    const uint8_t* const PIN_ECHO;

    const uint32_t& clkSystemMicros;
    bool triggerEnabled = false;
    bool logDistanceEnabled = false;
    static constexpr uint32_t MIN_TRIG_INTERVAL_MICROS = 100000UL;
    uint32_t trigIntervalMicros;
    uint8_t trigPinIndex;

    static constexpr int32_t INVALID_DISTANCE = -1L;
    int32_t distance[4][5] = {
        { INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE },
        { INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE },
        { INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE },
        { INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE, INVALID_DISTANCE }
    };
};



































Ultrasonic::Ultrasonic(const uint8_t _trigPins[4], const uint8_t _echoPins[2], uint32_t& _systemClockMicros, uint32_t _trigIntervalMicros)
    : PIN_TRIG(copyAndReturnArray(_trigPins, TRIG_PINS_COUNT))
    , PIN_ECHO(copyAndReturnArray(_echoPins, ECHO_PINS_COUNT))
    , clkSystemMicros(_systemClockMicros)
    , trigIntervalMicros(MAX(_trigIntervalMicros, MIN_TRIG_INTERVAL_MICROS))
{
    setNextTrigIndex(0);
}


Ultrasonic::~Ultrasonic()
{
    delete[] PIN_TRIG;
    delete[] PIN_ECHO;
}


inline void Ultrasonic::init(void (*_echoISR1)(), void (*_echoISR2)())
{
    pinMode(PIN_TRIG[0], OUTPUT);
    pinMode(PIN_TRIG[1], OUTPUT);
    pinMode(PIN_TRIG[2], OUTPUT);
    pinMode(PIN_TRIG[3], OUTPUT);

    pinMode(PIN_ECHO[0], INPUT);
    pinMode(PIN_ECHO[1], INPUT);

    digitalWrite(PIN_TRIG[0], LOW);
    digitalWrite(PIN_TRIG[1], LOW);
    digitalWrite(PIN_TRIG[2], LOW);
    digitalWrite(PIN_TRIG[3], LOW);

    attachInterrupt(digitalPinToInterrupt(PIN_ECHO[0]), _echoISR1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ECHO[1]), _echoISR2, CHANGE);
}


inline void Ultrasonic::trig()
{
    static uint32_t clkLastTrig = clkSystemMicros;

    if ((triggerEnabled == true) && (clkSystemMicros > clkLastTrig + trigIntervalMicros))
    {
        clkLastTrig = clkSystemMicros;

        // trigPinIndex: [0, 3] 순환
        trigPinIndex = (trigPinIndex + 1) % TRIG_PINS_COUNT;

        digitalWrite(PIN_TRIG[trigPinIndex], HIGH);
        delayMicroseconds(10);
        digitalWrite(PIN_TRIG[trigPinIndex], LOW);
    }
}


inline void Ultrasonic::setNextTrigIndex(uint8_t _nextIndex)
{
    if (_nextIndex < TRIG_PINS_COUNT)
    {
        // trig() 메서드에서 1 증가하므로, 여기서 1 감소시킨다.
        trigPinIndex = (_nextIndex + TRIG_PINS_COUNT - 1) % TRIG_PINS_COUNT;
    }
}


inline const int32_t* Ultrasonic::getDistanceArray(uint8_t _trigPinIndex) const
{
    if (_trigPinIndex < TRIG_PINS_COUNT)
    {
        return &distance[_trigPinIndex][0];
    }
    else
    {
        // exception
        return nullptr;
    }
}


inline void Ultrasonic::calculateDistanceInEchoISR1()
{
    static uint32_t echoStart, echoEnd;
    
    if (digitalRead(PIN_ECHO[0]) == HIGH)
    {
        echoStart = clkSystemMicros;
    }
    else
    {
        echoEnd = clkSystemMicros;

        distance[trigPinIndex][4] = distance[trigPinIndex][3];
        distance[trigPinIndex][3] = distance[trigPinIndex][2];
        distance[trigPinIndex][2] = distance[trigPinIndex][1];
        distance[trigPinIndex][1] = distance[trigPinIndex][0];

        if (logDistanceEnabled)
            distance[trigPinIndex][0] = (echoEnd - echoStart) * 343 / 2000;
        else
            distance[trigPinIndex][0] = INVALID_DISTANCE;
    }
}


inline void Ultrasonic::calculateDistanceInEchoISR2()
{
    static uint32_t echoStart, echoEnd;
    
    if (digitalRead(PIN_ECHO[1]) == HIGH)
    {
        echoStart = clkSystemMicros;
    }
    else
    {
        echoEnd = clkSystemMicros;

        distance[trigPinIndex][4] = distance[trigPinIndex][3];
        distance[trigPinIndex][3] = distance[trigPinIndex][2];
        distance[trigPinIndex][2] = distance[trigPinIndex][1];
        distance[trigPinIndex][1] = distance[trigPinIndex][0];

        if (logDistanceEnabled)
            distance[trigPinIndex][0] = (echoEnd - echoStart) * 343 / 2000;
        else
            distance[trigPinIndex][0] = INVALID_DISTANCE;
    }
}


inline uint8_t* Ultrasonic::copyAndReturnArray(const uint8_t* _src, size_t _count)
{
    uint8_t* dest = new uint8_t[_count];
    memcpy(dest, _src, sizeof(uint8_t) * _count);
    return dest;
}














#endif // MY_ULTRASONIC_HPP











