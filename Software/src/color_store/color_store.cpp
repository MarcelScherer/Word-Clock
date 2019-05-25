#include <EEPROM.h>
#include "color_store.h"
#include "../cfg.h"

void init_eeporm(void)
{
  if (!EEPROM.begin(6))
  {
      Serial.println("failed to initialise EEPROM");
  }
  for(uint8_t i = 0; i<6; i++)
  {
      Serial.println(EEPROM.read(i));
  }
}


led_col_t init_value_on(void)
{
    led_col_t temp_col_on;

    temp_col_on.red = EEPROM.read(0);
    temp_col_on.green = EEPROM.read(1);
    temp_col_on.blue = EEPROM.read(2);

#ifdef DEBUG
    Serial.print("EEPROM read Color ON:"); Serial.print(temp_col_on.red); Serial.print(" ");
    Serial.print(temp_col_on.green); Serial.print(" "); Serial.println(temp_col_on.blue); 
#endif

    return temp_col_on;
}

led_col_t init_value_off(void)
{
    led_col_t temp_col_off;

    temp_col_off.red = EEPROM.read(3);
    temp_col_off.green = EEPROM.read(4);
    temp_col_off.blue = EEPROM.read(5);

#ifdef DEBUG
    Serial.print("EEPROM read Color OFF:"); Serial.print(temp_col_off.red); Serial.print(" ");
    Serial.print(temp_col_off.green); Serial.print(" "); Serial.println(temp_col_off.blue); 
#endif

    return temp_col_off;
}

void set_value_on(led_col_t temp_col_on)
{
    EEPROM.write(0, temp_col_on.red);
    EEPROM.write(1, temp_col_on.green);
    EEPROM.write(2, temp_col_on.blue);

    EEPROM.commit();

#ifdef DEBUG
    Serial.print("EEPROM write Color ON:"); Serial.print(temp_col_on.red); Serial.print(" ");
    Serial.print(temp_col_on.green); Serial.print(" "); Serial.println(temp_col_on.blue); 
#endif
}

void set_value_off(led_col_t temp_col_off)
{
    EEPROM.write(3, temp_col_off.red);
    EEPROM.write(4, temp_col_off.green);
    EEPROM.write(5, temp_col_off.blue);

    EEPROM.commit();

#ifdef DEBUG
    Serial.print("EEPROM write Color OFF:"); Serial.print(temp_col_off.red); Serial.print(" ");
    Serial.print(temp_col_off.green); Serial.print(" "); Serial.println(temp_col_off.blue); 
#endif
}