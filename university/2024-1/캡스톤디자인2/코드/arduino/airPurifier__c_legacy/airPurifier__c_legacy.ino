/*

stepper_and_ultrasonic3_SLAM5_TEST4 파일은 직진만 있어.

stepper_and_ultrasonic3_SLAM5_TEST5 파일은, 우회전을 추가하자.

stepper_and_ultrasonic3_SLAM5_TEST6 파일은 TEST5 파일에서 좌회전도 추가.

stepper_and_ultrasonic3_SLAM5_TEST7 파일은 TEST6 파일에서, 팀장이 주문한 것들.
->
우측센서값들 중 몇 개 이상이 400 이상이면?
flag를 올리고 아래의 과정을 수행한 후, flag를 내린다.
->
제자리 우회전 시작.  ->  전방센서값 차이가, 4000 이내(센싱 최대거리) && 전방센서값의 일부분(5%?) 이내라면,  ->  직진/우회전 금지 및 stepperRatio를 모두 0.8로 고정하여 벽을 향해 직진한다.
->  좌회전을 하면, flag를 내리고, 정상 주행한다.





stepper_and_ultrasonic3_SLAM5_TEST8 파일은 TEST7에서 mappingCode(좌표연산) 추가.

stepper_and_ultrasonic3_SLAM5_TEST9 파일은 TEST8에서, app에 값을 보내기 위한 좌표값 저장 형태를 추가한다.

stepper_and_ultrasonic3_SLAM5_TEST91, TEST92, ... -> TEST9 이어서.



stepper_and_ultrasonic3_SLAM6 : SLAM5_TEST91 + [적외선 추가하여 1map 완성]



stepper_and_ultrasonic3_SLAM6_TEST1 : SLAM6 + [어디서든 정지 후 이어서 가동 (팬모터 구역 확보 및 외부 제어 기능)]








!! 문제점
1.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
원점을 (0, 0)으로 강제하는데,
setup()에서 벽 찾기로 시작하면 원점이 아닌 곳을 (0, 0)으로 하여 시작하기 때문에,
map이 틀어진다.
->
어디 벽 만나든 원점으로 가서 거기서 시작.
처음 원점 인식하면 flag를 올리자.
그 다음 원점 인식하면 원래 있던 flag를 올리자.
// 회전을 기점으로 flag가 초기화된다. 처음 원점을 인식하면 turnCount

// turn
















  



1.
회전 도중에는 절대로 멈추지 않는다.

1.
모터를 어떻게 멈춰?
timer interrupt  ->  ISR  ->  ++stepCount
->
timer(3, 4) interrupt disable
->
void stopMove()
{
  // disable timer interrupt
  TIMSK3 &= ~(1 << OCIE3A);
  TIMSK4 &= ~(1 << OCIE4A);

  // enable stepper driver
  digitalWrite(ENABLE1, HIGH);
  digitalWrite(ENABLE2, HIGH);
}
void startMove()
{
  // reset timer compare value to max
  ocr3a_val = 65535;
  ocr4a_val = 65535;

  // enable timer interrupt
  init_timer();

  // enable stsepper driver
  digitalWrite(ENABLE1, LOW);
  digitalWrite(ENABLE2, LOW);
}


1.
타이밍 제어
->
스텝 제어: 스테퍼가 멈추면 타이밍도 멈춘다.
시간 제어: 조금 복잡해진다. 현재 시간 제어가 trig() 하나 뿐이던가? 얘는 뭐 상관없으니까, 냅두자.

1.
initValues: 멈췄다 가면, 다시 초기화해야 하는 값들이 있는가? 살펴보자.










1.
전체 분기
->
!_isTurning
	우측 2센서 업데이트
		filteredValue
			// straight()
		!_filteredValue
			// turnRight()

	좌측 2센서 업데이트
		// turnRight();

isTurning
	회전시간체크
		// 값들 초기화





회전 도중에 멈추지 않아.
->
!isTurning 분기에서 stopFlag 수시로 확인.




정지냐, 아니냐 분기로 묶는다.
그리고, 초음파는 확실히 isMoving에 둬.
멈추는 걸 입력받는 코드는 isMoving에 둬. // if(~~) { isMoving = false; }
->


mainLoop


    // 멈추면 안 되고 반드시 지속되어야 하는 것들은, mainLoop 분기로 둬.
    // e.g. 통신, 인터럽트 제외한 외부 입력, ...


    // 수신 버퍼는 실시간으로 확인한다.
    // 정해진 데이터에 따라 각 if문 분기로 들어가고, 분기에 해당하는 실행을 곧바로 실행하거나, 실행이 다른 곳에 있어서 flag를 세울 수 있다.
    RecvData()


    isMoving


        // 초음파
        trig();


        !isTurning
            우측 2센서 업데이트
                filteredValue
                    moveStraight()
                !filteredValue
                    turnRight()

            좌측 2센서 업데이트
                turnLeft();


        isTurning
            회전시간체크
                값들 초기화



        if (멈춰)
            isMoving = false;




    !isMoving
        공기청정기능








































1.
전원 키면 1map 자동으로 완성.



1.
원격 제어
->
주행 - 멈춤/이어서
공청 - 멈춤/이어서

어디가서 공청하세요
팬모터 속도
그만하고 집으로 가세요

시간 표시 (LCD가 아니어도 돼)

공청 시간제어
공청 시간예약































공청은 !isMoving 분기에서 실행한다.


일단, 외부 입력 하나를 두고, 중간에 멈춰보자.
멈추기 이어서하기 함수부터 추가하자.

멈추기 함수는 isMoving 분기에서 실행하야 하고,
이어서하기는 !isMoving 분기에서 실행해야 한다.




















*/





// dbg : LED 표시기
const int RED = 44;
const int GREEN = 45;
const int BLUE = 46;























/*
  pinDef Wi-Fi module - ESP8266

  HW Serial2(16, 17)

  app의 요청에만 데이터를 전송한다.
*/
void setupWifi();

String dataToSend = "";
const int32_t SEND_INTERVAL = 300000;
inline int sendATCommandToESP8266(String _send);
inline int sendATCommandToESP8266(String _send, String _recv);
inline void sendDataToApp();

inline void sendPMValuesToApp(int);
inline void sendCanvasCarLocationToApp();
inline void sendCanvasMapToApp(int);

void recvDataFromApp();
void dataReceivedHandler(String _message);




enum CommandStatus {
    NO_COMMAND,
    FIXED,
    MOVING
    // NONE,
    // NO_OP,
    // EMPTY,
    // NO_ACTION,
    // NOTHING,
    // NO_STATUS,
    // COMMANDLESS,
    // WAITING
};

CommandStatus appCommand = NO_COMMAND;

int32_t cmd_location[2] = { 0, };

int cmd_turnCount = 0;

uint32_t clk_cleanStart = 0;

uint32_t cmd_duration = 0;

int cmd_touchCondition = 0;








































uint32_t clk_main = 0;







/*
  pinDef ultrasonic

  trigPin(4, 5)  ->  echoPin(2)  ->  echoISR1
  trigPin(6, 7)  ->  echoPin(21)  ->  echoISR2        // arduino MEGA external interrupt pin -> 2, 3, 18, 19, 20, 21

  초음파 센서는 간섭으로 인해 하나씩 실행한다.
  ISR을 번갈아가며 실행하기 위해, 실행 순서는 "4 - 6 - 5 - 7"로 정한다.

  trigPin(4) - TRIG[0] - distance[0] - dr1 : 우측 뒤쪽 초음파
  trigPin(6) - TRIG[1] - distance[1] - dr2 : 우측 앞쪽 초음파
  trigPin(5) - TRIG[2] - distance[2] - df1 : 전방 오른쪽 초음파
  trigPin(7) - TRIG[3] - distance[3] - df2 : 전방 왼쪽 초음파
*/
const int TRIG[4] = { 4, 6, 5, 7 };
const int ECHO1 = 2;
const int ECHO2 = 21;



// 작동 주기.
uint32_t TRIG_INTERVAL = 100000;

// 작동 주기를 위한 시간 측정.
unsigned long clk_trigStart = 0;

// echoPin을 공유하기 때문에 센서를 구분하기 위해서 trig(), echoISR() 두 함수가 인덱스를 공유한다.
unsigned int index_ultrasonic = 3;       // 3으로 두면, trig()에서 하나 더하여 0으로 시작한다.

// (초음파 왕복 시간) = (echo 핀의 HIGH 펄스 시간) = (echo 핀의 상승-하강 엣지 시간 차이)
unsigned long echoStart1 = 0;       // echoISR1
unsigned long echoEnd1 = 0;
unsigned long echoStart2 = 0;       // echoISR2
unsigned long echoEnd2 = 0;


// 센서 측정값을 직접 저장한다.
volatile long distance[4][5] = { { 0, }, { 0, }, { 99999, 99999, 99999, 99999, 99999 }, { 99999, 99999, 99999, 99999, 99999 } };        // in mm.

