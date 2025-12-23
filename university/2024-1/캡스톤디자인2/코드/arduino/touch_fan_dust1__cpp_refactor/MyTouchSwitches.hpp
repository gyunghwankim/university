#ifndef MY_TOUCH_SWITCHES_HPP
#define MY_TOUCH_SWITCHES_HPP

#include <Arduino.h>







/*
    ~~
*/
class TouchSwitch
{
public:
    TouchSwitch(const uint8_t _pinNumbers[], uint8_t _pinCount);
    ~TouchSwitch();

    void init();

    // return current switch number
    uint8_t getActiveSwitchIndex() const;

    void setActiveSwitchIndex(uint8_t _index);

    // polling
    void updateActiveSwitchIndexFromSensor();


private:
    static uint8_t* initPinArray(const uint8_t _pinNumbers[], uint8_t _pinCount);
    
    
private:
    uint8_t* SWITCH_PINS;
    const uint8_t SWITCH_COUNT;
    uint8_t activeSwitchIndex = 0;
};




























TouchSwitch::TouchSwitch(const uint8_t _pinNumbers[], uint8_t _pinCount)
    : SWITCH_PINS(initPinArray(_pinNumbers, _pinCount))
    , SWITCH_COUNT(_pinCount)
{
}


TouchSwitch::~TouchSwitch()
{
    delete[] SWITCH_PINS;
}


void TouchSwitch::init()
{
    for (uint8_t index = 0; index < SWITCH_COUNT; ++index)
    {
        pinMode(SWITCH_PINS[index], INPUT);
    }
}


inline uint8_t TouchSwitch::getActiveSwitchIndex() const
{
    return activeSwitchIndex;
}


inline void TouchSwitch::setActiveSwitchIndex(uint8_t _index)
{
    if (_index < SWITCH_COUNT)
    {
        activeSwitchIndex = _index;
    }
    else
    {
        // exception
    }
}


inline void TouchSwitch::updateActiveSwitchIndexFromSensor()
{
    for (uint8_t index = 0; index < SWITCH_COUNT; ++index)
    {
        if (digitalRead(SWITCH_PINS[index]) == HIGH)
        {
            activeSwitchIndex = index;
            break;
        }
    }
}


uint8_t* TouchSwitch::initPinArray(const uint8_t _pinNumbers[], uint8_t _pinCount)
{
    uint8_t* switchPins = new uint8_t[_pinCount];
    for (uint8_t index = 0; index < _pinCount; ++index)
    {
        switchPins[index] = _pinNumbers[index];
    }
    return switchPins;
}











#endif // MY_TOUCH_SWITCHES_HPP






