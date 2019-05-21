
/******************* CONFIG **********************************************/
#define INTERRUPT_PRESCALER 80  /*80Mhz -> 1000.000 ticks per second     */
#define INTERRUPT_TICK      1000000 /* interrupt ervery . sec            */
<<<<<<< Updated upstream
#define WIFIPWD "xxx"
=======
#define WIFIPWD "Isaac_Newton_1642"
>>>>>>> Stashed changes
#define WLAN_SSID "paula"
#define UPDATE_TIME_VAL 4 /* update time every 4 hour from internet or rtc */
#define USE_RTC true
#define DEBUG 1
#define LED_PIN 15
#define NUM_OF_PIXEL (11U*11U)
#define COLOR_OFF ( RgbColor(0, 0, 5))
#define COLOR_ON  ( RgbColor(80, 80, 80))
/*************************************************************************/