// 편하게 네이밍.
volatile long* dr1 = distance[0];
volatile long* dr2 = distance[1];
volatile long* df1 = distance[2];
volatile long* df2 = distance[3];



const long targetLine = 130;       // 벽에서 유지선까지의 거리.

// 우회전, 좌회전 임계값
const long rightTurnThreshold = 150;
const long leftTurnThreshold = 260;

// 노이즈 임계값
// 이전 값과 현재 값의 차이가 const 이하면 정상값으로 저장하고, const 이상이면 noise로 보고 저장하지 않는다.
// 우측 2센서는 주행과 우회전 모두에 사용되기 때문에 rightTurnThreshold 이상이어도 무시한다.       // 전방 2센서는 별도의 제한이 없다.
const long rightSensorsNoiseThreshold = 50;
const long frontSensorsNoiseThreshold = 100;

// filtering된 마지막 거리값을 갖는다.
// dr1[0], dr2[0], df1[0], df2[0]  ->  noise filtering 및 정상값 저장  ->  DR1, DR2, DF1, DF2        // 측정이 끝나고 ISR에서 판별하여 저장한다.
volatile long DR1 = 150;
volatile long DR2 = 150;
// volatile long DF1 = 0;
// volatile long DF2 = 0;










































/*
  pinDef stepper
  
  stepper1 : 차체 왼쪽 바퀴
  stepper2 : 차체 오른쪽 바퀴
*/
const int ENABLE1 = 26;        // LOW_on, HIGH_off
const int STEP1 = 27;
const int DIRECTION1 = 28;

const int ENABLE2 = 32;
const int STEP2 = 33;
const int DIRECTION2 = 34;



// 기본 스테퍼 속도
double stepperSpeed1 = 3.;
double stepperSpeed2 = 3.;

// 스테퍼 속도에 역수를 취하고 상수를 곱하여 stepDelay를 결정한다.        // 모터 속도 1 -> 스텝딜레이 5000us
unsigned long stepDelay1 = 99999999;
unsigned long stepDelay2 = 99999999;
const unsigned long constStepDelay = 5000;

// distance 값들로 실시간 주행 보정이 이루어진다.
double ratio_stepperSpeed1 = 1.;
double ratio_stepperSpeed2 = 1.;



/*
  스텝딜레이에 맞춰 스텝 실행  ->  timer interrupt - ISR

  stepper(1, 2) : 16-bit timer(3, 4)       system clock : 16MHz       // prescaler : 8
  ->
  타이머 클럭 주기 : 8 / 16MHz = 0.5us        // 타이머 클럭 주기는 stepDelay의 최대 오차가 된다.
  최대 타이머 주기 : 0.5us * 256 * 256 = 32768us        // 30000 이상 어딘가부터 오류.
*/
// stepDelay -> timer - OCRnA
uint16_t ocr3a_val = 65535;       // init max
uint16_t ocr4a_val = 65535;







































/*
  주행 관련.
*/
volatile int32_t stepCount = 0;

// stepCount를 임시로 저장한다.
int32_t savedStepCount = 0;

// 우회전 가능 확인 후, 일정 스텝 이후에 우회전한다.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
bool isRightTurnAllowed = false;
const int32_t rightTurnReadyStepCount = 950;

// 회전 지속시간이며, stepCount로 나타낸다.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const int32_t turnDuration = 1550;

// 우회전 직후, 일정 스텝 동안 우회전 조건 확인을 금지한다.
const int32_t turnCooldown = 500;





































/*
  SLAM

  매 회전마다 toggle carAxis
  좌회전에 ++carDir
  우회전에 --carDir

  rawMap 데이터 구조: (x, y) 좌표를 저장하는 turnPoint[2]의 집합.
*/
int32_t rawCarLocation[2] = { 0, };        // [x, y]
int32_t canvasCarLocation[2] = { 0, };        // [x, y]

int32_t rawMap[20][2] = { 0, };       // rawMap[turnCount][carAxis]
int32_t canvasMap[20][2] = { 0, };        // canvasMap[turnCount][carAxis]
int mapIndexMax = 20;

int turnCount = 0;
int carAxis = 0;        // carLocation[carAxis]       0: x        1: y
int carDir = 0;       // // 0: +x    1: +y    2: -x    3: -y

int prevCarAxis = 0;
int prevCarDir = 0;

inline int returnDir(int _carDir);

int32_t x_min_rawMap = 0;
int32_t y_min_rawMap = 0;
double ratio_stepToPixel = 0;       // map으로의 축소 비율이다.

const int32_t x_max_canvas = 400;       // 태희한테 받아야 해.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const int32_t y_max_canvas = 400;

inline void convertRawToCanvas();

bool isOriginDetected_initial = false;
bool isOriginDetected = false;
bool hasCanvasMap = false;





























/*
  undefined
*/
void stopMove();
void startMove();

// bool moveFlag = true;       // bool moveFlag = false;  ->  초기 세팅 끝나고 true로.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@








































/*
  pinDef IR

  하부 적외선 센서: 검은색의 원점을 인식한다.

  isMoving 분기에 포함된다.

  trig() 시간제어에 두어 0.1초의 쿨다운을 갖는다.

  각 센서에 0 ~ 10 사이의 값을 가지는 counter를 두어 감지 유무에 따라 +1, -1을 더한다.

  원점 감지 조건: counter = 10

  // 센서 감도조절은 끝났으니, 디지털값 1만 확인하면 된다.

*/
const int originDetector1 = 38;
const int originDetector2 = 39;

int originDetectCounter1 = 0;
int originDetectCounter2 = 0;

inline void updateOriginCounters();
inline void initOriginCounters();

















































/*
  dust sensor PMS7003

  HW Serial1(18, 19)

  UART
  데이터 패킷은 총 32바이트
  ->
  시작 2바이트 - header(0x424d)
  데이터 28바이트
  끝 2바이트 - checksum

  체크섬 확인을 포함하여 개별 오류에 대한 검증은 지웠다.
  개별 오류는 다음 측정에 영향을 끼치지 않는다.
  오류의 측정은, 값을 보내지 않는 것으로 해야 한다.

  모터의 속도가 1초마다 바뀔 수는 없으니,
  배열로 저장하고 평균을 내어 적용하면 될 듯하다.
*/
const int PMS7003BufferLength = 32;
byte PMS7003Buffer[PMS7003BufferLength] = { 0, };
bool PMS7003DataAvailable = false;

// mainLoop에서 팬모터 자동 제어에 사용하기 위해서, 주소값 하나 가져온다.
int* p_pm10 = NULL;

// 먼지값 업데이트되면, 팬모터 속도 적용에 1회 사용할 수 있다.
bool isPMValuesUpdated = false;










/*
  pinDef fanMotor

  fanMotor : use timer1 PWM

  공청 ON  ->  fanMotor ON

  fanPower는 PMValues에 비례한다.
*/
const int fanEna = 11;
const int fanDir1 = 12;
const int fanDir2 = 13;

uint8_t fanSpeed = 0;

void setFanSpeed(int _fanSpeed_0_255);
bool fanPowerFlag = false;        
// bool fanPowerFlag = true;        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@ dbg

void startFan();
void stopFan();

void startFan()
{
  fanPowerFlag = true;
}

void stopFan()
{
  fanPowerFlag = false;
}

void setFanSpeed(int _fanSpeed_0_255)
{
  // // dbg
  // fanPowerFlag = true;

  if (fanPowerFlag)
  {
    if (_fanSpeed_0_255 < 0)
      _fanSpeed_0_255 = 0;
    else if (_fanSpeed_0_255 > 255)
      _fanSpeed_0_255 = 255;
    analogWrite(fanEna, _fanSpeed_0_255);
  }
  else
    analogWrite(fanEna, 0);
}









/*
  pinDef touchSwitch

  touch(1, 2, 3, 4)  ->  digitalPin(50, 51, 52, 53);

  touch * 4
  ->
  interrupt 말고 전부 다 polling으로 하자.

  동작 방식은 risingEdge
  ->
  다행히 switch처럼 chattering이 있진 않아서,
  그냥 최근 1인 놈으로.
  동시에 눌리는 그딴 건 신경쓰지 말자.
*/
const int touchSwitch[4] = { 50, 51, 52, 53 };
#define FAN_OFF 0
#define FAN_LOW 1
#define FAN_HIGH 2
#define FAN_AUTO_FROM_PMVALUES 3
int touchCondition = FAN_OFF;

// 먼지 값에 따라 fanSpeed를 변경할 때 사용한다.
#define FAN_MINSPEED 150
#define FAN_MAXPM10VALUE 200


inline void readPMS7003Data();
inline void calculatePMValues();
inline void checkTouchSwitch();
inline void applyTouchSwitchToFanSpeed();



































































// ------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------- setup
// ------------------------------------------------------------------------------------------------

void init_ultrasonic();
void init_stepper();
void init_IR();
void init_fan();
void init_touchSwitch();
void init_timer();

inline void updateOriginCounter();
inline void trig();
inline void updateStepDelays();

