#ifndef MY_FAN_MOTOR_HPP
#define MY_FAN_MOTOR_HPP

#include <Arduino.h>








/*
    fanMotor : use timer1 PWM

    use motorDriver - L298N

*/
class FanMotor
{
public:
    FanMotor(uint8_t _enaPin, uint8_t _dir1Pin, uint8_t _dir2Pin);
    void init();
    void powerOn();
    void powerOff();
    void setFanSpeed(uint8_t _speed);

    bool isPowered() const;
    uint8_t getFanSpeed() const;
    
    
private:
    const uint8_t PIN_ENA;
    const uint8_t PIN_DIR1;
    const uint8_t PIN_DIR2;
    bool powerState = false;
    uint8_t fanSpeed = 0;
};






























FanMotor::FanMotor(uint8_t _enaPin, uint8_t _dir1Pin, uint8_t _dir2Pin)
    : PIN_ENA(_enaPin)
    , PIN_DIR1(_dir1Pin)
    , PIN_DIR2(_dir2Pin)
{
}


void FanMotor::init()
{
    pinMode(PIN_ENA, OUTPUT);
    pinMode(PIN_DIR1, OUTPUT);
    pinMode(PIN_DIR2, OUTPUT);

    analogWrite(PIN_ENA, fanSpeed);
    digitalWrite(PIN_DIR1, LOW);
    digitalWrite(PIN_DIR2, LOW);
}


inline void FanMotor::powerOn()
{
    if (!powerState)
    {
        digitalWrite(PIN_DIR1, HIGH);
        digitalWrite(PIN_DIR2, LOW);
        powerState = true;
    }
}


inline void FanMotor::powerOff()
{
    if (powerState)
    {
        digitalWrite(PIN_DIR1, LOW);
        digitalWrite(PIN_DIR2, LOW);
        powerState = false;
    }
}


inline void FanMotor::setFanSpeed(uint8_t _speed)
{
    if (fanSpeed != _speed)
    {
        analogWrite(PIN_ENA, _speed);
        fanSpeed = _speed;
    }
    else
    {
        // 같으면 analogWrite를 실행하지 않는다.
    }
}


inline bool FanMotor::isPowered() const
{
    return powerState;
}


inline uint8_t FanMotor::getFanSpeed() const
{
    return fanSpeed;
}







#endif // MY_FAN_MOTOR_HPP










