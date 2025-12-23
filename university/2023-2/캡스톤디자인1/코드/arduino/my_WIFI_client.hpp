#ifndef MY_WIFI_CLIENT_HPP
#define MY_WIFI_CLIENT_HPP


// Wi-Fi
#include <AltSoftSerial.h>

AltSoftSerial espSerial; // (RX, TX, unusable PWM pin) -> UNO(8, 9, 10), LEONARDO(13, 5, none)

const String AP_SSID = "TESTmyAP1";
const String AP_PW = "1402020899";
const String AP_IP = "192.168.4.1";

#define RETRY_INTERVAL 20000
#define COOLING 1000

#define OPERATION_SUCCESS "SUCCESS"
#define OPERATION_FAILURE "FAILURE"







/*
    공통.
*/
void ESPconnect_client();
void ESPdisconnect_client();
int ESP_SendATCommand(String, String, bool);
int ESP_SendData(String, bool);
void __ERROR(String);

/*
    기기별로 다름.
*/
//int ESP_RecvData_client();
//void ESP_handler(String);




































// dbg
String deviceName;

void ESPconnect_client()
{
  // restart
  delay(20000);
  espSerial.println("AT+RST");
  delay(COOLING);


  // 통신 체크
  while (ESP_SendATCommand("AT", "", 1))        // dbg: OK로 바꿔.
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ AP

  // station mode로 변경
  while (ESP_SendATCommand("AT+CWMODE=1", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // 특정 Wi-Fi 탐색
  while (ESP_SendATCommand("AT+CWLAP=\"" + AP_SSID + "\"", ",\"" + AP_SSID + "\",", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // 특정 Wi-Fi에 접속
  while (ESP_SendATCommand("AT+CWJAP=\"" + AP_SSID + "\",\"" + AP_PW + "\"", "WIFI GOT IP", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ server

  // server에 접속 : "TCP","AP_IP",10
  while (ESP_SendATCommand("AT+CIPSTART=\"TCP\",\"" + AP_IP + "\",10", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // 전송 모드 설정 : 0_정상(protocol)  1_투명(binary)
  while (ESP_SendATCommand("AT+CIPMODE=0", "OK", 1))
    delay(RETRY_INTERVAL);
  delay(COOLING);


  // ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ client->server identifying

  while (ESP_SendData(deviceName, 0))        // 이 client가 AIRCONDITIONER임을 server에게 알린다.
    delay(RETRY_INTERVAL);
  delay(COOLING);


  Serial.println("\n--- client ready ---\n");
}









































void ESPdisconnect_client()
{
  ESP_SendATCommand("AT+CIPCLOSE", "", 0);
  delay(COOLING);

  ESP_SendATCommand("AT+CWQAP", "", 0);
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
  < client >
  _data       : server에게 보낼 메세지
  _showError  : 오류처리 유무
*/
int ESP_SendData(String _data, bool _showError)
{
  if (ESP_SendATCommand("AT+CIPSTATUS", ",\"TCP\"", 0) == 0)
  {
    ESP_SendATCommand("AT+CIPSEND=" + String(_data.length()), "OK", 0);
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









#endif        // MY_WIFI_CLIENT_HPP