inline void turnLeft();

bool isMoving = true;       // bool isMoving = false;
bool isTurning = false;

bool isAirCleaning = false;



void setup()
{
  // dbg
  Serial.begin(9600);

  // dust sensor - PMS7003
  Serial1.begin(9600);

  // Wi-Fi module - ESP8266
  Serial2.begin(9600);

  setupWifi();

  init_timer();

  init_ultrasonic();

  init_stepper();

  init_IR();

  init_fan();
  init_touchSwitch();



  // // initial delay
  // cli();
  // delay(2000);
  // sei();



  // dbg
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
















  // // 우측 2센서, 2번씩 측정.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 꼭 2번 아니어도 됨.
  // {
  //   index_ultrasonic = 3;
  //   trig();
  //   delay(100);
  //   trig();
  //   delay(100);
  //   index_ultrasonic = 3;
  //   trig();
  //   delay(100);
  //   trig();
  //   delay(100);
  // }

  // if (dr1[0] > 400 || dr1[1] > 400 || dr2[0] > 400 || dr2[1] > 400)
  // {
  //   digitalWrite(DIRECTION2, HIGH);
  //   ratio_stepperSpeed1 = 0.2;
  //   ratio_stepperSpeed2 = 0.2;
  //   updateStepDelays();


  //   int32_t dist1 = 0, dist2 = 0;
  //   int32_t diff1 = 0, diff2 = 0;
  //   const int32_t SAFE_MEASUREMENT_DISTANCE = 2000;
  //   const double WALL_ALIGNMENT_THRESHOLD = 0.06;
  //   while (1)
  //   {
  //     delay(200);
  //     // 전방 2센서 확인.
  //     index_ultrasonic = 1;
  //     trig();
  //     delay(100);
  //     trig();
  //     delay(100);


  //     // 센서의 안정적인 측정 범위를 벗어나면, 거리를 다시 확인한다.
  //     if (df1[0] > SAFE_MEASUREMENT_DISTANCE || df2[0] > SAFE_MEASUREMENT_DISTANCE)
  //     {
  //       continue;
  //       // exception@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  //     }
  //     else
  //     {
  //       // 
  //       dist2 = dist1;
  //       diff2 = diff1;

  //       // 두 거리값을 구하고, 둘의 차이를 구한다.
  //       dist1 = (df1[0] + df2[0]) / 2;
  //       diff1 = df1[0] - df2[0];
  //       diff1 = diff1 >= 0 ? diff1 : -diff1;        // 차이는 절댓값으로.

  //       Serial.println(String(df1[0]) + "    " + String(df2[0]) + "    " + String(diff1));

  //       // 둘의 차이가 거리값의 5% 이내라면, 벽과 마주하고 있다고 판단하고 while문을 나간다.
  //       if ((double)diff1 / (double)dist1 < WALL_ALIGNMENT_THRESHOLD)
  //       {
  //         if ((double)diff2 / (double)dist2 < WALL_ALIGNMENT_THRESHOLD)
  //         {
  //           break;
  //         }
  //       }
  //     }
  //   }

    
  //   // 현재 벽을 바라보고 있으며, 벽으로 직진하여 좌회전하는 것으로 mainLoop로 진입한다.
  //   digitalWrite(DIRECTION1, LOW);
  //   digitalWrite(DIRECTION2, LOW);        // 직진.
  //   ratio_stepperSpeed1 = 1.;
  //   ratio_stepperSpeed2 = 1.;
  //   updateStepDelays();
  //   for (int i = 0; i < 4; ++i)       // 전방 2센서로 좌회전 체크해야하기 때문에, 초기화한다.
  //   {
  //     for (int j = 0; j < 5; ++j)
  //     {
  //       if (i < 2)
  //         distance[i][j] = 0;
  //       else
  //         distance[i][j] = 99999;
  //     }
  //   }
  //   while (1)
  //   {
  //     // 전방 2센서 확인.
  //     index_ultrasonic = 1;
  //     delay(200);
  //     trig();
  //     delay(100);
  //     trig();
  //     delay(100);
  //     // 주행 중의 좌회전 조건과 동일하게 한다.
  //     if ((df1[0] < leftTurnThreshold && df1[1] < leftTurnThreshold && df1[2] < leftTurnThreshold) || (df2[0] < leftTurnThreshold && df2[1] < leftTurnThreshold && df2[2] < leftTurnThreshold))
  //     {
  //       savedStepCount = stepCount;

  //       digitalWrite(DIRECTION1, HIGH);
  //       ratio_stepperSpeed1 = 1.;
  //       ratio_stepperSpeed2 = 1.;
  //       updateStepDelays();
  //       // dbg
  //       digitalWrite(RED, LOW);        // leftTurn: BLUE
  //       digitalWrite(GREEN, LOW);
  //       digitalWrite(BLUE, HIGH);

  //       while (stepCount - savedStepCount < turnDuration);

  //       break;
  //     }
  //   }
  //   // mainLoop로 넘어가면서 엇갈리는 것이 있는지, 확인해봐야 한다.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  //   stepCount = 0;
  // }
}





























bool aFlag = false;
bool bFlag = false;

bool isSendingCanvasMap = false;


// ------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------- mainLoop
// ------------------------------------------------------------------------------------------------

// inline void trig();

/*
  차량의 모든 주행은 stepDelay로 결정한다.
  아래의 주행 함수들은, stepDelay를 단 한 번 변경한다.
*/
inline void moveStraight();
inline void turnLeft();
inline void turnRight();
inline void updateStepDelays();

void echoISR1();
void echoISR2();



uint32_t clk_sendCooldown = 0;
uint32_t tmpClk = 0;
int tmp_turnCount = 0;

uint32_t clk_tmp = 0;

// bool isTurning = false;

volatile bool isRightSensorsUpdated = false;
volatile bool isFrontSensorsUpdated = false;

int turnCount_for_sendCanvasMap = 0;
bool sendFlag_canvasMap = false;


// dbg
uint32_t TMP_clk = 0;
bool TMP_flag = false;
uint32_t TMP_clk2 = 0;
bool isSendingDataToApp = false;

