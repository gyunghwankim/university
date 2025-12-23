// deviceName: BLINDS





// Wi-Fi
#include <my_WIFI_client.hpp>

// AltSoftSerial espSerial; // (RX, TX) = (8, 9) 고정





// servo motor
#include <ServoTimer2.h>

ServoTimer2 my_servo;

#define PIN_SERVOMOTOR 3

#define CYCLE_PER_30ANGLES 1900





void setup()
{
  // Init deviceName for send to the server.
  deviceName = "#BLINDS";


  // Wi-Fi
  Serial.begin(9600);       // dbg
  espSerial.begin(9600); // SoftwareSerial


  // servo motor
  my_servo.attach(PIN_SERVOMOTOR);
  my_servo.write(1500);        // init
}



void loop()
{
  // // 통신이 가능하도록 초기 세팅
  ESPconnect_client();



  // // // 통신 끊고 초기 상태로.
  // // ESPdisconnect_client();



  while (1)
  {
    ESP_RecvData_client();
  }





  // // 직접 통신
  // while (1)
  // {
  //   if (espSerial.available())
  //   {
  //     Serial.write(espSerial.read());
  //   }
  //   if (Serial.available())
  //   {
  //     espSerial.write(Serial.read());
  //   }
  // }
}












































int isNonDataTransmission;
String data;
/*
  < client >
  데이터수신이면 0, 아니면 1을 반환한다.
*/
int ESP_RecvData_client(void)
{
  if (espSerial.available())        // if문으로 확인만 하고, lineFeed('\n')까지만 읽고 처리한다. 반드시 딜레이가 없어야 한다. -> 반복
  {
    tmpString = espSerial.readStringUntil('\n');

    if (tmpString.startsWith("+IPD"))
    {
      // 수신 형태 -> client: +IPD,<데이터길이>:<데이터>    server: +IPD,<채널ID>,<데이터길이>:<데이터>
      // str.indexOf()를 활용해서 (채널ID와) 데이터를 추출하고, 이를 handler에게 넘긴다.
      data = tmpString.substring(tmpString.indexOf(':') + 1);

      isNonDataTransmission = 0;
    }
    else
    {
      // 데이터 수신 이외의 메세지.
      isNonDataTransmission = 1;
    }

    // dbg
    Serial.println(tmpString);


    if (!isNonDataTransmission)
    {
      ESP_handler(data);
    }
    else
    {
      // 특수커맨드나 데이터 수신 이외의 메세지들은 여기서 처리한다.
    }


    return isNonDataTransmission;
  }
}













































/*
  < client >
  _data       : 움직일 각도.

  // 명령에 따라 기기를 조작한다.
*/
/*
  < servo motor >

  헤더의 일부분
  ->
  #define MIN_PULSE_WIDTH       750        // the shortest pulse sent to a servo  
  #define MAX_PULSE_WIDTH      2250        // the longest pulse sent to a servo 
  #define DEFAULT_PULSE_WIDTH  1500        // default pulse width when servo is attached
  ->
  angles(0, 90, 180) == microseconds(750, 2250, 1500);



  750, 2250으로 최대 속도만 낸다.

  millis로 시간 체크.

  실제로 설치했을 때
  ->
  750 = 시계방향 = (+)
  2250 = 반시계방향 = (-)

  (초기 위치) = 0

  (현재 위치) = (이전 위치) + (작동 시간) * (시계/반시계 회전에 따라 (+/-))

  위치 (+/-) 최대값 존재.



  10초에 12바퀴.
  지름: 31.1mm, 둘레: 97.7mm
  ->
  1초당 1.2바퀴 = 117.24
  ->
  200mm 이동할 때 약 1.71s

  실제로 블라인드 조절할 때 범위
  ->
  position[-200mm, 200mm]
  ->
  velocity[-1.71s, 1.71s]
*/
int currLevel = 0;
int operatingTime_ms;
// unsigned long startTime;
void ESP_handler(String _data)
{
  int flag_updown;

  // 블라인드 각도 : _data{ -3, -2, -1, 0, 1, 2, 3 } = angles{ -90, -60, -30, 0, 30, 60, 90 }
  // 연산을 위해서 정수로 변환한다.
  int _level = _data.toInt();

  if (_level > 3)
  {
    _level = 3;
  }
  else if (_level < -3)
  {
    _level = -3;
  }



  if (currLevel >= _level)
  {
    operatingTime_ms = (currLevel - _level) * CYCLE_PER_30ANGLES;
    flag_updown = 0;
  }
  else if (currLevel < _level)
  {
    operatingTime_ms = (_level - currLevel) * CYCLE_PER_30ANGLES;
    flag_updown = 1;
  }



  if (flag_updown == 1)
  {
    startTime = millis();
    while ((millis() - startTime) < operatingTime_ms)
    {
      my_servo.write(750);
    }
    my_servo.write(1500);
  }
  else if (flag_updown == 0)
  {
    startTime = millis();
    while ((millis() - startTime) < operatingTime_ms)
    {
      my_servo.write(2250);
    }
    my_servo.write(1500);
  }



  currLevel = _level;

  

  // /*
  //   위치 확인 불가능.
  //   무조건 성공이라 가정한다.
  // */
  ESP_SendData(OPERATION_SUCCESS, 0);
}






// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline