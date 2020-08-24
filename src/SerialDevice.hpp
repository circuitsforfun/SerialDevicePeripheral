#ifndef SERIAL_DEVICE_HPP_
#define SERIAL_DEVICE_HPP_

namespace rw {
  namespace serial_device {

    const uint32_t kSerialNumberAny = 0;

    typedef enum kSudCommandType {
      SD_COMMAND_GET_INFO = 0x50,
      SD_COMMAND_SEND_DATA = 0x64,
      SD_COMMAND_SEND_INFO = 0x69,
	  SD_COMMAND_STOP_DATA = 0x72
    } kSudCommandType;

    typedef struct deviceDescriptor {
      uint16_t class_id;
      uint16_t type_id;
      uint32_t serial;
      uint8_t version_major;
      uint8_t version_minor;
      uint8_t version_revision;
      std::string name;
      std::string info;
    } deviceDescriptor;

    class CRC16 {
     public:
      CRC16() {
        uint16_t remainder;
        int dividend;
        unsigned char bit;
        width_ = 8 * sizeof(uint16_t);
        top_bit_ = 1 << (width_ - 1);

        for (dividend = 0; dividend < 256; ++dividend) {
          remainder = dividend << (width_ - 8);
          for (bit = 8; bit > 0; --bit) {
            if (remainder & top_bit_)
              remainder = (remainder << 1) ^ polynomial_;
            else
              remainder = (remainder << 1);
          }
          crc_table_[dividend] = remainder;
        }
      }
      ~CRC16() = default;

      uint16_t calculate(unsigned char const message[], int startByte, int endByte) {
        uint8_t data;
        uint16_t remainder = 0;

        for (int byte = startByte; byte < endByte; ++byte) {
          data = message[byte] ^ (remainder >> (width_ - 8));
          remainder = crc_table_[data] ^ (remainder << 8);
        }
        return (remainder);
      }

     private:
      uint16_t crc_table_[256]{};
      const char *CRC_name_ = "CRC-CCITT";
      const uint16_t polynomial_ = 0x1021;
      uint16_t width_;
      uint16_t top_bit_;

    };
  } // end namespace serial_device
} // end namespace rw
#endif // SERIAL_DEVICE_HPP_