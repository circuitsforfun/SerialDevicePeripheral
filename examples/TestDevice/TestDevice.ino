/*
  Test Device
  Shows how to configure a Serial Device as a simple test
  and allows you to toggle the builtin LED from the host application

*/
#include <SerialDevicePeripheral.hpp>

#ifndef LED_BUILTIN
#define LED_BUILTIN 22
#endif

// Pointer for our SerialDevicePeripheral that we will instantiate / initialize later
rw::serial_device::SerialDevicePeripheral *myDevice;

// the setup function runs once when you press reset or power the board
void setup() {

  // Initialize Serial
  Serial.begin(115200);
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // SerialDevicePeripheral Device Description that we need to fill in for intializing our device
  rw::serial_device::deviceDescriptor desc;

  desc.class_id = 0x0A0A;     // Class ID for our device (can be any 16-bit unsigned interger value that you want)
  desc.type_id = 0x0D0D;      // Type ID for our device (can be any 16-bit unsigned interger value that you want)
  desc.serial = 1010101;      // Unique Serial Number for our device
  desc.version_major = 0;     // Device Version Major
  desc.version_minor = 1;     // Device Version Minor
  desc.version_revision = 1;  // Device Version Revision
  desc.name = "Test Device";  // Name for our device
  desc.info = "Device For Testing Protocol";  // Description

  // Instantiate / Initialize our device with the serial device and description we want to use
  myDevice = new rw::serial_device::SerialDevicePeripheral((HardwareSerial*)&Serial, desc);
  
}

// the loop function runs over and over again forever
void loop() {

  if (myDevice->available()) // Check if there is data waiting in the buffer
  {
    if (myDevice->get<std::string>("cmd") == "led") // Get a string variable named "cmd" (will return an empty string if "cmd" doesn't exist
    {
      // Get a uint8_t variable named "sta" and turn the LED on or off based on the value
      if (myDevice->get<uint8_t>("sta") == 0)
        digitalWrite(LED_BUILTIN, LOW);
      else if (myDevice->get<uint8_t>("sta") == 1)
        digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  // Must call this update periodically to process SerialDevicePeripheral routines
  myDevice->update();
}
