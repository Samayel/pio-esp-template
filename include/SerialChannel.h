#ifdef ESP32_WITH_BLUETOOTHSERIAL

// https://www.az-delivery.de/en/blogs/azdelivery-blog-fur-arduino-und-raspberry-pi/das-zehnte-tuerchen

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define SerialChannel SerialBT

void setupSerialChannel()
{
  SerialChannel.begin("ESP32");
}

#else

#define SerialChannel Serial

void setupSerialChannel()
{
  SerialChannel.begin(115200);
}

#endif
