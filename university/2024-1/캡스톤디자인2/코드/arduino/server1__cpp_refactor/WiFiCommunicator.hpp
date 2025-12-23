#ifndef WIFI_COMMUNICATOR_HPP
#define WIFI_COMMUNICATOR_HPP

#include <Arduino.h>




/*
    bufferCommand()로 보낼 명령어를 입력하고, sendCommandToApp()으로 송신한다.
    recvData()로 수신한다.
*/
class WiFiCommunicator
{
public:
    WiFiCommunicator(HardwareSerial& _serial, uint32_t& _clkMicros);
    void begin(uint32_t _baudRate);
    void wifiSetup();
    void bufferCommand(String _command);
    // polling
    void sendCommandToApp();
    // polling
    void recvData();


private:
    void sendATCommandToESP8266(String _ATCommand);
    enum class RecvDataType : uint8_t {
        APP_COMMAND,
        AT_RESPONSE
    };
    void recvHandler(String _string, RecvDataType _recvType);
    
    
private:
    HardwareSerial& mSerial;
    String mSendBuffer[10];
    uint8_t mSendBufferSize = 0;
    String mRecvBuffer[10];
    uint8_t mRecvBufferSize = 0;
    const uint32_t& mClkMicros;
};



































WiFiCommunicator::WiFiCommunicator(HardwareSerial& _serial, uint32_t& _clkMicros)
    : mSerial(_serial)
    , mClkMicros(_clkMicros)
{
}


inline void WiFiCommunicator::begin(uint32_t _baudRate)
{
    mSerial.begin(_baudRate);
}


inline void WiFiCommunicator::wifiSetup()
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


inline void WiFiCommunicator::bufferCommand(String _command)
{
    if (mSendBufferSize < 10)
    {
        mSendBuffer[mSendBufferSize++] = _command;
    }
    else
    {
        // exception
    }
}


inline void WiFiCommunicator::sendCommandToApp()
{
    enum class SendingStage : uint8_t {
        STAGE1,
        STAGE2,
        STAGE3
    };
    static SendingStage sendingStage = SendingStage::STAGE1;

    static bool sending = false;

    static uint32_t clkATCommand, clkCIPSEND;

    static constexpr uint32_t INTERVAL_AT_COMMAND = 150000; // microsecond
    static constexpr uint32_t INTERVAL_CIPSEND = 50000; // microsecond
    // static constexpr uint32_t INTERVAL_TOTAL_SEND = INTERVAL_AT_COMMAND + INTERVAL_CIPSEND;


    switch (sendingStage)
    {
        case SendingStage::STAGE1:
        {
            if (mSendBufferSize && !sending)
            {
                sendATCommandToESP8266("AT+CIPSEND=0," + String(mSendBuffer[0].length()));

                sending = true;
                clkATCommand = mClkMicros;

                sendingStage = SendingStage::STAGE2;
            }
            break;
        }
        case SendingStage::STAGE2:
        {
            if ((mClkMicros - clkATCommand) > INTERVAL_AT_COMMAND)
            {
                sendATCommandToESP8266(mSendBuffer[0]);

                for (int i = 0; i < (mSendBufferSize - 1); ++i)
                {
                    mSendBuffer[i] = mSendBuffer[i + 1];
                }
                mSendBuffer[mSendBufferSize - 1] = "";
                --mSendBufferSize;

                clkCIPSEND = mClkMicros;

                sendingStage = SendingStage::STAGE3;
            }
            break;
        }
        case SendingStage::STAGE3:
        {
            if ((mClkMicros - clkCIPSEND) > INTERVAL_CIPSEND)
            {
                sending = false;

                sendingStage = SendingStage::STAGE1;
            }
            break;
        }
    }
}


inline void WiFiCommunicator::sendATCommandToESP8266(String _ATCommand)
{
    mSerial.println(_ATCommand);
}


