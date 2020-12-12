//
// https://github.com/SensorsIot/NTP-time-for-ESP8266-and-ESP32
//

#include <time.h>

// enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)
#define NTP_TIMEZONE "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"

#define NTP_SERVER1 "0.de.pool.ntp.org"
#define NTP_SERVER2 "1.de.pool.ntp.org"
#define NTP_SERVER3 "2.de.pool.ntp.org"

time_t now;
tm localTime;

// time() function only calls the NTP server every hour (after successful initialization!). So you can always use getNTPtime()
bool getNTPtime()
{
    time(&now);
    localtime_r(&now, &localTime);

    return (localTime.tm_year > 100);
}

void showTime()
{
    SerialChannel.printf(
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

void setupNTP()
{
    SerialChannel.println("setupNTP() ...");

    configTime(0, 0, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
    setenv("TZ", NTP_TIMEZONE, 1);

    getNTPtime();
    showTime();

    SerialChannel.println("done.");
}