void loop()
{
  clk_main = micros();



  // dbg
  if (clk_main - TMP_clk > 200000)
  {
    TMP_clk = clk_main;
    Serial.println(String(stepCount) + "    " + String(isOriginDetected) + "    " + String(isOriginDetected_initial));
    Serial.println(String(originDetectCounter1) + "    " + String(originDetectCounter2) + "    " + String(appCommand));
    Serial.println(String(turnCount) + "    " + String(turnCount_for_sendCanvasMap) + "    " + String(sendFlag_canvasMap));
    // Serial.println(String(canvasCarLocation[0]) + "    " + String(canvasCarLocation[1]) + '\n');
    Serial.println(String(touchCondition) + "    " + String(fanPowerFlag) + "    " + String(*p_pm10));
    // Serial.println(isRightTurnAllowed);
    // Serial.println(String(dr1[0]) + "    " + String(dr2[0]) + "    " + String(df1[0]) + "    " + String(df2[0]));
    // Serial.println("--");
    // if (isRightSensorsUpdated)
    // {
    //   stopMove();
    // }

  }





  /*
    이제, 1map 완성 직후 canvasMap 전체를 다다다다 보낼 거야.
    우선, 보낼 카운터를 저장하자.
    -> 마지막 회전에 저장함.

    카운터는, turnCount를 써야 하는데,
    
    첫 조건: 캔버스맵 있니?

    둘 조건: 다 보내면 true가 되는 놈이 false이니?

    셋 조건: 마지막 원점까지 보냈니?


    실행문1: 둘 조건에 사용되는 flag를 true로 만들고, 영영 안 보내기.
  */
  
  // #################################################################
  if (hasCanvasMap && bFlag)
  {
    if (!sendFlag_canvasMap)
    {
      if (!isSendingDataToApp)
      {
        if (clk_main - clk_tmp > 200000)
        {
          if (tmp_turnCount <= turnCount_for_sendCanvasMap)
          {
            isSendingCanvasMap = true;
            sendCanvasMapToApp(tmp_turnCount);
          }
          
          if (tmp_turnCount == turnCount_for_sendCanvasMap + 1)
          {
            sendCanvasMapToApp(0);
            sendFlag_canvasMap = true;
            isSendingCanvasMap = false;
          }

          ++tmp_turnCount;
        }
      }
    }
  }



  sendDataToApp();
  /*
    2초 내에, 3개를 나눠서 보낼 거야.

    각각을 보냈다는 flag가 필요해.

    설계부터.
    ->
    0 이상일 때 보내고, 0.6초 이상일 때 보내고, 1.2초 이상일 때 보낼 거야.
    ->
    0, 0.6, 1.2는, clk_main - clk_cooldown
  */
  static bool flag1 = false;
  static bool flag2 = false;
  static bool flag3 = false;

  // clk_sendCooldown 초기값은 0

  tmpClk = clk_main - clk_sendCooldown;
  if (tmpClk > 3000000)
  {
    clk_sendCooldown = clk_main;
    flag1 = false;
    flag2 = false;
    flag3 = false;
  }


  

  // 느낌표
  if (!isSendingDataToApp && !isSendingCanvasMap)
  {
    if (tmpClk >= 0)
    {
      if (!flag1)
      {
        flag1 = true;

        dataToSend = "!";
        sendDataToApp();
      }
    }
  }

  // 먼지값
  if (!isSendingDataToApp && !isSendingCanvasMap)
  {
    if (tmpClk > 1000000)
    {
      if (!flag2)
      {
        flag2 = true;

        sendPMValuesToApp(*p_pm10);
      }
    }
  }

  // 실시간 좌표값
  if (!isSendingDataToApp && hasCanvasMap && sendFlag_canvasMap)
  {
    if (tmpClk > 2000000)
    {
      if (!flag3)
      {
        flag3 = true;

        if (aFlag)
        {
          aFlag = false;

          dataToSend = "a";
          sendDataToApp();
        }
        else
        {
          sendCanvasCarLocationToApp();
        }
      }
    }
  }










  /*
    mainLoop 분기의 코드들은 항시 실행되어야 한다.
    e.g. 통신, ...
  */
  recvDataFromApp();



  /*
    dust sensor : 보드 가동 중에 항상 값을 받는다.
  */
  if (Serial1.available() > 0)
  {
    readPMS7003Data();

    if (PMS7003DataAvailable)
    {
      calculatePMValues();
      PMS7003DataAvailable = false;


    }
  }

  checkTouchSwitch();
  applyTouchSwitchToFanSpeed();











  /*
    명령을 받는다.
    ->
    1. appCommand = FIXED
    2. cmd_turnCount = 2;
    3. cmd_duration = 3000000;
    ->
    cmd_location[2] = { canvasMap[cmd_turnCount][0], canvasMap[cmd_turnCount][1] };
    startMove();
  */
  if (appCommand == FIXED)
  {
    // 가동 위치에 도착하기 전까지, 시간을 저장한다.
    if (isMoving)
    {
      clk_cleanStart = clk_main;
    }
    else        // 가동 위치에 도착하면, 시간을 잰다.
    {
      if (clk_main - clk_cleanStart > cmd_duration)
      {
        appCommand = NO_COMMAND;
        stopFan();
        startMove();
      }
    }
  }


  /*
    명령을 받는다.
    ->
    1. appCommand = MOVING
    2. cmd_duration = 3000000;
    ->
    // TRIG_INTERVAL = 200000;
    // stepperSpeed1 = 1.5;
    // stepperSpeed2 = 1.5;
    // startFan();
    // clk_cleanStart = clk_main;
    isMoving = true;
    startMove();
  */
  // mainLoop
  if (appCommand == MOVING)
  {
    if (clk_main - clk_cleanStart > cmd_duration)
    {
      appCommand = NO_COMMAND;
      stopFan();
      TRIG_INTERVAL = 100000;
      stepperSpeed1 = 3.;
      stepperSpeed2 = 3.;
      updateStepDelays();
    }
  }






  // // dbg
  // if (TMP_flag && clk_main - TMP_clk2 > 2000000)
  // {
  //   // 1회만 실행한다.
  //   TMP_flag = false;

  //   // // 받을 데이터
  //   // appCommand = FIXED;
  //   // cmd_turnCount = 2;
  //   // cmd_duration = 3000000;
  //   // cmd_touchCondition = FAN_HIGH;
    
  //   // // 실행할 명령
  //   // touchCondition = cmd_touchCondition;
  //   // cmd_location[0] = canvasMap[cmd_turnCount][0];
  //   // cmd_location[1] = canvasMap[cmd_turnCount][1];
  //   // startMove();



  //   // 받을 데이터
  //   appCommand = MOVING;
  //   cmd_duration = 10000000;
  //   cmd_touchCondition = FAN_LOW;

  //   // 실행할 명령
  //   TRIG_INTERVAL = 100000;
  //   stepperSpeed1 = 3.0;
  //   stepperSpeed2 = 3.0;
  //   clk_cleanStart = clk_main;
  //   touchCondition = cmd_touchCondition;
  //   startFan();
  //   startMove();
  // }








  // 실시간 위치 표현
  if (hasCanvasMap)
  {
    if (!isTurning)
    {
      // rawMap 값 저장
      canvasCarLocation[0] = rawCarLocation[0];
      canvasCarLocation[1] = rawCarLocation[1];
      canvasCarLocation[carAxis] += stepCount * returnDir(carDir);

      // canvasMap으로 변환
      canvasCarLocation[0] = (int32_t)((double)(canvasCarLocation[0] + (-x_min_rawMap)) * ratio_stepToPixel) + 40;
      canvasCarLocation[1] = (int32_t)((double)(canvasCarLocation[1] + (-y_min_rawMap)) * ratio_stepToPixel) + 40;
    }
  }







  if (isMoving)
  {
    // (쿨타임) : TRIG_INTERVAL
    if ((clk_main - clk_trigStart) > TRIG_INTERVAL)
    {
      // 쿨타임 돌리기.
      clk_trigStart = clk_main;

      trig();



      // 원점 감지
      updateOriginCounters();

      if (originDetectCounter1 == 10 && originDetectCounter2 == 10 )
      {
        // 처음 1map은 원점에서 시작한다.
        if (stepCount < 1500)       // 출발할 때 확인
        {
          isOriginDetected_initial = true;
        }


        if (stepCount > 1500)       // 도착할 때 확인
        {
          if (isOriginDetected_initial)
          {
            if (turnCount >= 3)
              isOriginDetected = true;
          }
          else
          {
            isOriginDetected = true;
          }
        }
      }



      // // 실시간 위치 표현
      // if (hasCanvasMap)
      // {
      //   if (!isTurning)
      //   {
      //     // rawMap 값 저장
      //     canvasCarLocation[0] = rawCarLocation[0];
      //     canvasCarLocation[1] = rawCarLocation[1];
      //     canvasCarLocation[carAxis] += stepCount * returnDir(carDir);

      //     // canvasMap으로 변환
      //     canvasCarLocation[0] = (int32_t)((double)(canvasCarLocation[0] + (-x_min_rawMap)) * ratio_stepToPixel) + 40;
      //     canvasCarLocation[1] = (int32_t)((double)(canvasCarLocation[1] + (-y_min_rawMap)) * ratio_stepToPixel) + 40;
      //   }
      // }
      // dbg
      // Serial.println(String(canvasCarLocation[0]) + "    " + String(canvasCarLocation[1]));
      // Serial.println(String(-x_min_rawMap) + "    " + String(-y_min_rawMap) + "    " + String(ratio_stepToPixel));

    }



    if (!isTurning)
    {
      if (isRightSensorsUpdated == true)
      {
        // dbg
        Serial.println("@@@@@@@@@@@@@@@@@@@@");

        isRightSensorsUpdated = false;

        if (dr1[0] == DR1 && dr2[0] == DR2)
        {
          // dbg
          Serial.println("111111111111111111111");
          
          moveStraight();

          isRightTurnAllowed = false;

          // dbg
          // 기울기가 안쪽으로 휠 때,
          if (ratio_stepperSpeed1 > ratio_stepperSpeed2)
          {
            digitalWrite(RED, HIGH);       // magenta
            digitalWrite(GREEN, LOW);
            digitalWrite(BLUE, HIGH);
          }
          // 기울기가 바깥쪽으로 휠 때,
          if (ratio_stepperSpeed1 <= ratio_stepperSpeed2)
          {
            digitalWrite(RED, HIGH);        // white
            digitalWrite(GREEN, HIGH);
            digitalWrite(BLUE, HIGH);
          }
        }
        else
        {
          // dbg
          Serial.println("222222222222222222222");

          // dbg
          digitalWrite(RED, HIGH);        // noise: red
          digitalWrite(GREEN, LOW);
          digitalWrite(BLUE, LOW);

          // 직진 함수로 기울기를 조정할 수 없으니, 기울기를 고정시킨다.
          ratio_stepperSpeed1 = 1.;
          ratio_stepperSpeed2 = 1.;
          updateStepDelays();

          // 회전 직후, 일정 스텝 동안 회전을 금지한다.
          if (stepCount > turnCooldown)
          {
            // 필터링 조건1
            if (dr1[0] - DR1 > rightTurnThreshold)
            {
              // 필터링 조건2
              if (dr2[0] - DR2 > rightTurnThreshold && dr2[1] - DR2 > rightTurnThreshold)
              {
                // 우회전 가능하면, stepCount 저장.
                if (isRightTurnAllowed == false)
                {
                  savedStepCount = stepCount;
                  isRightTurnAllowed = true;
                }

                // 우회전 가능하면, 일정 스텝 이후 우회전.
                if (stepCount - savedStepCount > rightTurnReadyStepCount)
                {
                  isTurning = true;

                  prevCarAxis = carAxis;
                  prevCarDir = carDir;
                  turnRight();
                  savedStepCount = stepCount;
                }
              }
            }
          }
        }
      }



      // 좌회전.
      if (isFrontSensorsUpdated == true)
      {
        isFrontSensorsUpdated = false;

        // 회전 직후, 일정 스텝 동안 회전을 금지한다.
        if (stepCount > turnCooldown)
        {
          // 전방 2센서 중 하나라도, 최근 3개의 거리값이 모두 임계 이하라면, 좌회전한다.
          if ((df1[0] < leftTurnThreshold && df1[1] < leftTurnThreshold && df1[2] < leftTurnThreshold) || (df2[0] < leftTurnThreshold && df2[1] < leftTurnThreshold && df2[2] < leftTurnThreshold))
          {
            isTurning = true;

            prevCarAxis = carAxis;
            prevCarDir = carDir;
            turnLeft();
            savedStepCount = stepCount;
          }
        }
      }
    }
    else // isTurning
    {
      // 회전 중.
      if (stepCount - savedStepCount > turnDuration)
      {
        // 회전 끝.
        isTurning = false;

        


        // 좌표값 계산 및 stepCount 초기화.
        if (!isOriginDetected)
        {
          rawCarLocation[prevCarAxis] += savedStepCount * returnDir(prevCarDir);

          ++turnCount;
          rawMap[turnCount][0] = rawCarLocation[0];
          rawMap[turnCount][1] = rawCarLocation[1];
        }
        else // isOriginDetected
        {
          isOriginDetected = false;

          // 통신으로 canvasMap을 보내주기 위해서, turnCount값을 저장한다.
          turnCount_for_sendCanvasMap = turnCount;

          // 원점 도착하면 app에서 canvasMap을 초기화하라는 명령을 보낼 수 있게 하는 flag.
          aFlag = true;
          

          // 원점에서 둘 다 0이어야 한다.
          if (carAxis || carDir)
          {
            // ERROR
          }
          carAxis = 0;
          carDir = 0;

          convertRawToCanvas();
          turnCount = 0;
          initOriginCounters();

          if (isOriginDetected_initial)
          {
            // 최초 1map 완성 시, 멈춤.
            if (!hasCanvasMap)
            {
              stopMove();
              hasCanvasMap = true;
            }


            // 아무런 명령이 없을 때, 원점에서 멈춘다.
            if (appCommand == NO_COMMAND)
            {
              stopMove();



              // dbg
              // Serial.println("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
              // TMP_flag = true;
              // TMP_clk2 = clk_main;
              // delay(5000);
            }
          }
        }
        stepCount = 0;



        // 회전하는 동안 측정된 값은 사용하지 않는다.
        isRightSensorsUpdated = false;
        isFrontSensorsUpdated = false;

        // 주행을 위해서 우측 센서부터 얼른 측정한다.
        index_ultrasonic = 3;

        // 회전에서 변경한 stepperDir을 되돌린다.
        digitalWrite(DIRECTION1, LOW);
        digitalWrite(DIRECTION2, LOW);

        // 회전 도중에도 센싱은 계속되어 로우값/필터값이 생산되는데, 필터값이 잘못 생성될 수도 있으니, 필터값을 targetLine(150)으로 초기화한다.
        // DR1 = targetLine;
        // DR2 = targetLine;
        DR1 = 200;
        DR2 = 200;
        isRightTurnAllowed = false;
        
        // turn 이후 moveStraight()가 즉각 실행되지 않으므로, turn으로 인해 극단적으로 바뀐 ratio 값을 1로 복귀시킨다. 이후 ratio는 moveStraight()로 조정될 것이다.
        ratio_stepperSpeed1 = 1.;
        ratio_stepperSpeed2 = 1.;
        updateStepDelays();



        // dbg
        Serial.println("-------------------");
        Serial.println(String(rawCarLocation[0]) + "    " + String(rawCarLocation[1]));
        Serial.println(String(rawMap[0][0]) + "    " + String(rawMap[0][1]));
        Serial.println(String(rawMap[1][0]) + "    " + String(rawMap[1][1]));
        Serial.println(String(rawMap[2][0]) + "    " + String(rawMap[2][1]));
        Serial.println(String(rawMap[3][0]) + "    " + String(rawMap[3][1]));
        Serial.println(String(rawMap[4][0]) + "    " + String(rawMap[4][1]));
        Serial.println(String(rawMap[5][0]) + "    " + String(rawMap[5][1]));
        Serial.println(String(rawMap[6][0]) + "    " + String(rawMap[6][1]));
        Serial.println(String(rawMap[7][0]) + "    " + String(rawMap[7][1]));
        Serial.println(String(rawMap[8][0]) + "    " + String(rawMap[8][1]));
        Serial.println(String(rawMap[9][0]) + "    " + String(rawMap[9][1]));
        // Serial.println(String(rawMap[5][0]) + "    " + String(rawMap[5][1]));
        // Serial.println(String(rawMap[6][0]) + "    " + String(rawMap[6][1]));
        // Serial.println(String(rawMap[7][0]) + "    " + String(rawMap[7][1]));
        // Serial.println(String(rawMap[8][0]) + "    " + String(rawMap[8][1]));
        // Serial.println(String(rawMap[9][0]) + "    " + String(rawMap[9][1]) + "\n\n");
        Serial.println("-------------------");
        Serial.println(String(canvasMap[0][0]) + "    " + String(canvasMap[0][1]));
        Serial.println(String(canvasMap[1][0]) + "    " + String(canvasMap[1][1]));
        Serial.println(String(canvasMap[2][0]) + "    " + String(canvasMap[2][1]));
        Serial.println(String(canvasMap[3][0]) + "    " + String(canvasMap[3][1]));
        Serial.println(String(canvasMap[4][0]) + "    " + String(canvasMap[4][1]));
        Serial.println(String(canvasMap[5][0]) + "    " + String(canvasMap[5][1]));
        Serial.println(String(canvasMap[6][0]) + "    " + String(canvasMap[6][1]));
        Serial.println(String(canvasMap[7][0]) + "    " + String(canvasMap[7][1]));
        Serial.println(String(canvasMap[8][0]) + "    " + String(canvasMap[8][1]));
        Serial.println(String(canvasMap[9][0]) + "    " + String(canvasMap[9][1]));
        Serial.println("-------------------");



        // 회전이 끝날 때마다 도착 지점인지 확인한다.
        if (appCommand == FIXED)
        {
          // if (
          // cmd_location[0] - 30 < canvasCarLocation[0] && canvasCarLocation[0] < cmd_location[0] + 30 &&
          // cmd_location[1] - 30 < canvasCarLocation[1] && canvasCarLocation[1] < cmd_location[1] + 30)
          // {
          //   stopMove();
          //   startFan();

          // }
          
          // dbg
          if (cmd_turnCount == turnCount)       // turnCount 확인.
          {
            stopMove();
            startFan();
          }
        }
      }
    }
  }
  else // !isMoving
  {
    //
  }














  if (isAirCleaning)
  {

  }
  else // !isAirCleaning
  {

  }
}







































