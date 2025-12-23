// deviceName: AIRCONDITIONER





// Wi-Fi
#include <my_WIFI_client.hpp>

// AltSoftSerial espSerial; // (RX, TX) = (8, 9) 고정





// IRsend
#include <my_IRsignal.h>

#define PIN_IRSEND 3        // fixed

IRstatus iStatus = { 18, };
unsigned short button = 0x0;





// soundSensor
#define PIN_SOUNDSENSOR A5










void setup()
{
  // Init deviceName for send to the server.
  deviceName = "#AIRCONDITIONER";


  // Wi-Fi
  Serial.begin(9600);       // dbg
  espSerial.begin(9600); // SoftwareSerial
  

  // IRsend
  IrSender.begin(DISABLE_LED_FEEDBACK);


  // soundSensor
  pinMode(PIN_SOUNDSENSOR, INPUT);
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
  _data       : 명령

  // 명령에 따라 기기를 조작한다.
*/
void ESP_handler(String _data)
{
  if (_data.indexOf('$') != -1)
  {
    String tmp = String(iStatus.power)
    + String(iStatus.temp)
    + String(iStatus.mode)
    + String(iStatus.fanSpeed)
    + String(iStatus.swing)
    + String(iStatus.turbo)
    + String(iStatus.eco)
    + String(iStatus.offTimer);
    ESP_SendData("$" + tmp, 0);
  }
  else
  {
    IRsend(iStatus, button, _data[0]);
    startTime = millis();

    /*
      서버측에서 IR 받고, 소리를 내.
      오래 안 걸리니까,
      바로 소리 받자.

      일정 시간 이상 소리가 안 오면,
      else -> fail
    */

    while ((millis() - startTime) <= 500)
    {
      if (analogRead(PIN_SOUNDSENSOR) >= 1000)
      {
        // server에게 작동되었음을 알려주자.
        ESP_SendData(OPERATION_SUCCESS, 0);
        
        return;
      }
      delay(20);
    }
    // 시간경과해서 실패로 간주.
    ESP_SendData(OPERATION_FAILURE, 0);
    // 예외처리가 없는 이유는, 아니 없어야 한다.
    // 우리집 에어컨은, IR송신 실패해도, 리모컨의 status는 바뀜.
  }
}







// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline