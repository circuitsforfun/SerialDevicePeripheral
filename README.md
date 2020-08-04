# Serial Device Peripheral

Allows you to create a USB HID like device using the USB Serial interface

**Some key features:**
* Easily enumerate all devices on the host system
* Supports detailed device information: class, type, serial number, version, name, desc.
* Automatically find devices based on user defined Class, Type and/or Serial Number
* Automatically detect baud rate
* Supports many different data types
* Uses Key / Value pairs with dynamically changeable data types
* Robust to changes in the order of the data or data type to easily maintain compatibility between versions of peripheral and host application
* Easily discover available keys and data types in packets
* Optimized binary packet data for streamlined data throughput 

While there are many different serial protocol libraries available, most of them were lacking the key features mentioned above.
I wanted the host side of things to be as easy and robust as possible while avoiding the added complication of writing a custom USB device and host driver.
This library along with the [SerialDeviceHost Library](https://github.com/circuitsforfun/SerialDeviceHost) attempts to fulfill those needs.

The data container essentially works like a very crude primitive version of JSON, you can easily add new key value pairs or modify existing ones and easily mix and match data types but unlike JSON there is no nested or hierarchical aspect to the data structure everything is completely flat.

This library relies heavily on the C++ STL vector class and is not particularly great for performance when used on a microcontroller with limited resources.
It performs fairly well on something like an ESP32 or other fairly decent 32-bit microcontroller with a good clock speed and plenty of memory.

If you are going to use this with a lower-end micro like an Arduino Uno or similar you will need to make sure you have the [ArduinoSTL Library](https://github.com/mike-matera/ArduinoSTL) Installed.
 
That being said you will still want to use any kind of dynamic allocation / deallocation very carefully, I would suggest pre-allocating what ever data structure you need at the start of your program and only make modifications to the values after that.

The serial protocol has a 16-bit CRC implementation using polynomial 0x1021 also referred to as CRC-CCITT XMODEM, while it will use the CRC to reject messages with errors it will not currently attempt to retry the transmission.

**Another thing to note:**
While key strings are limited to 255 characters I would suggest keeping them small usually around 2 or 3 characters this helps to keep the packet size smaller which will help with data throughput.

Installation
-
Download or clone this repo and extract / put it in your Arduino Library folder

Then add library to your sketch or see examples for more information.
