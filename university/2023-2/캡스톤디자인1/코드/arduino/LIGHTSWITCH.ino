// deviceName: LIGHTSWITCH





// Wi-Fi
#include <my_WIFI_client.hpp>

// AltSoftSerial espSerial; // (RX, TX) = (8, 9) 고정





// servo motor
#include <ServoTimer2.h>

ServoTimer2 my_servo;

#define PIN_SERVOMOTOR 3

#define DEGREE_MS 210





void setup()
{
  // Init deviceName for send to the server.
  deviceName = "#LIGHTSWITCH";


  // Wi-Fi
  Serial.begin(9600);       // dbg
  espSerial.begin(9600); // SoftwareSerial


  // servo motor
  my_servo.attach(PIN_SERVOMOTOR);
  my_servo.write(1690);        // init
}



void loop()
{
  // 통신이 가능하도록 초기 세팅
  ESPconnect_client();



  // // 통신 끊고 초기 상태로.
  // ESPdisconnect_client();



  while (1)
  {
    ESP_RecvData_client();
  }





  // 직접 통신
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
  _data       : 명령

  // 명령에 따라 기기를 조작한다.
*/
int forLoopCounter;
void ESP_handler(String _data)
{
  /*
    서보모터 제어.

    _data : 1_ON    0_OFF
  */
  
  if (_data == "1")
  {
  my_servo.write(2060);
  delay(300);
  my_servo.write(1690);
  delay(50);
  }
  else
  {
    my_servo.write(1320);
    delay(300);
    my_servo.write(1690);
    delay(50);
  }


  /*
    위치 확인 불가능.
    무조건 성공이라 가정한다.
  */
  ESP_SendData(OPERATION_SUCCESS, 0);
}







// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline