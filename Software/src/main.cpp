#include <Arduino.h>
#include "cfg.h"
#include "time_hdl/time_hdl.h"
#include <NeoPixelBus.h>
#include "word_array_hdl/word_array_hdl.h"


volatile int interruptCounter;
int totalInterruptCounter;
uint8_t update_hour;

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

  strip.Begin();
  strip.Show();

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

    clock_time_t timer = update_clock_time(ticks);
#ifdef DEBUG
    Serial.print(timer.hour);
    Serial.print(":");
    Serial.print(timer.minutes);
    Serial.print(":");
    Serial.println(timer.seconds);
#endif
    if((timer.hour - update_hour )%24 > UPDATE_TIME_VAL)
    {
#ifdef DEBUG
    Serial.println("update time");
    Serial.print("update_hour:");
    Serial.println(update_hour);
    Serial.print("timer.hour:");
    Serial.println(timer.hour);
#endif
      if(check_wlan_conection())
      {
        timer = get_internet_time();
      }
      else if(USE_RTC)
      {
        timer = get_rtc_time();
      }
      else
      {
        
      }
    }

    uint8_t * p_pixel_array = create_word_array(timer);
    Serial.print("ARRRAY: ");
    for(int8_t i=0; i<NUM_OF_PIXEL; i++)
    {
      if(p_pixel_array[i]  == 1)
      {
        strip.SetPixelColor(i, COLOR_ON);
        Serial.print(1);
      }
      else
      {
        strip.SetPixelColor(i, COLOR_OFF);
        Serial.print(0);
      }
      
    }
    Serial.println(" ");

    strip.Show();
  }
}

