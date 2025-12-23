#ifndef MY_PMS7003_HPP
#define MY_PMS7003_HPP

#include <Arduino.h>






class MyPMS7003
{
public:
    MyPMS7003(HardwareSerial& _serial);

    void begin(uint32_t _baudRate);

    // polling
    void readRawDataFromPMS7003();
    
    // condition: rawDataReceived(true)
    void processRawData();
    
    void clearUpdatedFlag();

    bool isRawDataReceived() const;
    bool isProcessedDataUpdated() const;
    uint16_t getPM10() const;
    
    
private:
    HardwareSerial& serial;
    static constexpr uint8_t PMS7003_BUFFER_LENGTH = 32;
    uint8_t recvBuffer[PMS7003_BUFFER_LENGTH] = { 0, };
    static constexpr uint16_t INVALID_PM_VALUE = 65535;
    uint16_t pm1_0 = INVALID_PM_VALUE;
    uint16_t pm2_5 = INVALID_PM_VALUE;
    uint16_t pm10 = INVALID_PM_VALUE;
    bool rawDataReceived = false;
    bool processedDataUpdated = false;
};





MyPMS7003::MyPMS7003(HardwareSerial& _serial)
    : serial(_serial)
{
}


void MyPMS7003::begin(uint32_t _baudRate)
{
    serial.begin(_baudRate);
}


inline void MyPMS7003::readRawDataFromPMS7003()
{
    static uint8_t index = 0;
    static uint8_t currentByte = 0;

    while (serial.available())
    {
        currentByte = serial.read();

        // 2 bytes index(0, 1) = 0x424d  ->  PMS7003 dataPacket's header
        // 통신 오류로 데이터가 밀리면, index(0, 1)에서 header를 기다린다.
        if (index == 0 && currentByte != 0x42)
        {
            continue;
        }
        if (index == 1 && currentByte != 0x4d)
        {
            index = 0;
            continue;
        }

        recvBuffer[index++] = currentByte;

        if (index == PMS7003_BUFFER_LENGTH)
        {
            index = 0;

            rawDataReceived = true;
        }
    }
}


inline void MyPMS7003::processRawData()
{
  pm1_0 = (static_cast<uint16_t>(recvBuffer[10]) << 8) + static_cast<uint16_t>(recvBuffer[11]);
  pm2_5 = (static_cast<uint16_t>(recvBuffer[12]) << 8) + static_cast<uint16_t>(recvBuffer[13]);
  pm10 = (static_cast<uint16_t>(recvBuffer[14]) << 8) + static_cast<uint16_t>(recvBuffer[15]);

  rawDataReceived = false;
  processedDataUpdated = true;
}


inline void MyPMS7003::clearUpdatedFlag()
{
    processedDataUpdated = false;
}


inline bool MyPMS7003::isRawDataReceived() const
{
    return rawDataReceived;
}


inline bool MyPMS7003::isProcessedDataUpdated() const
{
    return processedDataUpdated;
}


inline uint16_t MyPMS7003::getPM10() const
{
    return pm10;
}









#endif // MY_PMS7003_HPP