void setupWifi()
{
  sendATCommandToESP8266("AT+CIPCLOSE=0");
  delay(1000);
  sendATCommandToESP8266("AT+CIPCLOSE=1");
  delay(1000);
  sendATCommandToESP8266("AT+CIPCLOSE=2");
  delay(1000);
  sendATCommandToESP8266("AT+CWMODE=2");
  delay(1000);
  sendATCommandToESP8266("AT+CWSAP=\"TESTmyAP1_206\",\"1402020899\",1,3");
  delay(2000);
  sendATCommandToESP8266("AT+CIPMUX=1");
  delay(1000);
  sendATCommandToESP8266("AT+CIPSERVER=1,50");
  delay(1000);
  sendATCommandToESP8266("AT+CIPSTO=7200");
  delay(1000);
}





/*
  _send       : AT command
  _recv       : 원하는 응답이 있는지 확인한다.
*/
inline int sendATCommandToESP8266(String _send)
{
  Serial2.println(_send);
  return 0;
}

bool isAwaitingResponseOfESP8266 = false;
String expectedResponse = "";
inline int sendATCommandToESP8266(String _send, String _recv)
{
  if (!isAwaitingResponseOfESP8266)
  {
    Serial2.println(_send);
    isAwaitingResponseOfESP8266 = true;
    expectedResponse = _recv;
    return 0;
  }
  else
  {
    // ERROR
    return 1;
  }
}




int32_t clk_sendStart = 0;
// bool isSendingDataToApp = false;
inline void sendDataToApp()
{
  if (dataToSend != "")
  {
    if (!isSendingDataToApp)
    {
      sendATCommandToESP8266("AT+CIPSEND=0," + String(dataToSend.length()));
      isSendingDataToApp = true;
      clk_sendStart = clk_main;
    }
    else if (clk_main - clk_sendStart > SEND_INTERVAL)
    {
      sendATCommandToESP8266(dataToSend);
      dataToSend = "";
    }
  }

  if (isSendingDataToApp && (clk_main - clk_sendStart >= SEND_INTERVAL + 300000))
  {
    isSendingDataToApp = false;
    clk_tmp = clk_main;
  }
}






