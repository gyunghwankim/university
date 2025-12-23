// AT+CWMODE=2

// AT+CWSAP="TESTmyAP1_206","1402020899",1,3

// AT+CIPMUX=1

// AT+CIPSERVER=1,50

// AT+CIPSTO=7200






#include "WiFiCommunicator.hpp"






// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ endline












uint32_t clk_main = 0;

WiFiCommunicator wifiCommunicator(Serial1, clk_main);


void setup()
{
    Serial.begin(9600);

    wifiCommunicator.begin(9600);

    wifiCommunicator.wifiSetup();
}



void loop()
{
    clk_main = micros();

    // wifiCommunicator.bufferCommand("send command");

    wifiCommunicator.recvData();

    wifiCommunicator.sendCommandToApp();
}

    // AT_RESPONSE부터 확인하자.
        // if (Serial2.available())
        // {
        //     Serial1.write(Serial2.read());
        // }
    // if (Serial.available())
    // {
    //     Serial1.write(Serial.read());
    // }
    // if (Serial1.available())
    // {
    //     Serial.write(Serial1.read());
    //     wifiCommunicator.recvData();
    // }



    // /*
    //     PC입력 -> Serial2 입력. (2에게 명령.)
    // */
    // if (Serial.available())
    // {
    //     static char c1;
    //     c1 = Serial.read();

    //     if (c1 == '/')
    //     {
    //         static bool flag1 = true;
    //         if (flag1)
    //         {
    //             wifiCommunicator.bufferCommand("1");
    //             wifiCommunicator.bufferCommand("12");
    //             wifiCommunicator.bufferCommand("123");
    //             wifiCommunicator.bufferCommand("1234");
    //             wifiCommunicator.bufferCommand("12345");
    //             flag1 = false;
    //         }
    //     }
    //     else
    //     {
    //         Serial.write(c1);
    //         Serial2.write(c1);
    //     }
    // }

/*
    디버그 코드로 무시하려고 했지만, 조건을 다시 짜야 한다.
    데이터 형식은 디버그 포함하여 세 가지
    ->
    1. ~~\r\n (dbg)
    2. \r\n~~\r\n (AT_RESPONSE)
    3. \r\n+IPD~~ (APP_COMMAND)
*/

/*
    Wi-Fi module - ESP8266의 현재 firmware는, 수신받은 byte를 즉각 수신자에게 전송한다.
*/






























