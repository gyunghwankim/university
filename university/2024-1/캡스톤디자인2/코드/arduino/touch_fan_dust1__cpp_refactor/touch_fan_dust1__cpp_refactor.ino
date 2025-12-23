#include "MyTouchSwitches.hpp"
#include "MyPMS7003.hpp"
#include "MyFanMotor.hpp"




#ifndef CLAMP
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#endif // CLAMP







constexpr uint8_t TOUCH_SWITCHES[4] = { 50, 51, 52, 53 };
TouchSwitch touchSwitches(TOUCH_SWITCHES, 4);


enum class FanMode : uint8_t {
    FAN_OFF = 0,
    FAN_LOW = 1,
    FAN_HIGH = 2,
    FAN_AUTO_FROM_PM_VALUES = 3
};

MyPMS7003 dustSensor(Serial1);

FanMotor fanMotor(11, 12, 13);

inline void applySwitchesToFanSpeed();









void setup()
{
    // dbg
    Serial.begin(9600);

    touchSwitches.init();
    fanMotor.init();
    dustSensor.begin(9600);

    fanMotor.powerOn();
}


void loop()
{
    dustSensor.readRawDataFromPMS7003();
    if (dustSensor.isRawDataReceived())
    {
        dustSensor.processRawData();
    }

    touchSwitches.updateActiveSwitchIndexFromSensor();

    applySwitchesToFanSpeed();
}





inline void applySwitchesToFanSpeed()
{
    static uint8_t lastSwitchStatus = static_cast<uint8_t>(FanMode::FAN_OFF);
    static FanMode fanMode = static_cast<FanMode>(lastSwitchStatus);

    uint8_t newSwitchStatus = touchSwitches.getActiveSwitchIndex();
    
    if (lastSwitchStatus != newSwitchStatus)
    {
        lastSwitchStatus = newSwitchStatus;
        fanMode = static_cast<FanMode>(newSwitchStatus);

        switch(fanMode)
        {
            case FanMode::FAN_OFF:
                fanMotor.setFanSpeed(0);
                break;
            case FanMode::FAN_LOW:
                fanMotor.setFanSpeed(150);
                break;
            case FanMode::FAN_HIGH:
                fanMotor.setFanSpeed(255);
                break;
            default:
                break;
        }
    }

    // setFanSpeed - analogWrite 소요 시간은 대략 100us, 먼지 센서 업데이트는 1초에 1회이므로, 아래의 case에서 runtime은 1/10000이다. 게다가 setFanSpeed() 메서드에서도 같은 값이면 실행하지 않는다.
    if (fanMode == FanMode::FAN_AUTO_FROM_PM_VALUES)
    {
        if (dustSensor.isProcessedDataUpdated())
        {
            dustSensor.clearUpdatedFlag();
            
            // PM 최소최대값 수정할 수 있도록 인터페이스 추가하자.@@@@@@@@@@@@@@@@@@@@
            static constexpr uint16_t PM10_MIN = 20, PM10_MAX = 50;
            static constexpr uint8_t FAN_SPEED_MIN = 150, FAN_SPEED_MAX = 255;
            static constexpr double RATIO = static_cast<double>(FAN_SPEED_MAX - FAN_SPEED_MIN) / static_cast<double>(PM10_MAX - PM10_MIN);
            
            // pm10 값을 받아오고 clamping
            uint16_t pm10 = CLAMP(dustSensor.getPM10(), PM10_MIN, PM10_MAX);

            // pm10값 범위를 fanSpeed값 범위로 mapping하여 fanSpeed를 결정한다.
            uint8_t fanSpeed = static_cast<uint8_t>(
                static_cast<double>(pm10 - PM10_MIN) * RATIO + static_cast<double>(FAN_SPEED_MIN));

            fanMotor.setFanSpeed(fanSpeed);
        }
    }
}

























// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline