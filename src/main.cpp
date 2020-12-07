// Uncomment this line if you want to use the code with freertos only on the ESP32; has to be done before including "OTA.h"
#define ESP32_RTOS

#include <Arduino.h>
#include <Credentials.h>
#include <OTA.h>
#include <NTP.h>
#include <TelnetStream.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("setup() ...");

  setupOTA(wifi_hostname, wifi_SSID, wifi_password);
  setupNTP();

  //pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("done.");
}

void loop()
{
  Serial.println("loop() ...");
  TelnetStream.println("loop() ...");

  handleOTA();
  getNTPtime();

  //showTime();

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(1000);

  Serial.println("done.");
  TelnetStream.println("done.");
  delay(1000);
}