/*
    ATcommand 입력 딜레이가 존재해.


    송신 - ATcommand
    delay
    수신 - OK
    delay


    delay를 줄이거나 없앨 수 있는가?
    ->
    1.
    ATcommandSet을 사용하는 현재 firmware로는 불가능하다.
    Wi-Fi module - ESP8266의 firmware를 변경해야 한다.

    firmware 변경 없이 어떻게 해야 하는가?


    ->
    송신코드는 동시에 실행할 수 없으며, delay가 있다.


    ->
    주기적인 송신코드들은 공통 clock을 공유하고, 개별 실행 시각이 있으며, 서로 delay 이상의 시간 간격이 있다.

    비주기적인 송신코드의 실행은 주기적인 송신코드의 실행을 방해하는데, 어떻게 해결하는가?


    ->
    우선 다시 확인할 것들
    1.
    주기 송신의 시간 간격은 delay 이상이며, 여유 시간이 있다.
    1.
    주기 송신의 실행 시각은 정해져 있다.

    비주기 송신이 난입하면, 주기 송신의 정해진 실행 시각에 실행하지 못한다.


    ->
    주기 송신의 실행 시각은 지연된다.

    주기 송신의 실행 시각을 다시 스케줄링하여 원래 주기로 돌아가도록 조정한다.

    비주기 송신의 난입 실행도 더욱 어려워진다.


    ->
    방법
    1.
    지연이 되면 원래대로 조정하지 않고, 지연된 주기 그대로 실행한다.
    1.
    각 주기 송신의 실행 시각에 전체 지연 시간을 더하고, 앞으로의 주기 송신을 일찍 실행하는 것으로 지연 시간을 조금씩 소비한다.
    1.
    송신 조건 : !(송신중)
    송신 코드 실행  ->  queue의 rear(back)에 enqueue한다.
    송신 함수는 queue의 front에서 dequeue한다.
    원소는 5개 이상 쌓이지 않으므로, static array를 사용한다.





*/
















































  /*
시리얼 모니터 : Both NL & CR, 115200baud -> 9600baud

AT command :
  PC와 모뎀간의 통신에 사용하는 아주 간단한 commandSet이다.
  TCP/IP server/client 지원하고, ATcommand만으로 통신이 가능하다.
  ESP-01의 FW version이 높으면 더 넓은 제어가 가능하다.
  이외의 protocol(MQTT, HTTP(web), ...)을 사용하려면, ESP-01 보드의 firmware programming이 필수적이다.



ㅡ 명령어 유형 4가지 :        // 없는 게 너무 많아. test는 CWMODE 말곤 하나도 없어. manual/parameter는 github에서 확인하자.
  <type>        <command format>        <description>
  test          AT+<x>=?                Query the set command or internal parameters and its range values.
  query         AT+<x>?                 Returns the current value of the parameter.
  set           AT+<x>=<...>            Set the value of user-defined parameters in commands and run.



ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ 기본 설정 및 network(AP/STATION mode) 설정.

ㅡ AT+RST       // 단순 재부팅이며, 어떠한 설정도 초기값으로 변경되지 않는다.

ㅡ AT+RESTORE       // 펌웨어 버전을 제외하고 모듈의 모든 설정을 초기화한다. (baudrate 포함 -> 115200)


ㅡ AT+GMR        // Check version information


ㅡ AT+UART_DEF=9600,8,1,0,0     // 모듈의 통신속도를 9600으로 변경.

ㅡ AT+CWMODE=?        // :: +CWMODE:(1-3)        // 1. (STA)station mode (접속)    2. (AP)soft AP mode (로컬네트워크 형성)   3. STA+AP mode (접속도 하고, 네크워크도 열고,)

ㅡ AT+CWMODE?       // 현재 명령어 모드 확인.

ㅡ AT+CWMODE=1        // 명령어 모드를 1(STA)모드로 변경.

ㅡ AT+CWLAP       // Browse accessible local network(Wi-Fi).    접속 가능한 AP 탐색

    e.g.
    +CWLAP:(3,"toxx",-80,"00:00:00:00:00:00",0,0,0,0,0,0,0)
    +CWLAP:(3,"LAAA",-35,"00:00:00:00:00:00",0,0,0,0,0,0,0)
    OK

ㅡ AT+CWLAP="SSID"        // 특정 SSID(AP name) 조회.    대소문자 구별.

    e.g.
    AT+CWLAP="Laaa"

    ->  OK        // 조회 결과 없음.

    AT+CWLAP="LAAA"

    ->  +CWLAP:(3,"LAAA",-34,"00:00:00:00:00:00",0,0,0,0,0,0,0)
        OK        // 일치하는 AP 찾았음.

ㅡ AT+CWJAP="SSID","PASSWORD"       // Wi-Fi 접속

ㅡ AT+CWJAP?        // 접속 중인 AP 확인        // IP, MAC, 보안모드(1/2/3/4), 수신감도 등.

ㅡ AT+CWQAP       // 접속 중인 AP 접속 해제



ㅡ AT+CWMODE=2        // soft AP mode

ㅡ AT+CWSAP?        // myAP 설정 확인

ㅡ AT+CWSAP="SSID","PW","채널ID","ecn(encryption method; 암호화 방법)"        // myAP 설정.       // 채널ID는 무시하고 1로 설정.
    ecn - 0: OPEN    1: WEP    2: WPA_PSK    3: WPA2_PSK    4: WPA_WPA2_PSK    5: WPA2_Enterprise (AT can NOT connect to WPA2_Enterprise AP for now.)
    ="TESTmyAP1_206","1402020899",1,3

ㅡ AT+CIFSR       // 자신의 STA/AP mode 각각의 IP, MAC, ... 확인

ㅡ AT+CWLIF       // myAP에 접속한 client들의 IP/MAC 확인



ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ unsorted

ㅡ AT+FSR       // 로컬 IP 주소를 반환한다.       // 어떻게 쓰는거

ㅡ AT+IPR=<통신 속도>       // ESP 모듈의 통신 속도를 설정한다.




ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ server/client 설정.

ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ common

ㅡ AT+CIPSTATUS        // 연결 상태 및 정보를 반환한다.       // 접속확인유무로 사용할 수 있겠어. -> 확실해.



ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ server

ㅡ AT+CIPMUX=<mode>        // 다중 연결 활성화    // mode: 0_단일  1_다중
    AT+CIPMUX?        // 체크.
    mode=1

ㅡ AT+CIPSERVER=<mode>,<portNumber>       // 지정된 포트번호로 서버를 활성화/비활성화       // 0_비활성화  1_활성화
    =1,10
    AT+CIPSERVER=0        // 서버 닫을 때 포트번호 없이. (어차피 열린 포트는 하나 뿐이니까.)

ㅡ AT+CIPCLOSE=<channel ID>       // 특정 client의 연결을 해제한다.       // server에 들어오는 순서대로 "0, CONNECTED", "1, CONNECTED", ... 여기서 숫자가 채널ID.

ㅡ AT+CIPSTO=<T_sec>        // client의 타임아웃을 설정한다.        // 데이터가 교류될 때마다 초기화된다.       // max=7200s=2h
    =7200



ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ client

ㅡ AT+CIPSTART=<protocol>,<원격 IP>,<원격 포트>        // server에 접속한다.
    ="TCP","192.168.4.1",10

ㅡ AT+CIPCLOSE        // server와의 연결을 해제한다.

ㅡ AT+CIPMODE=<mode>        // 전송 모드 설정.        // 0_정상(protocol)  1_투명(binary)       // 이건 데이터쪽으로 옮기자. -> 이게 주고받기 전 마지막 단계야. 이것도 그냥 여기에 포함해.



ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ server-client 데이터 주고받기.

ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ common

// 양측 모두 보내기 전에 <AT+CIPSTATUS>로 연결 확인하고 보내.

ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ server

ㅡ AT+CIPSEND=<channel ID>,<length(byte)>

ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ client

ㅡ AT+CIPSEND=<length(byte)>

  e.g.
  -> SEND <- (client의 채널ID는 0이라고 한다.)
      AT+CIPSEND=0,10       // server
      (or)
      AT+CIPSEND=10       // client
      
      OK
      > (input string and ENTER: 123412341234)        // busy s...는 무시하고, (연결상태 확인) + (최소딜레이 추가)만 넣으면 되겠다.
      Recv 10 bytes

      SEND OK

  -> RECV <-        // 10bytes 이상은 짤림.
      +IPD,0,10:1234123412        // server
      +IPD,10:1234123412        // client
  
ㅡ 전송 관련 규칙
  1. length 초과는 짤린다.
  2. length-3 이하부터는 한 번에 전송되지 않으며, 처음 전송에 lineFeed를 포함하여 데이터의 최대 길이는 length-2가 된다.
      전송 1: string1 + lineFeed
      전송 2: string2       // (string1 + string2) <= length-2














*/

















































/*
< 단축키 >

  preferences (기본 설정)
  Ctrl ,

  compile
  Ctrl R

  upload
  Ctrl U

  toggle bottom panel
  Ctrl J

  keyboard shortcuts
  Ctrl Alt ,

  toggle sketchbook view
  Ctrl Shift B






*/