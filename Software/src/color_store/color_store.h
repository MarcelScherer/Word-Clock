#include <Arduino.h>

typedef struct  
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} led_col_t;

void init_eeporm(void);
led_col_t init_value_on(void);
led_col_t init_value_off(void);
void set_value_on(led_col_t temp_col_on);
void set_value_off(led_col_t temp_col_off);