inline void sendPMValuesToApp(int _pm10)
{
  // _pm10 값은 0에서부터 100을 넘어가며, 전송 시 앞에 0을 붙여 세 자리로 보낸다.
  static String tmpString = "";


  if (_pm10 < 10)
  {
    tmpString = "00";
  }
  else if (_pm10 < 100)
  {
    tmpString = "0";
  }
  else if (_pm10 >= 100)
  {
    tmpString = "";
  }


  dataToSend = "*" + tmpString + String(_pm10);
  sendDataToApp();
}





inline void sendCanvasCarLocationToApp()
{
  // 0에서 400으로 고정.

  canvasCarLocation[0] = canvasCarLocation[0] < 0 ? 0 : canvasCarLocation[0];
  canvasCarLocation[0] = canvasCarLocation[0] > 400 ? 400 : canvasCarLocation[0];

  canvasCarLocation[1] = canvasCarLocation[1] < 0 ? 0 : canvasCarLocation[1];
  canvasCarLocation[1] = canvasCarLocation[1] > 400 ? 400 : canvasCarLocation[1];





  static String tmpString1 = "";
  static String tmpString2 = "";


  
  
  if (canvasCarLocation[0] < 10)
  {
    tmpString1 = "00";
  }
  else if (canvasCarLocation[0] < 100)
  {
    tmpString1 = "0";
  }
  else if (canvasCarLocation[0] >= 100)
  {
    tmpString1 = "";
  }

  if (canvasCarLocation[1] < 10)
  {
    tmpString2 = "00";
  }
  else if (canvasCarLocation[1] < 100)
  {
    tmpString2 = "0";
  }
  else if (canvasCarLocation[1] >= 100)
  {
    tmpString2 = "";
  }




  dataToSend = "#" + tmpString1 + String(canvasCarLocation[0]) + tmpString2 + String(canvasCarLocation[1]);
  sendDataToApp();
}





inline void sendCanvasMapToApp(int _turnCount)
{
  // _pm10 값은 0에서부터 100을 넘어가며, 전송 시 앞에 0을 붙여 세 자리로 보낸다.
  static String tmpString1 = "";
  static String tmpString2 = "";



  if (canvasMap[_turnCount][0] < 10)
  {
    tmpString1 = "00";
  }
  else if (canvasMap[_turnCount][0] < 100)
  {
    tmpString1 = "0";
  }
  else if (canvasMap[_turnCount][0] >= 100)
  {
    tmpString1 = "";
  }

  if (canvasMap[_turnCount][1] < 10)
  {
    tmpString2 = "00";
  }
  else if (canvasMap[_turnCount][1] < 100)
  {
    tmpString2 = "0";
  }
  else if (canvasMap[_turnCount][1] >= 100)
  {
    tmpString2 = "";
  }




  dataToSend = "@" + tmpString1 + String(canvasMap[_turnCount][0]) + tmpString2 + String(canvasMap[_turnCount][1]);
  sendDataToApp();
}





uint32_t clk_recvStart = 0;
bool isRecvStartSaved = false;
bool isDataReceiving = false;
uint32_t RECV_INTERVAL = 50000;

void recvDataFromApp()
{
  static String dataReceived = "";
  static char tmpChar = 0;

  static String message = "";

  if (Serial2.available())
  {
    if (!isRecvStartSaved)
    {
      clk_recvStart = clk_main;
      isDataReceiving = true;
      isRecvStartSaved = true;
    }

    tmpChar = (char)Serial2.read();
    dataReceived += tmpChar;
  }

  if (isDataReceiving && (clk_main - clk_recvStart > RECV_INTERVAL))
  {
    isDataReceiving = false;
    isRecvStartSaved = false;

    message = dataReceived.substring(dataReceived.indexOf(':') + 1);
    dataReceived = "";
    dataReceivedHandler(message);
  }
}





void dataReceivedHandler(String _message)
{
  /*
    데이터 형식: +IPD,0,5:12345

    1.
    팬속도
    ->
    $1 $2 $3 $4

    2.
    무빙공청
    ->
    속도%시간

    3.
    고정공청
    (회전지점-1)^시간

    4.
    명령취소
    &
  */
  // Serial.println(_message);

  static String tmpString1 = "";
  static String tmpString2 = "";
  static int tmpIndex = 0;

  // 팬속도
  if (_message.indexOf('$') != -1)
  {
    // analyze data
    tmpString1 = _message.substring(_message.indexOf('$') + 1);

    // handler
    touchCondition = (int)tmpString1.toInt() - 1;
  }

  // 무빙공청
  else if (_message.indexOf('^') != -1)
  {
    // analyze data
    // 속도: [10%, 100%] -> [1, 10]
    // 시간: [1시간, 10시간] -> [1, 10]       // 단위는 자유롭게.
    tmpIndex = _message.indexOf('^');
    tmpString1 = _message.substring(0, tmpIndex);        // 차체 속도
    tmpString2 = _message.substring(tmpIndex + 1);       // 청소 시간

    // handler
    appCommand = MOVING;
    double ratio = (double)tmpString1.toInt() / 10.;
    // TRIG_INTERVAL *= ratio;
    // stepperSpeed1 *= ratio;
    // stepperSpeed2 *= ratio;
    TRIG_INTERVAL = (uint32_t)((double)TRIG_INTERVAL * ratio);
    stepperSpeed1 = (uint32_t)((double)stepperSpeed1 * ratio);
    stepperSpeed2 = (uint32_t)((double)stepperSpeed2 * ratio);
    updateStepDelays();
    clk_cleanStart = clk_main;
    cmd_duration = (uint32_t)tmpString2.toInt() * 3000000;
    startFan();
    startMove();
  }

  // 고정공청
  else if (_message.indexOf('%') != -1)
  {
    // analyze data
    // 회전 지점: 원점이 1, [1, n];
    // 시간: [1시간, 10시간] -> [1, 10]       // 단위는 자유롭게.
    tmpIndex = _message.indexOf('%');
    tmpString1 = _message.substring(0, tmpIndex);        // 청소 지점
    tmpString2 = _message.substring(tmpIndex + 1);       // 청소 시간

    // handler
    appCommand = FIXED;
    cmd_turnCount = tmpString1.toInt() - 1;
    cmd_location[0] = canvasMap[cmd_turnCount][0];
    cmd_location[1] = canvasMap[cmd_turnCount][1];
    cmd_duration = (uint32_t)tmpString2.toInt() * 3000000;
    startMove();


    // // 받을 데이터
    // appCommand = FIXED;
    // cmd_turnCount = 2;
    // cmd_duration = 3000000;
    // cmd_touchCondition = FAN_HIGH;
    
    // // 실행할 명령
    // touchCondition = cmd_touchCondition;
    // cmd_location[0] = canvasMap[cmd_turnCount][0];
    // cmd_location[1] = canvasMap[cmd_turnCount][1];
    // startMove();
  }

  // 명령취소
  else if (_message.indexOf('&') != -1)
  {
    appCommand = NO_COMMAND;
    TRIG_INTERVAL = 100000;
    stepperSpeed1 = 3.;
    stepperSpeed2 = 3.;
    updateStepDelays();
    stopFan();
    startMove();
  }

  // app에서 캔버스맵을 보내달래.
  else if (_message.indexOf("((") != -1)
  {
    bFlag = true;
  }

  



  
}



































void init_ultrasonic()
{
  // trigPin : digital output
  pinMode(TRIG[0], OUTPUT);
  pinMode(TRIG[1], OUTPUT);
  pinMode(TRIG[2], OUTPUT);
  pinMode(TRIG[3], OUTPUT);


  // echoPin : digital input
  pinMode(ECHO1, INPUT);
  pinMode(ECHO2, INPUT);



  // reset output
  digitalWrite(TRIG[0], LOW);
  digitalWrite(TRIG[1], LOW);
  digitalWrite(TRIG[2], LOW);
  digitalWrite(TRIG[3], LOW);



  /*
    echoPin : interrupt input
    TRIG[0, 2] -> ECHO1
    TRIG[1, 3] -> ECHO2
  */
  attachInterrupt(digitalPinToInterrupt(ECHO1), echoISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ECHO2), echoISR2, CHANGE);
}





void init_stepper()
{
  // stepper : all digital output
  pinMode(ENABLE1, OUTPUT);
  pinMode(STEP1, OUTPUT);
  pinMode(DIRECTION1, OUTPUT);

  pinMode(ENABLE2, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIRECTION2, OUTPUT);



  // reset output
  digitalWrite(ENABLE1, LOW);
  digitalWrite(STEP1, LOW);
  digitalWrite(DIRECTION1, LOW);

  digitalWrite(ENABLE2, LOW);
  digitalWrite(STEP2, LOW);
  digitalWrite(DIRECTION2, LOW);
}





