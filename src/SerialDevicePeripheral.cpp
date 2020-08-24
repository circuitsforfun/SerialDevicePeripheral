#include "SerialDevicePeripheral.hpp"

namespace rw {
    namespace serial_device {
        SerialDevicePeripheral::SerialDevicePeripheral(HardwareSerial *serial_device) {
            device_description_.class_id = 1;
            device_description_.type_id = 1;
            device_description_.serial = 1;
            device_description_.version_major = 1;
            device_description_.version_minor = 0;
            device_description_.version_revision = 0;
            device_description_.name = "Generic Device";
            device_description_.info = "Basic device for sending data";
            serial_device_ = serial_device;
            data_available_ = false;
			data_error_ = false;
            header_byte_ = 0xAA;
            packet_id_ = 0xBB;
            stop_byte_ = 0xDD;
        }

        SerialDevicePeripheral::SerialDevicePeripheral(HardwareSerial *serial_device, const deviceDescriptor &desc) {
            device_description_.class_id = desc.class_id;
            device_description_.type_id = desc.type_id;
            device_description_.serial = desc.serial;
            device_description_.version_major = desc.version_major;
            device_description_.version_minor = desc.version_minor;
            device_description_.version_revision = desc.version_revision;
            device_description_.name = desc.name;
            device_description_.info = desc.info;
            serial_device_ = serial_device;
            data_available_ = false;
			data_error_ = false;
            header_byte_ = 0xAA;
            packet_id_ = 0xBB;
            stop_byte_ = 0xDD;
        }

        SerialDevicePeripheral::~SerialDevicePeripheral() = default;

        void SerialDevicePeripheral::setClassID(uint16_t classID) {
            device_description_.class_id = classID;
        }

        void SerialDevicePeripheral::setTypeID(uint16_t typeID) {
            device_description_.type_id = typeID;
        }

        void SerialDevicePeripheral::setSerial(uint32_t serialNum) {
            device_description_.serial = serialNum;
        }

        void SerialDevicePeripheral::setVersion(uint8_t major, uint8_t minor, uint8_t revision) {
            device_description_.version_major = major;
            device_description_.version_minor = minor;
            device_description_.version_revision = revision;
        }

        void SerialDevicePeripheral::setName(std::string name) {
            device_description_.name = name;
        }

        void SerialDevicePeripheral::setInfo(std::string info) {
            device_description_.info = info;
        }

        uint16_t SerialDevicePeripheral::getClassID() const {
            return device_description_.class_id;
        }

        uint16_t SerialDevicePeripheral::getTypeID() const {
            return device_description_.type_id;
        }

        uint32_t SerialDevicePeripheral::getSerial() const {
            return device_description_.serial;
        }

        uint8_t SerialDevicePeripheral::getVersionMajor() const {
            return device_description_.version_major;
        }

        uint8_t SerialDevicePeripheral::getVersionMinor() const {
            return device_description_.version_minor;
        }

        uint8_t SerialDevicePeripheral::getVersionRev() const {
            return device_description_.version_revision;
        }

        std::string SerialDevicePeripheral::getName() const {
            return device_description_.name;
        }

        std::string SerialDevicePeripheral::getInfo() const {
            return device_description_.info;
        }

        void SerialDevicePeripheral::update() {
            if (serial_device_->available()) {
                uint8_t data = serial_device_->read();
                if (data == header_byte_) {
                    data = serial_device_->read();
                    if (data == packet_id_) {
                        in_packet_.clear();
                        uint8_t b1 = serial_device_->read();
                        uint8_t b2 = serial_device_->read();
                        uint16_t pSize = (b1 << 8) + b2;
                        uint8_t cmd = serial_device_->read();
                        for (int i = 0; i < pSize - 4; i++) {
                            in_packet_.push_back(serial_device_->read());
                        }
                        b1 = serial_device_->read();
                        b2 = serial_device_->read();
                        uint8_t stopByte = serial_device_->read();
                        uint16_t packCRC16 = (b1 << 8) + b2;
                        //serial_device_->println(packCRC16);

                        // Check CRC
        #ifdef ARDUINO_STL_USE_VECTOR_BEGIN
                        uint16_t calcCRC16 = crc_.calculate((const unsigned char*)in_packet_.begin(), 0, in_packet_.size());
        #else
                        uint16_t calcCRC16 = crc_.calculate((const unsigned char *) in_packet_.data(), 0, in_packet_.size());
        #endif
                        if (calcCRC16 != packCRC16) {
                            data_available_ = false;
							data_error_ = true;
                            return;
                        }
						data_error_ = false;
                        if (stopByte == stop_byte_) {
                            if (cmd == (uint8_t) SD_COMMAND_GET_INFO) {
                                sendInfoPacket();
                                data_available_ = false;
                                return;
                            }
							else if (cmd == (uint8_t)SD_COMMAND_STOP_DATA)
							{
								stop_data_ = true;
								stop_timeout_ = millis();
							}
							else if (cmd == (uint8_t) SD_COMMAND_SEND_DATA) {
                                if (in_packet_.size() > 0) {
                                    deserialize(in_packet_);
                                    data_available_ = true;
                                    data_read_ = false;
                                    return;
                                }
                            }
                        }
                    }
                }
            } else {
                if (data_read_ == true)
                    data_available_ = false;
            }
        }

        void SerialDevicePeripheral::sendPacket(kSudCommandType cmd) {
			if (stop_data_)
			{
				if (millis() - stop_timeout_ < 4000)
					return;
				else
					stop_data_ = false;
			}
            out_packet_.clear();
            uint16_t pSize = 0;
            cmd_ = cmd;
            out_packet_.push_back(header_byte_);
            out_packet_.push_back(packet_id_);
            out_packet_.push_back(0); // Place holder for packet size
            out_packet_.push_back(0); // Place holder for packet size
            out_packet_.push_back(cmd_);
            pSize++;
            pSize += serialize(out_packet_);
            out_packet_.push_back(0); // Place holder for CRC-16
            out_packet_.push_back(0); // Place holder for CRC-16
            out_packet_.push_back(stop_byte_);
            pSize += 3;
            out_packet_.at(2) = pSize >> 8;
            out_packet_.at(3) = pSize & 0xFF;

            // Calculate and insert CRC16
        #ifdef ARDUINO_STL_USE_VECTOR_BEGIN
            uint16_t calcCRC16 = crc_.calculate((const unsigned char *)out_packet_.begin(), 5, out_packet_.size() - 3);
        #else
            uint16_t calcCRC16 = crc_.calculate((const unsigned char *) out_packet_.data(), 5, out_packet_.size() - 3);
        #endif
            //Serial.print("CRC: ");
            //Serial.println(calcCRC16);
            out_packet_.at(out_packet_.size() - 3) = calcCRC16 >> 8;
            out_packet_.at(out_packet_.size() - 2) = calcCRC16 & 0xFF;

        #ifdef ARDUINO_STL_USE_VECTOR_BEGIN
            //serial_device_->write((const char *)out_packet_.begin(), out_packet_.size());
            for (int i= 0; i < out_packet_.size(); i++)
            {
                uint8_t c = out_packet_[i];
                //Serial.print((char)out_packet_.at(i));
                Serial.write(&c, 1);
                //delay(20);
                //Serial.println(i);
            }
        #else
            serial_device_->write((const uint8_t *) out_packet_.data(), out_packet_.size());
        #endif
        }

        void SerialDevicePeripheral::sendInfoPacket() {
            clearData();
            out_packet_.clear();
            add("class", device_description_.class_id);
            add("type", device_description_.type_id);
            add("serial", device_description_.serial);
            add("v1", device_description_.version_major);
            add("v2", device_description_.version_minor);
            add("v3", device_description_.version_revision);
            add("name", device_description_.name);
            add("info", device_description_.info);
            uint16_t pSize = 0;
            cmd_ = (uint8_t) SD_COMMAND_SEND_INFO;
            out_packet_.push_back(header_byte_);
            out_packet_.push_back(packet_id_);
            out_packet_.push_back(0); // Place holder for packet size
            out_packet_.push_back(0); // Place holder for packet size
            out_packet_.push_back(cmd_);
            pSize++;
            pSize += serialize(out_packet_);
            out_packet_.push_back(0); // Place holder for CRC-16
            out_packet_.push_back(0); // Place holder for CRC-16
            out_packet_.push_back(stop_byte_);
            pSize += 3;
            out_packet_.at(2) = pSize >> 8;
            out_packet_.at(3) = pSize & 0xFF;

            // Calculate and insert CRC16
        #ifdef ARDUINO_STL_USE_VECTOR_BEGIN
            uint16_t calcCRC16 = crc_.calculate((const unsigned char*)out_packet_.begin(), 5, out_packet_.size() - 3);
        #else
            uint16_t calcCRC16 = crc_.calculate((const unsigned char *) out_packet_.data(), 5, out_packet_.size() - 3);
        #endif
            out_packet_.at(out_packet_.size() - 3) = calcCRC16 >> 8;
            out_packet_.at(out_packet_.size() - 2) = calcCRC16 & 0xFF;

        #ifdef ARDUINO_STL_USE_VECTOR_BEGIN
            //serial_device_->write((uint8_t*)out_packet_.begin(), out_packet_.size());
            for (int i = 0; i < out_packet_.size(); i++)
            {
                uint8_t c = out_packet_[i];
                //Serial.print((char)out_packet_.at(i));
                Serial.write(&c, 1);
                //delay(10);
                //Serial.println(i);
            }
        #else
            serial_device_->write((uint8_t *) out_packet_.data(), out_packet_.size());
        #endif
        }

        bool SerialDevicePeripheral::available() {
            return data_available_;
        }
    }
}
