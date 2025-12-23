#ifndef SYSTEM_CONTROLLER_HPP
#define SYSTEM_CONTROLLER_HPP


#include "MyStepper.hpp"
#include "MyUltrasonic.hpp"

#include <Arduino.h>





class SystemController
{
public:
    SystemController(MyStepper& _stepper1, MyStepper& _stepper2, Ultrasonic& _ultrasonic, uint32_t& _systemClockMicros, const uint8_t _echoPins[2]);
    ~SystemController();

    void initUltrasonic(void (*_echoISR1)(), void (*_echoISR2)());
    void initStepper1();
    void initStepper2();
    void mainLoop();
    void incrementStepCountInISR() { ++stepCount; }
    void filterRawDistanceInEchoISR1();
    void filterRawDistanceInEchoISR2();


private:
    // 주행 함수는 두 스테퍼의 속도만 변경한다.
    void moveStraight();
    void turnRight();
    void turnLeft();

    bool canMoveStraight();
    bool canTurnRight();
    bool canTurnLeft();

    bool isTurnEnd();
    void processTurnEnd();
    
    void resetStepCount() { stepCount = 0; }
    int32_t getStepCount() const { return stepCount; }

    static uint8_t* copyAndReturnArray(const uint8_t* _src, size_t _count);
    inline bool isValid(int32_t _distance) { return _distance != INVALID_DISTANCE; }


private:
    MyStepper& stepper1;
    MyStepper& stepper2;
    Ultrasonic& ultrasonic;

    const uint32_t& clkSystemMicros;

    static constexpr uint8_t ECHO_PINS_COUNT = 2;
    const uint8_t* const PIN_ECHO;

    // 각 초음파 센서의 거리값 배열
    const int32_t* const dr1, * const dr2, * const df1, * const df2;

    static constexpr int32_t INVALID_DISTANCE = -1L;

    // 주행을 위한 거리 필터값
    volatile int32_t DR1 = INVALID_DISTANCE
        , DR2 = INVALID_DISTANCE
        , DF1 = INVALID_DISTANCE
        , DF2 = INVALID_DISTANCE;

    volatile bool isRightSensorsUpdated = false;
    volatile bool isFrontSensorsUpdated = false;

    // 우회전, 좌회전 임계값
    static constexpr int32_t RIGHT_TURN_THRESHOLD = 150;
    static constexpr int32_t LEFT_TURN_THRESHOLD = 250;

    // 센서의 노이즈 임계값. 센서별로 현재/이전 거리값을 비교하고, 임계값 이하이면 필터값으로 저장한다.
    // 우측 센서: 최대 기울기에서의 현재/이전 거리값의 차이보다 커야 한다.
    // 전방 센서: 현재 속도에서의 현재/이전 거리값의 차이보다 커야 한다.
    static constexpr int32_t RIGHT_SENSORS_NOISE_THRESHOLD = 50;
    static constexpr int32_t FRONT_SENSORS_NOISE_THRESHOLD = 100;



    volatile int32_t stepCount = 0;

    bool isTurning = false;

    static constexpr int32_t STEPS_FOR_TURN_COOLDOWN = 500;
    bool isRightTurnAllowed = false;

    int32_t savedStepCountBeforeRightTurnStart;
    static constexpr int32_t STEPS_UNTIL_RIGHT_TURN = 950;

    int32_t savedStepCountAtTurnStart;

    static constexpr int32_t STEPS_FOR_TURN_DURATION = 1550;

    static constexpr double BASE_SPEED = 3.0;
};



















































SystemController::SystemController(MyStepper& _stepper1, MyStepper& _stepper2, Ultrasonic& _ultrasonic, uint32_t& _systemClockMicros, const uint8_t _echoPins[2])
    : stepper1(_stepper1)
    , stepper2(_stepper2)
    , ultrasonic(_ultrasonic)
    , clkSystemMicros(_systemClockMicros)
    , PIN_ECHO(copyAndReturnArray(_echoPins, ECHO_PINS_COUNT))
    , dr1(ultrasonic.getDistanceArray(0))
    , dr2(ultrasonic.getDistanceArray(1))
    , df1(ultrasonic.getDistanceArray(2))
    , df2(ultrasonic.getDistanceArray(3))
{
}