void init_IR()
{
  // IR : digital input
  pinMode(originDetector1, INPUT);
  pinMode(originDetector2, INPUT);
}





void init_fan()
{
  pinMode(fanEna, OUTPUT);
  pinMode(fanDir1, OUTPUT);
  pinMode(fanDir2, OUTPUT);

  fanSpeed = 0;
  analogWrite(fanEna, fanSpeed);
  digitalWrite(fanDir1, HIGH);
  digitalWrite(fanDir2, LOW);
}

void init_touchSwitch()
{
  pinMode(touchSwitch[0], INPUT);
  pinMode(touchSwitch[1], INPUT);
  pinMode(touchSwitch[2], INPUT);
  pinMode(touchSwitch[3], INPUT);
}





void init_timer()
{
  cli();
  // stepper(1, 2)  ->  timer(3, 4)
  // timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = ocr3a_val; // CTC 모드 매칭 값 설정
  TCCR3B |= (1 << WGM32); // CTC 모드
  TCCR3B |= (1 << CS31);        // prescaler : 8
  TIMSK3 |= (1 << OCIE3A); // 출력 비교 인터럽트 허용
  // timer4
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4 = 0;
  OCR4A = ocr4a_val; // CTC 모드 매칭 값 설정
  TCCR4B |= (1 << WGM42); // CTC 모드
  TCCR4B |= (1 << CS41);        // prescaler : 8
  TIMSK4 |= (1 << OCIE4A); // 출력 비교 인터럽트 허용
  sei();
}





































inline void trig()
{
  // 0 ~ 3 반복
  index_ultrasonic = (++index_ultrasonic < 4) ? index_ultrasonic : 0;



  // 초음파 트리거
  digitalWrite(TRIG[index_ultrasonic], HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG[index_ultrasonic], LOW);



  // dbg
  // Serial.println(String(dr1[0]) + "    " + String(dr2[0]) + "    " + String(df1[0]) + "    " + String(df2[0]));
}








































inline void updateOriginCounters()
{
  // 첫 번째 센서 값 읽기 및 카운터 업데이트
  if (digitalRead(originDetector1) == HIGH)
    originDetectCounter1 = min(originDetectCounter1 + 1, 10); // 감지되면 카운터 증가, 최대값 10
  else
    originDetectCounter1 = max(originDetectCounter1 - 1, 0); // 감지되지 않으면 카운터 감소, 최소값 0

  // 두 번째 센서 값 읽기 및 카운터 업데이트
  if (digitalRead(originDetector2) == HIGH)
    originDetectCounter2 = min(originDetectCounter2 + 1, 10); // 감지되면 카운터 증가, 최대값 10
  else
    originDetectCounter2 = max(originDetectCounter2 - 1, 0); // 감지되지 않으면 카운터 감소, 최소값 0
}





inline void initOriginCounters()
{
  originDetectCounter1 = 0;
  originDetectCounter2 = 0;
}




































inline void readPMS7003Data()
{
  static int index = 0;
  static byte c = 0;

  while (Serial1.available() > 0)
  {
    c = Serial1.read();

    // 2 bytes index(0, 1) = 0x424d  ->  PMS7003 dataPacket's header
    // 통신 오류로 데이터가 밀리면, index(0, 1)에서 header를 기다린다.
    if (index == 0 && c != 0x42)
    {
      continue;
    }
    if (index == 1 && c != 0x4d)
    {
      index = 0;
      continue;
    }

    PMS7003Buffer[index++] = c;

    if (index == PMS7003BufferLength)
    {
      PMS7003DataAvailable = true;
      index = 0;
    }
  }
}





inline void calculatePMValues()
{
  static int pm1_0, pm2_5, pm10;
  p_pm10 = &pm10;

  pm1_0 = (PMS7003Buffer[10] << 8) + PMS7003Buffer[11];
  pm2_5 = (PMS7003Buffer[12] << 8) + PMS7003Buffer[13];
  pm10 = (PMS7003Buffer[14] << 8) + PMS7003Buffer[15];
  isPMValuesUpdated = true;

  // // dbg_
  // Serial.print("PM1.0: ");
  // Serial.print(pm1_0);
  // Serial.print(" µg/m³, PM2.5: ");
  // Serial.print(pm2_5);
  // Serial.print(" µg/m³, PM10: ");
  // Serial.print(pm10);
  // Serial.println(" µg/m³");
  // // _dbg
}





inline void checkTouchSwitch()
{
  static int loopCount;
  for (loopCount = 0; loopCount < 4; ++loopCount)
  {
    if (digitalRead(touchSwitch[loopCount]) == HIGH)
    {
      touchCondition = loopCount;
      break;
    }
  }
}




inline void applyTouchSwitchToFanSpeed()
{
  switch (touchCondition)
  {
    case FAN_OFF:
      {
        setFanSpeed(0);
        break;
      }
    case FAN_LOW:
      {
        setFanSpeed(150);
        break;
      }
    case FAN_HIGH:
      {
        setFanSpeed(255);
        break;
      }
    case FAN_AUTO_FROM_PMVALUES:
      {
        if (isPMValuesUpdated && p_pm10 != NULL)
        {
          // fanSpeed의 범위는 FAN_MINSPEED부터 255까지이다.
          // pm10 값은 최대 FAN_MAXPM10VALUE로 고정하며, 범위는 0부터 FAN_MAXPM10VALUE까지이다.
          // fanSpeed는 pm10 값에 비례한다.
          static int pm10Value;
          pm10Value = *p_pm10;
          pm10Value = pm10Value > FAN_MAXPM10VALUE ? FAN_MAXPM10VALUE : pm10Value;
          fanSpeed = (uint8_t)((double)FAN_MINSPEED + (double)pm10Value * ((double)(255 - FAN_MINSPEED) / (double)FAN_MAXPM10VALUE));
          setFanSpeed(fanSpeed);

          isPMValuesUpdated = false;

          // // dbg
          // Serial.println("    " + String(fanSpeed) + "    " + String(pm10Value) + "    " + String(*p_pm10));
        }
      }
  }
}












































/*
  사용한 변수들은 모두 (double)ratio_stepperSpeed(1, 2)를 구하는 데에만 사용되기 때문에, 전부 double로 선언한다.
  차체의 위치 = 차체와 벽 사이의 거리 = dr1[0]
*/

// const long targetLine = 150;       // 벽에서 유지선까지의 거리.
// const double targetLineHalf = targetLine / 2.;
// targetLine과의 거리가 thresh 이상이면 최대 기울기로 접근하고, thresh 이하라면 거리에 비례하여 기울기가 변경된다.
const long slopeChangeThreshold = 70;

const long rightSensorsSpacing = 150;        // 우측 2센서의 물리적 간격.
const double maxSlope = 0.42 * (double)rightSensorsSpacing;

long dTL = 0;       // distanceToTargetLine
double targetSlope = 0.;
double currSlope = 0.;
double slopeDiff = 0.;       // (targetSlope - currSlope)

inline void moveStraight()
{
  // 목표 기울기 계산.
  dTL = (DR1 + DR2) / 2 - targetLine;

  if (slopeChangeThreshold <= dTL)
  {
    targetSlope = maxSlope;
  }
  else if ((-slopeChangeThreshold <= dTL) && (dTL < slopeChangeThreshold))
  {
    targetSlope = (double)dTL * maxSlope / (double)slopeChangeThreshold;
  }
  else if (dTL < -slopeChangeThreshold)
  {
    targetSlope = -maxSlope;
  }



  // 현재 기울기에서 목표 기울기로의 변화량 계산.
  currSlope = (double)DR1 - (double)DR2;

  // 최대 최소 지정.
  if (maxSlope <= currSlope)
  {
    currSlope = maxSlope;
  }
  else if (currSlope < -maxSlope)
  {
    currSlope = -maxSlope;
  }


  slopeDiff = targetSlope - currSlope;

  if (slopeDiff >= 0.)
  {
    ratio_stepperSpeed1 = 1.;
    ratio_stepperSpeed2 = 1. - slopeDiff * 0.7 / (2. * maxSlope);
  }
  else
  {
    ratio_stepperSpeed1 = 1. + slopeDiff * 0.7 / (2. * maxSlope);
    ratio_stepperSpeed2 = 1.;
  }



  updateStepDelays();


  // Serial.println(String(dTL) + "    " + String(targetSlope) + "\n" + String(dr1[0]) + "    " + String(dr2[0]) + "    " + String(currSlope) + "    " + String(slopeDiff) + "\n" + String(ratio_stepperSpeed1) + "    " + String(ratio_stepperSpeed2));
  // Serial.println(String(dr1[0]) + "    " + String(dr2[0]) + "            " + String(ratio_stepperSpeed1) + "    " + String(ratio_stepperSpeed2) + "\n");
  // Serial.println(String(dTL) + "    " + String(targetSlope) + "    " + String(currSlope) + "    " + String(slopeDiff) + "\n");
}






