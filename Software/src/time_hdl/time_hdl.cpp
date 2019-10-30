#include "time_hdl.h"
#include "../cfg.h"
#include <string.h>
#include <RTClib.h>
#include <ESPmDNS.h>

static void set_clock_timer(void);
int8_t summertime_EU_offest(int year, byte month, byte day, byte hour, byte tzHours);

/* wlan connection */
const char* ssid     = WLAN_SSID;                             
const char* password = WIFIPWD; 

/* rtc object */
static RTC_DS3231 rtc;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

clock_time_t clock_time;

void time_hdl_initialize(void)
{
    uint8_t wlan_init_count = 0;

    pinMode(SUMMER_TIME_PIN, INPUT);                                    // sets the digital pin for summer time switch as input

    WiFi.begin(ssid, password);                                         // set ssid and password
    while (    (WiFi.status() != WL_CONNECTED)                          // while wlan not connected
            && (wlan_init_count < 20))
    {
        if(USE_RTC)
        {
            wlan_init_count++;                                          // increment timer for wlan time out
        }
        delay(500);                                                     // wait 0.5 seconds for wlan connection
#ifdef DEBUG                                                         
        Serial.print(".");                                              // print "."
#endif
    }
#ifdef DEBUG 
    Serial.println(".");
    Serial.print("IP address: ");                                      // print local ip
    Serial.println(WiFi.localIP());
#endif

    if(USE_RTC)
    {
        /* init rtc module */
        if (! rtc.begin()) {                                           // try to connect to rtc
            Serial.println("Couldn't find RTC");
            while (1);
        }
        else
        {
    #ifdef DEBUG
            Serial.println("RTC found");
    #endif   
        }
    }

    if((WiFi.status() == WL_CONNECTED))                                 /* if wlan is connected ... */
    {
        // Initialize a NTPClient to get time
        timeClient.begin();                                             /* Initialize NTP clinet ... */
        timeClient.setTimeOffset(3600);                                 /* ... set time offset ... */
        timeClient.forceUpdate();                                       /* ... and read time from internet */
        set_clock_timer();                                              /* ... convert internet time to clock_time */
        set_rtc_date_time(clock_time, 2018, 1, 1);                      /* write actual time in rtc clock */

        if (!MDNS.begin("Word_Clock")) 
        {
#ifdef DEBUG 
            Serial.println("Error setting up MDNS responder!");
#endif 
        }
#ifdef DEBUG 
        Serial.println(timeClient.getFormattedTime());
#endif 
    }        
    else                                                                /* if wlan is not connected ... */
    {
        clock_time = get_rtc_time();                                    /* reat time from rtc clock */
    }

}

/* convert ntc time to clock_time */
static void set_clock_timer(void)
{
    if(!digitalRead(SUMMER_TIME_PIN))
    {
        clock_time.hour    = timeClient.getHours();                     // winter time
#ifdef DEBUG 
        Serial.println("winter time");
#endif 
    }
    else
    {
        clock_time.hour    = timeClient.getHours() + 1;                 // summer time
#ifdef DEBUG 
        Serial.println("summer time");
#endif 
    }
    clock_time.minutes = timeClient.getMinutes();
    clock_time.seconds = timeClient.getSeconds();
}

bool check_wlan_conection(void)
{
    uint8_t count = 0;

    if(WiFi.status() != WL_CONNECTED)                                   /* if wlan not connected */
    {
        WiFi.begin(ssid, password);                                     /* try to connect */
    }

    if(    (WiFi.status() != WL_CONNECTED)                              /* wait for connection */
        || count >= 20)
    {
        delay(500); 
    }

    return (WiFi.status() == WL_CONNECTED);                             /* return internet status connectet = true */
}

/* update time */
clock_time_t update_clock_time(uint8_t ticks)
{ 
    uint8_t seconds = clock_time.seconds + ticks;                       /* add seconds and look that not over 59 */
    clock_time.seconds = seconds % 60;

    uint8_t minutes = clock_time.minutes + seconds / 60;                /* add minutes and look that not over 59 */
    clock_time.minutes = minutes % 60;

    uint8_t hour = clock_time.hour + minutes / 60;                      /* add hours and look that not over 23 */
    clock_time.hour = hour % 24;

    return clock_time;                                                  /* return new clock_time */
}

/* write time and date to rtc clock */
void set_rtc_date_time(clock_time_t time, uint16_t year, uint8_t month, uint8_t day )
{
#ifdef DEBUG 
        Serial.print("set RTC time: ");
        Serial.print(time.hour);
        Serial.print(":");
        Serial.print(time.minutes);
        Serial.print(":");
        Serial.println(time.seconds);
#endif
    rtc.adjust(DateTime(year, month, day, time.hour, time.minutes, time.seconds));
}

/* get time from rtc clock */
clock_time_t get_rtc_time(void)
{
    clock_time_t temp_time;
    DateTime now = rtc.now();                                           /* reat time from rtc clock ... */

    temp_time.hour    = now.hour();                                     /* ... and convert to clock_time */
    temp_time.minutes = now.minute();
    temp_time.seconds = now.second();

#ifdef DEBUG 
        Serial.print("get RTC time: ");
        Serial.print(temp_time.hour);
        Serial.print(":");
        Serial.print(temp_time.minutes);
        Serial.print(":");
        Serial.println(temp_time.seconds);
#endif
    return temp_time;                                                   /* return clock_time from rtc clock */
}

/* get time from internet ntp */
clock_time_t get_internet_time(void)
{
    timeClient.forceUpdate();                                           /* reat time from internet ntp server */
    set_clock_timer();                                                  /* convert ntp time to clock_time */
    return clock_time;                                                  /* retunr clock_time form ntp server*/
}

int8_t summertime_EU_offest(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
  if (month<3 || month>10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month>3 && month<10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month==3 && (hour + 24 * day)>=(1 + tzHours + 24*(31 - (5 * year /4 + 4) % 7)) || month==10 && (hour + 24 * day)<(1 + tzHours + 24*(31 - (5 * year /4 + 1) % 7)))
    return 1;
  else
    return 0;
}