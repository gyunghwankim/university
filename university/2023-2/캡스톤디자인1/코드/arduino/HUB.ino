// deviceName: HUB(server)





// Wi-Fi
#include <my_WIFI_server.hpp>

// SoftwareSerial espSerial(8, 9); // RX, TX





// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ external devices ->
// IRrecv
#include <my_IRsignal.h>

#define PIN_IRRECV 3



// buzzer
#define PIN_BUZZER 5        // PWM
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ <- external devices













void setup()
{
  // Wi-Fi
  Serial.begin(9600);       // dbg
  espSerial.begin(9600);        // SoftwareSerial

  // APP = '0';       // dbg
  // AIRCONDITIONER = '2'; // dbg
  // LIGHTSWITCH = '3'; // dbg



  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ external devices ->
  // IRrecv
  IrReceiver.begin(PIN_IRRECV, false);



  // buzzer
  pinMode(PIN_BUZZER, OUTPUT);
  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ <- external devices
}











void loop()
{
  // 통신이 가능하도록 세팅
  ESPconnect_server();



  // // 통신 끊고 초기 상태로.
  // ESPdisconnect_server();



  while (1)
  {
    ESP_RecvData_server();



    // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ external devices ->
    device_AIRCONDITIONER();
    // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ <- external devices
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

  //   device_AIRCONDITIONER();
  // }
}






































int isNonDataTransmission;
char channelID;
String data;
/*
  < server >
  데이터 수신이면 0, 아니면 1을 반환한다.
*/
int ESP_RecvData_server()
{
  if (espSerial.available())        // if문으로 확인만 하고, lineFeed('\n')까지만 읽고 처리한다. 반드시 딜레이가 없어야 한다. -> 반복
  {
    tmpString = espSerial.readStringUntil('\n');

    if (tmpString.startsWith("+IPD"))
    {
      // 수신 형태 -> client: +IPD,<데이터길이>:<데이터>    server: +IPD,<채널ID>,<데이터길이>:<데이터>
      // str.indexOf()를 활용해서 (채널ID와) 데이터를 추출하고, 이를 handler에게 넘긴다.
      channelID = tmpString.charAt(tmpString.indexOf(',') + 1);
      data = tmpString.substring(tmpString.indexOf(':') + 1);

      isNonDataTransmission = 0;


      // device maching
      if (data.indexOf('#') != -1)
      {
        if (data == "#AIRCONDITIONER")
        {
          AIRCONDITIONER = channelID;
          Serial.println("channelID - AIRCONDITIONER = " + String(channelID));      // dbg
        }
        else if (data == "#LIGHTSWITCH")
        {
          LIGHTSWITCH = channelID;
          Serial.println("channelID - LIGHTSWITCH = " + String(channelID));     // dbg
        }
        else if (data == "#BLINDS")
        {
          BLINDS = channelID;
          Serial.println("channelID - BLINDS = " + String(channelID));     // dbg
        }
        else if (data == "#APP")
        {
          APP = channelID;
          Serial.println("channelID - APP = " + String(channelID));     // dbg
        }

        // 이걸 앱에게 보내주자. 형식은 어떻게 할까? 일단, 어떤기기냐라는 _data를 보내야 하고, 그 데이터의 채널ID도 보내야 해.

        return isNonDataTransmission;
      }
    }
    else
    {
      // 데이터 수신 이외의 메세지.
      isNonDataTransmission = 1;
    }


    if (!isNonDataTransmission)
    {
      ESP_handler(data, channelID);
    }
    else
    {
      // 특수커맨드나 데이터 수신 이외의 메세지들은 여기서 처리한다.
    }


    return isNonDataTransmission;
  }
}






































/*
  < server >
  _channelID  : deviceName
  _data       : data
*/
void ESP_handler(String _data, char _channelID)
{
  if (_channelID == APP)        // 데이터가 APP에서 왔다면,
  {
    if (_data.indexOf('@') != -1)
    {
      if (_data == "@getStatus_ID")
      {
        Serial.println("good: " + _data);       // dbg
        ESP_SendData("@" + String(AIRCONDITIONER) + String(LIGHTSWITCH) + String(BLINDS), APP, 0);       // add devices
      }
    }
    else if (_data.indexOf('$') != -1)
    {
      ESP_SendData(_data, AIRCONDITIONER, 0);
    }
    else
    {
      // APP에서 온 명령어 구조: <채널ID>:<message>  ->  채널ID와 message를 분리하고, client에게 전송한다.
      Serial.println(_data.substring(2) + "," + _data.charAt(0));       // dbg
      ESP_SendData(_data.substring(2), _data.charAt(0), 0);
    }
  }
  else
  {
    // SUCCESS or FAILURE
    ESP_SendData(_data, APP, 0);
  }
}












































// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ external devices
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

/*
  에어컨을 가져올 수는 없으니, 기기를 최대한 간단하게 구현한다.
  원래 허브의 역할이 아니며, 실제에 적용할 때는 없는 코드일 것이다.
  그러므로 반드시 기존의 코드와 분리시킨다. (리소스 신경 쓰지 말고, 그냥 지우면 끝나게 코딩.)
*/


/*
  AIRCONDITIONER
  -> IrReceiver + buzzer

  1.
  IRrecv -> loop()

  2.
  IRrecv -> (various conditions)true -> buzzer

  3.
  (various conditions)
  ->
  에어컨이 동작하지 않는 조건은, IRsignal이 뭉개졌을 때이므로, 모든 버튼에 동일한 protocol값과 address값을 확인한다.
    protocol = LG
    address = 0x88        // rawData = 0x088?????
*/

// #include <my_IRsignal.h>

// #define PIN_IRRECV 2
// #define PIN_BUZZER 3        // PWM

// void setup() {
//   Serial.begin(9600);
//   IrReceiver.begin(PIN_IRRECV, false);
//   pinMode(PIN_BUZZER, OUTPUT);
// }

// void loop() {
//   if (IrReceiver.decode())
//   {
//     if ((IrReceiver.decodedIRData.protocol == LG) && ((IrReceiver.decodedIRData.decodedRawData >> 20) == 0x88))
//     {
//       tone(PIN_BUZZER, 1000);
//       delay(200)
//       noTone(PIN_BUZZER);
//     }

//     IrReceiver.resume();
//   }
// }



/*
  에어컨
*/

void device_AIRCONDITIONER()
{
  if (IrReceiver.decode())
  {
    IrReceiver.printIRResultShort(&Serial);
    if ((IrReceiver.decodedIRData.protocol == LG) && ((IrReceiver.decodedIRData.decodedRawData >> 20) == 0x88))
    {
      tone(PIN_BUZZER, 1000);
      delay(200);
      noTone(PIN_BUZZER);
    }

    IrReceiver.resume();
  }
}







// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline