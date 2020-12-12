// Uncomment this line if you want to use the code with freertos only on the ESP32; has to be done before including "OTA.h"
#define ESP32_RTOS

#include <Arduino.h>

#include <Credentials.h>
#include <SerialChannel.h>
#include <TelnetChannel.h>
#include <OTA.h>
#include <NTP.h>

void setup()
{
  setupSerialChannel();
  SerialChannel.println("setup() ...");

  setupOTA(wifi_hostname, wifi_SSID, wifi_password);
  setupTelnetChannel();
  setupNTP();

  //pinMode(LED_BUILTIN, OUTPUT);

  SerialChannel.println("done.");
}

void loop()
{
  SerialChannel.println("loop() ...");
  TelnetChannel.println("loop() ...");

  handleOTA();
  getNTPtime();

  //showTime();

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(1000);

  SerialChannel.println("done.");
  TelnetChannel.println("done.");
  delay(1000);
}
