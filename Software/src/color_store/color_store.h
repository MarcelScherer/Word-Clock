#include <Arduino.h>

typedef struct  
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} led_col_t;

/* init EEPROM */
void init_eeporm(void);

/* load EEPROM value for led on */
led_col_t init_value_on(void);

/* load EEPROM value for led off */
led_col_t init_value_off(void);

/* store EEPROM value for led on */
void set_value_on(led_col_t temp_col_on);

/* store EEPROM value for led off */
void set_value_off(led_col_t temp_col_off);