// 제자리 회전이므로, 둘 다 같은 값으로 하고 directionPin toggle. -> 회전 끝나면 다시 toggle. @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
inline void turnLeft()
{
  digitalWrite(DIRECTION1, HIGH);
  ratio_stepperSpeed1 = 1.;
  ratio_stepperSpeed2 = 1.;

  updateStepDelays();

  carAxis = !carAxis;
  carDir = ++carDir > 3 ? 0 : carDir;


  // dbg
  digitalWrite(RED, LOW);        // leftTurn: BLUE
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
}





inline void turnRight()
{
  digitalWrite(DIRECTION2, HIGH);
  ratio_stepperSpeed1 = 1.;
  ratio_stepperSpeed2 = 1.;

  updateStepDelays();

  carAxis = !carAxis;
  carDir = --carDir < 0 ? 3 : carDir;


  // dbg
  digitalWrite(RED, LOW);        // rightTurn: GREEN
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}





inline void updateStepDelays()
{
  stepDelay1 = (unsigned long)((1. / (stepperSpeed1 * ratio_stepperSpeed1)) * (double)constStepDelay);
  stepDelay2 = (unsigned long)((1. / (stepperSpeed2 * ratio_stepperSpeed2)) * (double)constStepDelay);

  ocr3a_val = (uint16_t)(((double)stepDelay1 / 0.5) - 1.);
  ocr4a_val = (uint16_t)(((double)stepDelay2 / 0.5) - 1.);

  cli();
  OCR3A = ocr3a_val;
  OCR4A = ocr4a_val;
  sei();
}





inline int returnDir(int _carDir)
{
  int returnValue = 0;
  switch (_carDir)
  {
    case 0:
      returnValue = +1;
      break;
    case 1:
      returnValue = +1;
      break;
    case 2:
      returnValue = -1;
      break;
    case 3:
      returnValue = -1;
      break;
  }
  return returnValue;
}





inline void convertRawToCanvas()
{
  rawCarLocation[0] = 0;
  rawCarLocation[1] = 0;


  // 아래는 좌표값에서 음수를 없애고 canvas로 축소시키는 과정이다.
  int iLoop;



  if (!hasCanvasMap)
  {
    // canvasMap 초기화
    for (iLoop = 0; iLoop < mapIndexMax; ++iLoop)
    {
      canvasMap[iLoop][0] = 0;
      canvasMap[iLoop][1] = 0;
    }


    // rawMap에서 x_min, y_min을 구한다.
    for (iLoop = 0; iLoop <= turnCount; ++iLoop)
    {
      x_min_rawMap = min(x_min_rawMap, rawMap[iLoop][0]);
      y_min_rawMap = min(y_min_rawMap, rawMap[iLoop][1]);
    }


    // 모든 좌표에 x_min, y_min 절대값을 더하여 음수를 없앤다.
    for (iLoop = 0; iLoop <= turnCount; ++iLoop)
    {
      rawMap[iLoop][0] += (-x_min_rawMap);
      rawMap[iLoop][1] += (-y_min_rawMap);
    }


    // 계산된 rawMap에서 x_max, y_max를 구한다.
    int32_t x_max = 0, y_max = 0;
    for (iLoop = 0; iLoop <= turnCount; ++iLoop)
    {
      x_max = max(x_max, rawMap[iLoop][0]);
      y_max = max(y_max, rawMap[iLoop][1]);
    }


    // cx_max/rx_max, cy_max/ry_max 둘을 비교하여 값이 더 작은(축소 비율이 더 큰) 값을 ratio_stepToPixel 값으로 한다.
    // 오차 표기를 위해 캔버스 외곽 30pixel을 남긴다.
    double x_ratio = (double)(x_max_canvas - 80) / (double)x_max;
    double y_ratio = (double)(y_max_canvas - 80) / (double)y_max;
    ratio_stepToPixel = x_ratio < y_ratio ? x_ratio : y_ratio;


    // rawMap에 축소 비율인 ratio_stepToPixel을 곱하여 canvasMap을 만든다.
    for (int iLoop = 0; iLoop <= turnCount; ++iLoop)
    {
      canvasMap[iLoop][0] = (int32_t)((double)rawMap[iLoop][0] * ratio_stepToPixel) + 40;
      canvasMap[iLoop][1] = (int32_t)((double)rawMap[iLoop][1] * ratio_stepToPixel) + 40;
    }
  }


  // rawMap 초기화
  for (iLoop = 0; iLoop < mapIndexMax; ++iLoop)
  {
    rawMap[iLoop][0] = 0;
    rawMap[iLoop][1] = 0;
  }
}






































void stopMove()
{
  if (isMoving)
  {
    cli();
    // disable timer interrupt
    TIMSK3 &= ~(1 << OCIE3A);
    TIMSK4 &= ~(1 << OCIE4A);
    sei();

    // enable stepper driver
    digitalWrite(ENABLE1, HIGH);
    digitalWrite(ENABLE2, HIGH);

    isMoving = false;
  }
}

void startMove()
{
  if (!isMoving)
  {
    cli();
    // reset timer compare value to max
    ocr3a_val = 5000;
    ocr4a_val = 5000;
    sei();

    DR1 = 150;
    DR2 = 150;

    // enable timer interrupt
    init_timer();

    // enable stepper driver
    digitalWrite(ENABLE1, LOW);
    digitalWrite(ENABLE2, LOW);

    isMoving = true;
  }
}










































void echoISR1()
{
  if (digitalRead(ECHO1) == HIGH)
  {
    echoStart1 = micros();
  }
  else
  {
    echoEnd1 = micros();

    distance[index_ultrasonic][4] = distance[index_ultrasonic][3];
    distance[index_ultrasonic][3] = distance[index_ultrasonic][2];
    distance[index_ultrasonic][2] = distance[index_ultrasonic][1];
    distance[index_ultrasonic][1] = distance[index_ultrasonic][0];

    distance[index_ultrasonic][0] = (long)((double)(echoEnd1 - echoStart1) * 0.343 / 2.);



    switch (index_ultrasonic)
    {
      case 0:
        {
          if (dr1[0] < DR1 + rightTurnThreshold)
          {
            if (-rightSensorsNoiseThreshold < (dr1[0] - dr1[1]) && (dr1[0] - dr1[1]) < rightSensorsNoiseThreshold)
            {
              DR1 = dr1[0];
            }
          }
          break;
        }
      case 2:
        {
          if (isTurning)
          {
            distance[index_ultrasonic][0] = 99999;
          }

          // if (df1[0] < DF1 + frontSensorsNoiseThreshold)
          // {
          //   if (-frontSensorsNoiseThreshold < (df1[0] - df1[1]) && (df1[0] - df1[1]) < frontSensorsNoiseThreshold)
          //   {
          //     DF1 = df1[0];
          //   }
          // }
        }
    }
  }
}





void echoISR2()
{
  if (digitalRead(ECHO2) == HIGH)
  {
    echoStart2 = micros();
  }
  else
  {
    echoEnd2 = micros();

    distance[index_ultrasonic][4] = distance[index_ultrasonic][3];
    distance[index_ultrasonic][3] = distance[index_ultrasonic][2];
    distance[index_ultrasonic][2] = distance[index_ultrasonic][1];
    distance[index_ultrasonic][1] = distance[index_ultrasonic][0];

    distance[index_ultrasonic][0] = (long)((double)(echoEnd2 - echoStart2) * 0.343 / 2.);



    switch (index_ultrasonic)
    {
      case 1:
        {
          // noise filtering
          if (dr2[0] < DR2 + rightTurnThreshold)
          {
            if (-rightSensorsNoiseThreshold < (dr2[0] - dr2[1]) && (dr2[0] - dr2[1]) < rightSensorsNoiseThreshold)
            {
              DR2 = dr2[0];
            }
          }

          isRightSensorsUpdated = true;
          break;
        }
      case 3:
        {
          if (isTurning)
          {
            distance[index_ultrasonic][0] = 99999;
          }

          // // noise filtering
          // if (df2[0] < DF2 + frontSensorsNoiseThreshold)
          // {
          //   if (-frontSensorsNoiseThreshold < (df2[0] - df2[1]) && (df2[0] - df2[1]) < frontSensorsNoiseThreshold)
          //   {
          //     DF2 = df2[0];
          //   }
          // }

          isFrontSensorsUpdated = true;
        }
    }
  }
}





// // timer interrupt enable에 ISR 정의는 필수이다. analogWrite()로 timer1을 설정할 것이므로, 비워둔다.
// ISR(TIMER1_COMPA_vect) {}  ->  HW PWM은 timer 설정만 하고, timer interrupt를 사용하지 않는다. HW PWM을 위한 timer 설정이 있나보다.
// 대신 flag로 제어하자.

ISR(TIMER3_COMPA_vect)
{
  digitalWrite(STEP1, HIGH);
  digitalWrite(STEP1, LOW);

  ++stepCount;
}

ISR(TIMER4_COMPA_vect)
{
  digitalWrite(STEP2, HIGH);
  digitalWrite(STEP2, LOW);
}









// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline