#ifndef SERIAL_DEVICE_PERIPHERAL_HPP_
#define SERIAL_DEVICE_PERIPHERAL_HPP_

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#if defined(ARDUINO_ARCH_AVR)\
 || defined(ARDUINO_ARCH_MEGAAVR)\
 || defined(ARDUINO_ARCH_SAMD)
#include <ArduinoSTL.h>
#define ARDUINO_STL_USE_VECTOR_BEGIN

#endif

#include "MixedDataType.hpp"
#include "SerialDevice.hpp"

namespace rw {
    namespace serial_device {
        class SerialDevicePeripheral : public mdt::MixedDataType {
         public:
          SerialDevicePeripheral(HardwareSerial *serial_device);
          SerialDevicePeripheral(HardwareSerial *serial_device, const deviceDescriptor &desc);
          ~SerialDevicePeripheral() override;
          void setClassID(uint16_t classID);
          void setTypeID(uint16_t typeID);
          void setSerial(uint32_t serialNum);
          void setVersion(uint8_t major, uint8_t minor, uint8_t revision);
          void setName(std::string name);
          void setInfo(std::string info);
          uint16_t getClassID() const;
          uint16_t getTypeID() const;
          uint32_t getSerial() const;
          uint8_t getVersionMajor() const;
          uint8_t getVersionMinor() const;
          uint8_t getVersionRev() const;
          std::string getName() const;
          std::string getInfo() const;
          void update();
          void sendPacket(kSudCommandType cmd = SD_COMMAND_SEND_DATA);
          bool available();

         protected:
          void sendInfoPacket();

         private:
          deviceDescriptor device_description_;
          HardwareSerial *serial_device_;
          std::vector<uint8_t> in_packet_;
          std::vector<uint8_t> out_packet_;
          uint8_t header_byte_;
          uint8_t packet_id_;
          uint8_t stop_byte_;
          uint8_t cmd_{};
          bool data_available_;
		  bool data_error_;
		  bool stop_data_;
		  long stop_timeout_;
          CRC16 crc_;
        };
    } // End mdt namespace
} // End rw namespace
#endif // SERIAL_DEVICE_PERIPHERAL_HPP_
