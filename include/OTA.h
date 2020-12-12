//
// https://github.com/SensorsIot/ESP32-OTA
//

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#if defined(ESP32_RTOS) && defined(ESP32)
void ota_handle(void *parameter)
{
    for (;;)
    {
        ArduinoOTA.handle();
        delay(3500);
    }
}

void handleOTA()
{
}
#else // If you do not use FreeRTOS, you have to regulary call the handle method.
void handleOTA()
{
    ArduinoOTA.handle();
}
#endif

void setupOTA(const char *nameprefix, const char *ssid, const char *password)
{
    // Configure the hostname
    uint16_t maxlen = strlen(nameprefix) + 7;
    char *fullhostname = new char[maxlen];
    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
    ArduinoOTA.setHostname(fullhostname);
    delete[] fullhostname;

    // Configure and start the WiFi station
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        SerialChannel.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Port defaults to 3232
    // ArduinoOTA.setPort(3232); // Use 8266 port if you are working in Sloeber IDE, it is fixed there and not adjustable

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        //NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            type = "sketch";
        }
        else // U_SPIFFS
        {
            type = "filesystem";
        }

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        SerialChannel.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        SerialChannel.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        SerialChannel.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        SerialChannel.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
        {
            SerialChannel.println("\nAuth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            SerialChannel.println("\nBegin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            SerialChannel.println("\nConnect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            SerialChannel.println("\nReceive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            SerialChannel.println("\nEnd Failed");
        }
    });

    ArduinoOTA.begin();

    SerialChannel.println("OTA initialized");
    SerialChannel.print("IP address: ");
    SerialChannel.println(WiFi.localIP());
    SerialChannel.print("Hostname: ");
    SerialChannel.println(ArduinoOTA.getHostname());

#if defined(ESP32_RTOS) && defined(ESP32)
    xTaskCreate(
        ota_handle,   /* Task function. */
        "OTA_HANDLE", /* String with name of task. */
        10000,        /* Stack size in bytes. */
        NULL,         /* Parameter passed as input of the task */
        1,            /* Priority of the task. */
        NULL);        /* Task handle. */
#endif
}
