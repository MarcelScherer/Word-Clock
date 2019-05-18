#ifndef TIME_HDL
#define TIME_HDL

#include <Arduino.h>
#include "WiFi.h"
#include <NTPClient.h>


typedef struct{
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
} clock_time_t;

/* initialize time handlier */
void time_hdl_initialize(void);

/* update time -> add saconds and return new time*/
clock_time_t update_clock_time(uint8_t ticks);

/* get time from rtc clock */
clock_time_t get_rtc_time(void);

/* check if wlan is connected and try to connect */
bool check_wlan_conection(void);

/* get time from internet ntp */
clock_time_t get_internet_time(void);

/* write time and date to rtc clock */
void set_rtc_date_time(clock_time_t time, uint16_t year, uint8_t month, uint8_t day );

#endif