SystemController::~SystemController()
{
    delete[] PIN_ECHO;
}


inline void SystemController::initUltrasonic(void (*_echoISR1)(), void (*_echoISR2)())
{
    // ultrasonic
    ultrasonic.init(_echoISR1, _echoISR2);
    ultrasonic.setTriggerSwitch(true);
    ultrasonic.setLogDistanceSwitch(true);
}


inline void SystemController::initStepper1()
{
    // stepper1
    stepper1.init(BASE_SPEED);
    stepper1.start();
    stepper1.setSpeedRatio(1.0);
}


inline void SystemController::initStepper2()
{
    // stepper2
    stepper2.init(BASE_SPEED);
    stepper2.start();
    stepper2.setSpeedRatio(1.0);
}


inline void SystemController::mainLoop()
{
    ultrasonic.trig();

    if (!isTurning)
    {
        if (isRightSensorsUpdated)
        {
            isRightSensorsUpdated = false;

            if (canMoveStraight())
            {
                moveStraight();
            }
            else if (canTurnRight())
            {
                turnRight();
            }
        }


        if (isFrontSensorsUpdated)
        {
            isFrontSensorsUpdated = false;

            if (canTurnLeft())
            {
                turnLeft();
            }
        }
    }
    else // !isTurning
    {
        // 회전 중
        if (isTurnEnd())
        {
            isTurning = false; // 회전 끝

            processTurnEnd();


        }
    }
}


inline void SystemController::filterRawDistanceInEchoISR1()
{
    ultrasonic.calculateDistanceInEchoISR1();

    static uint8_t currTrigIndex;
    currTrigIndex = ultrasonic.getCurrentTrigIndex();

    // echo start
    if (digitalRead(PIN_ECHO[0]) == HIGH)
    {

    }
    // echo end
    else
    {
        switch (currTrigIndex)
        {
            // dr1
            case 0:
                if (isValid(dr1[0]) && isValid(dr1[1]))
                {
                    // noise filtering
                    if (dr1[0] < DR1 + RIGHT_TURN_THRESHOLD)
                    {
                        if (-RIGHT_SENSORS_NOISE_THRESHOLD < (dr1[0] - dr1[1]) && (dr1[0] - dr1[1]) < RIGHT_SENSORS_NOISE_THRESHOLD)
                        {
                            DR1 = dr1[0];
                        }
                    }
                }
                break;

            // dr2
            case 1:
                if (isValid(dr2[0]) && isValid(dr2[1]))
                {
                    // noise filtering
                    if (dr2[0] < DR2 + RIGHT_TURN_THRESHOLD)
                    {
                        if (-RIGHT_SENSORS_NOISE_THRESHOLD < (dr2[0] - dr2[1]) && (dr2[0] - dr2[1]) < RIGHT_SENSORS_NOISE_THRESHOLD)
                        {
                            DR2 = dr2[0];
                        }
                    }
                    isRightSensorsUpdated = true;
                }
                break;
        }
    }
}


inline void SystemController::filterRawDistanceInEchoISR2()
{
    ultrasonic.calculateDistanceInEchoISR2();

    static uint8_t currTrigIndex;
    currTrigIndex = ultrasonic.getCurrentTrigIndex();

    // echo start
    if (digitalRead(PIN_ECHO[1]) == HIGH)
    {

    }
    // echo end
    else
    {
        switch (currTrigIndex)
        {
            // df1
            case 2:
                /*
                    현재 전방 필터값을 사용하지 않음.
                */
                // if (isValid(df1[0]) && isValid(df1[1]))
                // {
                //     if (df1[0] < DF1 + FRONT_SENSORS_NOISE_THRESHOLD)
                //     {
                //         if (-FRONT_SENSORS_NOISE_THRESHOLD < (df1[0] - df1[1]) && (df1[0] - df1[1]) < FRONT_SENSORS_NOISE_THRESHOLD)
                //         {
                //             DF1 = df1[0];
                //         }
                //     }
                // }
                break;
                
            // df2
            case 3:
                /*
                    현재 전방 필터값을 사용하지 않음.
                */
                // if (isValid(df2[0]) && isValid(df2[1]))
                // {
                //     // noise filtering
                //     if (df2[0] < DF2 + FRONT_SENSORS_NOISE_THRESHOLD)
                //     {
                //         if (-FRONT_SENSORS_NOISE_THRESHOLD < (df2[0] - df2[1]) && (df2[0] - df2[1]) < FRONT_SENSORS_NOISE_THRESHOLD)
                //         {
                //             DF2 = df2[0];
                //         }
                //     }
                //     isFrontSensorsUpdated = true;
                // }
                break;
        }
    }
}


















