inline void WiFiCommunicator::recvData()
{
    /*
        server(board), client(app)

        수신 형태는 두 가지이다.

        0. serial monitor dbg: ~~~\r\n
        -> 무시한다.

        1. data packet structure from client: \r\n+IPD,<id>,<len>:<data>

        2. ATCommand response from Wi-Fi module: \r\n<ATResponse>\r\n
    */


    enum class RecvProcessStage : uint8_t {
        IDENTIFYING_DATA_TYPE,
        PARSING_DATA,
        PROCESSING_DATA
    };
    static RecvProcessStage currStage = RecvProcessStage::IDENTIFYING_DATA_TYPE;
    static bool shouldInitialize = false;

    static char recvChar = 0;
    static String recvString = "";
    static unsigned int endIndex = 0;

    static unsigned int colonIndex = 0;
    static unsigned int commaIndex = -1;
    static unsigned int dataLength = 0;

    static uint32_t clk_lastRecv = mClkMicros;
    static constexpr uint32_t INTERVAL_TIMEOUT_RECV_PER_BYTE = 50000;



    if (mSerial.available())
    {
        // init recv buffer - recv completed
        if (shouldInitialize)
        {
            shouldInitialize = false;
            recvString = "";
            currStage = RecvProcessStage::IDENTIFYING_DATA_TYPE;
        }

        // init recv buffer - timeout
        if ((recvString != "") && (mClkMicros - clk_lastRecv > INTERVAL_TIMEOUT_RECV_PER_BYTE))
        {
            recvString = "";
            currStage = RecvProcessStage::IDENTIFYING_DATA_TYPE;
        }


        // update clk_lastRecv for timeout
        clk_lastRecv = mClkMicros;



        // read byte
        recvChar = static_cast<char>(mSerial.read());
        recvString += recvChar;
        endIndex = recvString.length() - 1;

        // dbg
        // Serial.println(String((int)recvChar) + "    " + recvString);

        // process data
        switch (currStage)
        {
            case RecvProcessStage::IDENTIFYING_DATA_TYPE:
            {
                if (recvString.endsWith("+IPD"))
                {
                    currStage = RecvProcessStage::PARSING_DATA;
                }
                else if (recvChar == '\n')
                {
                    // dbg__
                    if (recvString[0] != '\r')
                    {
                        // ignore data and init
                        shouldInitialize = true;
                        // Serial.println("@");
                    } else
                    // __dbg
                    if (endIndex != 1 && recvString[endIndex - 1] == '\r')
                    {
                        recvHandler(recvString.substring(2, endIndex - 1), RecvDataType::AT_RESPONSE);
                        shouldInitialize = true;
                    }
                }
                break;
            }
            case RecvProcessStage::PARSING_DATA:
            {
                if (recvChar == ':')
                {
                    int iLoop = 1;
                    while (1)               // 위험한 코드@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                    {
                        if (recvString[endIndex - iLoop] == ',')
                        {
                            commaIndex = endIndex - iLoop;
                            break;
                        }
                        ++iLoop;
                    }
                    colonIndex = endIndex;
                    dataLength = static_cast<unsigned int>(recvString.substring(commaIndex + 1, colonIndex).toInt());
                    currStage = RecvProcessStage::PROCESSING_DATA;

                    // dbg
                    // Serial.println("    colon: " + String(colonIndex));
                }
                break;
            }
            case RecvProcessStage::PROCESSING_DATA:
            {
                // dbg__
                // Serial.println(static_cast<int>(recvChar));
                // Serial.println("    end: " + String(endIndex));
                // Serial.println("    end-colon: " + String(endIndex - colonIndex));
                
                if (endIndex - colonIndex == dataLength)
                {
                    recvHandler(recvString.substring(colonIndex + 1), RecvDataType::APP_COMMAND);
                    shouldInitialize = true;
                }
                break;
            }
        }
    }
}


inline void WiFiCommunicator::recvHandler(String _string, RecvDataType _recvType)
{
    // dbg
    // Serial.println("enter the recvHandler() method");

    // C로 작성했던 코드 그대로 가져올까, 프로젝트 리빌딩할까,
    switch (_recvType)
    {
        case RecvDataType::APP_COMMAND:
        {
            // dbg
            Serial.println("APP_ " + _string);


            break;
        }
        case RecvDataType::AT_RESPONSE:
        {
            // dbg
            Serial.println("AT_ " + _string);


            break;
        }
    }
}











#endif // WIFI_COMMUNICATOR_HPP












