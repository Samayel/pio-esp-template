// Uncomment this line if you want to use the code with freertos only on the ESP32; has to be done before including "OTA.h"
#define ESP32_RTOS
                   
#include <Arduino.h>
#include <Credentials.h>
#include <OTA.h>
#include <NTP.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");

  setupOTA(wifi_hostname, wifi_SSID, wifi_password);
  setupNTP();

  //pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  handleOTA();

  getNTPtime();
  showTime(timeinfo);
  delay(1000);

  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(1000);
}
