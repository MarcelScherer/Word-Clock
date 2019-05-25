#include <Arduino.h>
#include "cfg.h"
#include "time_hdl/time_hdl.h"
#include <NeoPixelBus.h>
#include "word_array_hdl/word_array_hdl.h"
#include "color_store/color_store.h"


volatile int interruptCounter;
int totalInterruptCounter;
uint8_t update_hour;

led_col_t led_on  = {80, 80, 80};
led_col_t led_off = {0,   0,  5};

#define COLOR_OFF ( RgbColor(led_off.red, led_off.green, led_off.blue))
#define COLOR_ON  ( RgbColor(led_on.red,  led_on.green,  led_on.blue))

// Set web server port number to 80
WiFiServer server(80);

//NeoPixelBus<LED_STRIPE_TYP, NeoEsp32I2s1800KbpsMethod> strip(11*11, LED_PIN);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(NUM_OF_PIXEL, LED_PIN);

hw_timer_t * timer = NULL;                                           /* pointer for interrupt */  
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;                /* semaphore for interruptCounter */

 /* interrup function called by internal interrupt */
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);                                 /* lock semaphore */ 
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);                                  /* release semaphore */
}
 
 /* initialize */
void setup() {
  Serial.begin(115200);                                              /* start serial monitoring with 115.200 baut */ 

  time_hdl_initialize();
  init_eeporm();

    if(WiFi.status() == WL_CONNECTED)
    {
        server.begin();
    }

  strip.Begin();
  strip.Show();

  led_on = init_value_on();
  led_off = init_value_off();

  timer = timerBegin(0, INTERRUPT_PRESCALER, true);                 /* config interrupt register */
  timerAttachInterrupt(timer, &onTimer, true);                      /* function ponter for call by interrup */
  timerAlarmWrite(timer, INTERRUPT_TICK, true);                     /* set interrupt counter */
  timerAlarmEnable(timer);                                          /* enable interrup */
  update_hour = update_clock_time(0).hour;
}
 
void loop() {

  uint8_t ticks = 0;

  if (interruptCounter > 0) {                                        /* check if interrup was set */
 
    portENTER_CRITICAL(&timerMux);                                   /* lock semaphore */
    ticks = interruptCounter;                                              
    interruptCounter = 0;
    portEXIT_CRITICAL(&timerMux);                                    /* release semaphore */

    clock_time_t timer = update_clock_time(ticks);                   /* update actual time */
#ifdef DEBUG
//    Serial.print(timer.hour);
//    Serial.print(":");
//    Serial.print(timer.minutes);
//    Serial.print(":");
//    Serial.println(timer.seconds);
#endif
    /* update time from internet or RTC */
    if((timer.hour - update_hour )%24 > UPDATE_TIME_VAL)            
    {
#ifdef DEBUG
    Serial.println("update time");
    Serial.print("update_hour:");
    Serial.println(update_hour);
    Serial.print("timer.hour:");
    Serial.println(timer.hour);
#endif
      /* update time from internet if wlan is available */
      if(check_wlan_conection())                                
      {
        timer = get_internet_time();
      }
      /* or update from RTC */
      else if(USE_RTC)
      {
        timer = get_rtc_time();
      }
      else
      {
        
      }
      update_hour = timer.hour;
    }

    /* update all pixels from pixel array*/
    uint8_t * p_pixel_array = create_word_array(timer);
    for(int8_t i=0; i<NUM_OF_PIXEL; i++)
    {
      if(p_pixel_array[i]  == 1)
      {
        strip.SetPixelColor(i, COLOR_ON);
      }
      else
      {
        strip.SetPixelColor(i, COLOR_OFF);
      }
      
    }
    strip.Show();
  } /* end of second cycle*/

  WiFiClient client = server.available();   // Listen for incoming clients
  if (client)                               // If a new client connects,
  {
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected())              // loop while the client's connected
    {
      if (client.available())               // if there's bytes to read from the client,
      {
        uint8_t data_dump[5];
        std::fill_n(data_dump, sizeof(data_dump), 0);  // fill array with zeros

        Serial.print("datadump: "); 
        for(uint8_t k = 0; k<5; k++)
        {
          data_dump[k] = client.read(); 
          Serial.print(data_dump[k]); 
          Serial.print(" ");
        }
        Serial.println(""); 

        if(data_dump[4] == 1)
        {
          led_on.red = (uint8_t)((float)(data_dump[1])*(float)(data_dump[0])
                       /((float)(0xFF)*((float)(0xFF)/(float)(MAX_BRIGHTNESS_ON)) ));
          led_on.green = (uint8_t)((float)(data_dump[2])*(float)(data_dump[0])
                       /((float)(0xFF)*((float)(0xFF)/(float)(MAX_BRIGHTNESS_ON)) ));
          led_on.blue = (uint8_t)((float)(data_dump[3])*(float)(data_dump[0])
                       /((float)(0xFF)*((float)(0xFF)/(float)(MAX_BRIGHTNESS_ON)) ));

          set_value_on(led_on);
#ifdef DEBUG
          Serial.print("Color ON:"); Serial.print(led_on.red); Serial.print(" ");
          Serial.print(led_on.green); Serial.print(" "); Serial.println(led_on.blue); 
#endif
        }
        else if(data_dump[4] == 2)
        {      
          led_off.red = (uint8_t)((float)(data_dump[1])*(float)(data_dump[0])
                       /((float)(0xFF)*((float)(0xFF)/(float)(MAX_BRIGHTNESS_OFF)) ));
          led_off.green = (uint8_t)((float)(data_dump[2])*(float)(data_dump[0])
                       /((float)(0xFF)*((float)(0xFF)/(float)(MAX_BRIGHTNESS_OFF)) ));
          led_off.blue = (uint8_t)((float)(data_dump[3])*(float)(data_dump[0])
                       /((float)(0xFF)*((float)(0xFF)/(float)(MAX_BRIGHTNESS_OFF)) ));

          set_value_off(led_off);
#ifdef DEBUG
          Serial.print("Color OFF:"); Serial.print(led_off.red); Serial.print(" ");
          Serial.print(led_off.green); Serial.print(" "); Serial.println(led_off.blue); 
#endif
        }
        else
        {
          Serial.println("wring data dump");
          strip.SetPixelColor(0, RgbColor(100, 0, 0));
          strip.Show();
        }
        
      }
    }
  }
}

