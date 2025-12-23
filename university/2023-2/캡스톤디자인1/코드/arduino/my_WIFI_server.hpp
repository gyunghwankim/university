// deviceName: HUB(server)


#ifndef MY_WIFI_SERVER_HPP
#define MY_WIFI_SERVER_HPP


// Wi-Fi
#include <SoftwareSerial.h>

SoftwareSerial espSerial(8, 9); // RX, TX

const String AP_SSID = "TESTmyAP1";
const String AP_PW = "1402020899";
const String AP_IP = "192.168.4.1";

#define RETRY_INTERVAL 20000
#define COOLING 1000

#define OPERATION_SUCCESS "SUCCESS"
#define OPERATION_FAILURE "FAILURE"

/*
  deviceName = channelID
  client는 ESPconnect_server() 실행 후, '@' 문자와 함께 자신의 deviceName을 송신한다. (data = "@AIRCONDITIONER")
  server는 '@'를 확인하면, device maching을 한다.
*/
char APP;
char AIRCONDITIONER;
char LIGHTSWITCH;
char BLINDS;
// add device







void ESPconnect_server();
void ESPdisconnect_server();
int ESP_SendATCommand(String, String, bool);
int ESP_SendData(String, char, bool);
void __ERROR(String);

int ESP_RecvData_server();
void ESP_handler(String, char);







































void ESPconnect_server()
{
  // restart
  delay(2000);
  espSerial.println("AT+RST");
  delay(COOLING);


  // 통신 체크
  while (ESP_SendATCommand("AT", "", 1))        // dbg: OK로 바꿔.
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ AP

  // soft AP mode로 변경
  while (ESP_SendATCommand("AT+CWMODE=2", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // AP 정보 변경
  while (ESP_SendATCommand("AT+CWSAP=\"" + AP_SSID + "\",\"" + AP_PW + "\",1,3", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ server

  // enable multiple connections
  while (ESP_SendATCommand("AT+CIPMUX=1", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // server open - portNumber 10
  while (ESP_SendATCommand("AT+CIPSERVER=1,10", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // client-timeout 설정 : MAX=7200s=2h
  while (ESP_SendATCommand("AT+CIPSTO=7200", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  Serial.println("\n--- server ready ---\n");
}









































void ESPdisconnect_server()
{
  // 각 client에게 ESPdisconnect() 실행시키라는 특정 메세지를 보내자.

  // AT version:1.7.4.0  ->  최대 소켓 5 (채널ID = 0, 1, 2, 3, 4), AT+CIPCLOSE=5 : 모든 소켓을 닫는다.
  ESP_SendATCommand("AT+CIPCLOSE=5", "", 0);
  delay(COOLING);

  ESP_SendATCommand("AT+CIPSERVER=0", "", 0);
  delay(COOLING);

  ESP_SendATCommand("AT+CIPMUX=0", "", 0);
  delay(COOLING);

  ESP_SendATCommand("AT+CWMODE=1", "", 0);
  delay(COOLING);

  Serial.println("\n--- default ---\n");
}





































/*
  _send       : AT command
  _recv       : 원하는 응답이 있는지 확인한다.
  _showError  : 응답이 불일치하면 오류처리 및 1을 반환한다.
*/
String tmpString;
char tmpChar;
unsigned long startTime;
int ESP_SendATCommand(String _send, String _recv, bool _showError)
{
  // send ATcommand
  espSerial.println(_send);


  // Save ESP-response to stringType.
  tmpString = "";       // init
  if (_showError)
  {
    startTime = millis();
    while ((tmpString.indexOf("OK") == -1) && ((millis() - startTime) <= 30000))       // breakCondition: OK 확인되거나 30초 경과.
    {
      while (espSerial.available())
      {
        tmpChar = espSerial.read();       // tmpString += espSerial.read();    -> 증감연산자는, C++ 문자열의 buffer 할당/해제 관련 문제를 야기한다.
        tmpString += tmpChar;
      }
    }
  }
  else
  {
    while ((tmpString.indexOf("OK") == -1) && (tmpString.indexOf("ERROR") == -1))       // breakCondition: OK나 ERROR 확인.
    {
      while (espSerial.available())
      {
        tmpChar = espSerial.read();       // tmpString += espSerial.read();    -> 증감연산자는, C++ 문자열의 buffer 할당/해제 관련 문제를 야기한다.
        tmpString += tmpChar;
      }
    }
  }


  // dbg
  Serial.println(tmpString);


  // check
  if (tmpString.indexOf(_recv) != -1)       // string.memberFunction - indexOf(_str)  :  string에 _str이 있으면 index를 반환하고, 없으면 -1을 반환한다. (EOF 아님)
  {
    return 0;
  }
  else
  {
    if (_showError)
    {
      __ERROR(_send);
      // 오류처리 추가
    }
    return 1;
  }
}








































/*
  < server >
  _data       : client에게 보낼 메세지.
  _channelID  : 수신할 client의 채널ID(=deviceName)
  _showError  : 오류처리 유무
*/
int ESP_SendData(String _data, char _channelID, bool _showError)
{
  if (ESP_SendATCommand("AT+CIPSTATUS", String(_channelID) + ",\"TCP\"", 0) == 0)
  {
    ESP_SendATCommand("AT+CIPSEND=" + String(_channelID) + "," + String(_data.length()), "OK", 0);
    ESP_SendATCommand(_data, "SEND OK", 0);
    
    return 0;
  }
  else
  {
    if (_showError)
    {
      __ERROR("send: " + _data);
      // ERROR : 세 단계에 모두 flag 체크.
    }
    
    return 1;
  }
}











































// dbg
void __ERROR(String _send)
{
  Serial.println("--------------------------------------------------------------------\n---   " + _send + "   ---\n--------------------------------------------------------------------");
}









#endif        // MY_WIFI_SERVER_HPP