inline void SystemController::moveStraight()
{
    isRightTurnAllowed = false;
    
    /*
        distance unit: mm

        // 차체의 기울기는 우측 2센서가 측정한 거리값의 차이로 표현할 수 있다.
    */

    // 벽에서 유지해야 할 목표 거리
    constexpr long TARGET_DISTANCE = 150L;

    // 차체는 목표 거리와 70mm 이상일 때, 목표 거리를 향해 최대 기울기로 접근한다.
    constexpr long MINIMUM_DISTANCE_FOR_MAX_SLOPE = 70L;

    // 우측 2센서 사이의 거리
    constexpr long DISTANCE_BETWEEN_RIGHT_SENSORS = 150L;

    // 사용하는 초음파 센서 모델의 안정 측정 각도는 30도 이내이며, 안전하게 25도로 한다.
    // (최대 기울기) = (두 센서 사이의 거리) * sin(25도)
    constexpr double MAX_SLOPE = 0.42 * static_cast<double>(DISTANCE_BETWEEN_RIGHT_SENSORS);


    // distance to target
    static long dTT;
    static double targetSlope;
    static double currentSlope;
    // slopeDiff = targetSlope - currentSlope
    static double slopeDiff;



    /*
        과정 1. 목표 기울기 계산
    */
    dTT = (DR1 + DR2) / 2 - TARGET_DISTANCE;

    if (MINIMUM_DISTANCE_FOR_MAX_SLOPE <= dTT)
    {
        targetSlope = MAX_SLOPE;
    }
    else if ((-MINIMUM_DISTANCE_FOR_MAX_SLOPE <= dTT) && (dTT < MINIMUM_DISTANCE_FOR_MAX_SLOPE))
    {
        targetSlope = static_cast<double>(dTT) * MAX_SLOPE / static_cast<double>(MINIMUM_DISTANCE_FOR_MAX_SLOPE);
    }
    else if (dTT < -MINIMUM_DISTANCE_FOR_MAX_SLOPE)
    {
        targetSlope = -MAX_SLOPE;
    }


    /*
        과정 2. 현재 기울기에서 목표 기울기로의 변화량(차체의 회전 속도) 계산
    */
    currentSlope = static_cast<double>(DR1) - static_cast<double>(DR2);

    currentSlope = CLAMP(currentSlope, -MAX_SLOPE, MAX_SLOPE);

    slopeDiff = targetSlope - currentSlope;

    if (slopeDiff > 0.)
    {
        stepper1.setSpeedRatio(1.);
        stepper2.setSpeedRatio(1. - slopeDiff * 0.7 / (2. * MAX_SLOPE));
    }
    else
    {
        stepper1.setSpeedRatio(1. - -slopeDiff * 0.7 / (2. * MAX_SLOPE));
        stepper2.setSpeedRatio(1.);
    }
}


inline void SystemController::turnRight()
{
    stepper1.setDirection(HIGH);
    stepper2.setDirection(LOW);
    stepper1.setSpeedRatio(1.);
    stepper2.setSpeedRatio(1.);

    savedStepCountAtTurnStart = stepCount;
    isTurning = true;
    ultrasonic.setLogDistanceSwitch(false); // 회전 중에 거리를 기록하지 않는다.
}


inline void SystemController::turnLeft()
{
    stepper1.setDirection(LOW);
    stepper2.setDirection(HIGH);
    stepper1.setSpeedRatio(1.);
    stepper2.setSpeedRatio(1.);

    savedStepCountAtTurnStart = stepCount;
    isTurning = true;
    ultrasonic.setLogDistanceSwitch(false); // 회전 중에 거리를 기록하지 않는다.
}


inline bool SystemController::canMoveStraight()
{
    return (isValid(DR1) && isValid(DR2) && isValid(dr1[0]) && isValid(dr2[0])) // 사용하고자 하는 거리값이 유효한지 확인한다.
        && (DR1 == dr1[0] && DR2 == dr2[0]); // 현재 거리값이 필터값이라면 직진
}


inline bool SystemController::canTurnRight()
{
    // 필터값이 아니기에 주행 조정이 불가능하므로, 일직선으로 주행한다.
    stepper1.setSpeedRatio(1.0);
    stepper2.setSpeedRatio(1.0);

    if (stepCount > STEPS_FOR_TURN_COOLDOWN // 연속 회전을 금지하기 위한 딜레이
        && (isValid(DR1) && isValid(DR2) && isValid(dr1[0]) && isValid(dr2[0]) && isValid(dr2[1])) // 사용하고자 하는 거리값이 유효한지 확인한다.
        && dr1[0] - DR1 > RIGHT_TURN_THRESHOLD // filtering conditon1
        && (dr2[0] - DR2 > RIGHT_TURN_THRESHOLD && dr2[1] - DR2 > RIGHT_TURN_THRESHOLD)) // filtering conditon2
    {
        // 우회전 가능하면 현재 stepCount를 저장한다. 불가능하면 초기화한다.
        if (isRightTurnAllowed == false)
        {
            savedStepCountBeforeRightTurnStart = stepCount;
            isRightTurnAllowed = true;
        }

        // 우회전 가능하면 일정 스텝 이후 우회전한다.
        if (stepCount - savedStepCountBeforeRightTurnStart > STEPS_UNTIL_RIGHT_TURN)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        isRightTurnAllowed = false;
        return false;
    }
}


inline bool SystemController::canTurnLeft()
{
    return (stepCount > STEPS_FOR_TURN_COOLDOWN) // 회전 직후, 일정 스텝 동안 회전을 금지한다.
        && (isValid(df1[0]) && isValid(df1[1]) && isValid(df1[2]) && isValid(df2[0]) && isValid(df2[1]) && isValid(df2[2]))
        && ((df1[0] < LEFT_TURN_THRESHOLD && df1[1] < LEFT_TURN_THRESHOLD && df1[2] < LEFT_TURN_THRESHOLD) || (df2[0] < LEFT_TURN_THRESHOLD && df2[1] < LEFT_TURN_THRESHOLD && df2[2] < LEFT_TURN_THRESHOLD));
}


inline bool SystemController::isTurnEnd()
{
    return stepCount - savedStepCountAtTurnStart > STEPS_FOR_TURN_DURATION;
}


inline void SystemController::processTurnEnd()
{
    // 스텝카운트 초기화
    resetStepCount();

    // 회전으로 변경된 방향을 되돌린다.
    stepper1.setDirection(HIGH);
    stepper2.setDirection(HIGH);

    // 측정 거리를 다시 기록한다.
    ultrasonic.setLogDistanceSwitch(true);

    // 직진 주행을 위해서 우측 센서부터 우선 측정한다.
    ultrasonic.setNextTrigIndex(0);


    // 지워도 된다고 봐. 테스트까지 하고 결정하자.
            // 회전 도중에도 센싱은 계속되어 로우값/필터값이 생산되는데, 필터값이 잘못 생성될 수도 있으니, 필터값을 targetLine(150)으로 초기화한다.
            // @@@@@@@@@@@@@@@@@@@@@@@@@@@@ 이거 필요할 거라고 생각해. 나중에 보자.
            // DR1 = targetLine;
            // DR2 = targetLine;
            // DR1 = 200;
            // DR2 = 200;
}











































inline uint8_t* SystemController::copyAndReturnArray(const uint8_t* _src, size_t _count)
{
    uint8_t* dest = new uint8_t[_count];
    memcpy(dest, _src, sizeof(uint8_t) * _count);
    return dest;
}












#endif // SYSTEM_CONTROLLER_HPP