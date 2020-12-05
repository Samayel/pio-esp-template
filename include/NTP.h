//
// https://github.com/SensorsIot/NTP-time-for-ESP8266-and-ESP32
//

#include <time.h>
#include <TelnetStream.h>

const uint NTP_TIMEOUT_MILLISECONDS = 10 * 1000;
const bool NTP_REBOOT_ON_ERROR = false;
const char *NTP_SERVER = "de.pool.ntp.org";
const char *TZ_INFO = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"; // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

tm timeinfo;
time_t now;

void showTime(tm localTime)
{
    TelnetStream.printf(
        "%04d-%02d-%02d %02d:%02d:%02d, day %d, %s time\n",
        localTime.tm_year + 1900,
        localTime.tm_mon + 1,
        localTime.tm_mday,
        localTime.tm_hour,
        localTime.tm_min,
        localTime.tm_sec,
        (localTime.tm_wday > 0 ? localTime.tm_wday : 7),
        (localTime.tm_isdst == 1 ? "DST" : "STD"));
}

// time() function only calls the NTP server every hour. So you can always use getNTPtime()
bool getNTPtime()
{
    uint32_t start = millis();
    do
    {
        time(&now);
        localtime_r(&now, &timeinfo);
        TelnetStream.print(".");
        delay(10);
    } while (((millis() - start) <= NTP_TIMEOUT_MILLISECONDS) && (timeinfo.tm_year < (2016 - 1900)));

    if (timeinfo.tm_year <= (2016 - 1900))
    {
        return false; // the NTP call was not successful
    }

    // TelnetStream.print("now ");
    // TelnetStream.println(now);
    // char time_output[30];
    // strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    // TelnetStream.println(time_output);
    // TelnetStream.println();

    return true;
}

void setupNTP()
{
    configTime(0, 0, NTP_SERVER);
    // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
    setenv("TZ", TZ_INFO, 1);

    if (!getNTPtime() && NTP_REBOOT_ON_ERROR)
    {
        ESP.restart();
    }

    showTime(timeinfo);
}
