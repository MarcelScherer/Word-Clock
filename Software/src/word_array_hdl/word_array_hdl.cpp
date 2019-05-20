#include "../cfg.h"
#include <string.h>
#include "word_array_hdl.h"

uint8_t * create_hours(uint8_t * p_array, uint8_t hour, uint8_t minutes );
void create_minutes(uint8_t * p_array, uint8_t minutes );

uint8_t * create_word_array(clock_time_t actaul_time)
{
    static uint8_t pixelarray[NUM_OF_PIXEL];                            // pixel arry for pixel actuation
    std::fill_n(pixelarray, sizeof(pixelarray), 0);                     // fill array with zeros

    /* ES */
    pixelarray[110] = 1;
    pixelarray[111] = 1;
    /* IST */
    pixelarray[113] = 1;
    pixelarray[114] = 1;
    pixelarray[115] = 1;
    
    // set right pixel for minutes and hours
    create_minutes(&pixelarray[0], actaul_time.minutes );               
    uint8_t * p_pixelarray = create_hours(&pixelarray[0], actaul_time.hour, actaul_time.minutes );
    return p_pixelarray;
}

uint8_t * create_hours(uint8_t * p_array, uint8_t hour, uint8_t minutes )
{
    static uint8_t hour_12;
    static uint8_t temp_hour;

    // if the next full hour is called
    if((minutes/5) >= 5 || (minutes/5) == 3)
    {
        temp_hour = hour + 1;
    }
    else
    {
        temp_hour = hour;
    }

    if(temp_hour == 0 || temp_hour == 12 || temp_hour == 24)
    {
        hour_12 = 12;
    }
    else
    {
        hour_12 = temp_hour%12;
    }

   switch ( hour_12 ){
        case 1 :
            if(minutes/5 == 0)
            {for(uint8_t i = 50; i<53; i++){ p_array[i] = 1; } break;} // EIN
            else
            {for(uint8_t i = 50; i<54; i++){ p_array[i] = 1; } break;} // EINS
        case 2 : 
            for(uint8_t i = 15; i<19; i++){ p_array[i] = 1; }break;  // ZWEI
        case 3 : 
            for(uint8_t i = 55; i<59; i++){ p_array[i] = 1; }break;  // DREI
        case 4 : 
            for(uint8_t i = 62; i<66; i++){ p_array[i] = 1; }break;  // VIER
        case 5 : 
            for(uint8_t i = 40; i<44; i++){ p_array[i] = 1; }break;  // FÜNF
        case 6 : 
            for(uint8_t i = 27; i<32; i++){ p_array[i] = 1; }break;  // SECHS
        case 7 : 
            for(uint8_t i = 71; i<77; i++){ p_array[i] = 1; }break;  // SIEBEN
        case 8 : 
            for(uint8_t i = 22; i<26; i++){ p_array[i] = 1; }break;  // ACHT
        case 9 : 
            for(uint8_t i = 33; i<37; i++){ p_array[i] = 1; } break;  // NEUN
        case 10 : 
            for(uint8_t i = 36; i<40; i++){ p_array[i] = 1; } break;  // ZEHN
        case 11 : 
            for(uint8_t i = 59; i<62; i++){ p_array[i] = 1; } break;  // ELF
        case 12 : 
            for(uint8_t i = 45; i<50; i++){ p_array[i] = 1; } break;  // ZWÖLF
        default:
            break;
    }
    return &p_array[0];
}

void create_minutes(uint8_t * p_array, uint8_t minutes )
{
  switch ( minutes/5 ){
        case 0 :
            for(uint8_t i = 82; i<87; i++){ p_array[i] = 1; };        // GENAU
            for(uint8_t i = 2; i<5; i++){ p_array[i] = 1; } break;    // UHR
        case 1 :
            for(uint8_t i = 117; i<121; i++){ p_array[i] = 1; }       // FÜNF
            for(uint8_t i = 95; i<99; i++){ p_array[i] = 1; } break;  // NACH
        case 2 : 
            for(uint8_t i = 99; i<103; i++){ p_array[i] = 1; }        // ZEHN
            for(uint8_t i = 95; i<99; i++){ p_array[i] = 1; } break;  // NACH
        case 3 : 
            for(uint8_t i = 88; i<95; i++){ p_array[i] = 1; }break;   // VIETEL
        case 4 : 
            for(uint8_t i = 103; i<110; i++){ p_array[i] = 1; }        // ZWANZIG
            for(uint8_t i = 95; i<99; i++){ p_array[i] = 1; } break;   // NACH
        case 5 : 
            for(uint8_t i = 117; i<121; i++){ p_array[i] = 1; }        // FÜNF
            for(uint8_t i = 78; i<81; i++){ p_array[i] = 1; }          // VOR
            for(uint8_t i = 66; i<70; i++){ p_array[i] = 1; }break;    // HALB
        case 6 : 
            for(uint8_t i = 66; i<70; i++){ p_array[i] = 1; }break;    // HALB 
        case 7 : 
            for(uint8_t i = 117; i<121; i++){ p_array[i] = 1; }        // FÜNF
            for(uint8_t i = 95; i<99; i++){ p_array[i] = 1; }          // NACH
            for(uint8_t i = 66; i<70; i++){ p_array[i] = 1; }break;    // HALB
        case 8 : 
            for(uint8_t i = 103; i<110; i++){ p_array[i] = 1; }        // ZWANZIG
            for(uint8_t i = 78; i<81; i++){ p_array[i] = 1; }break;    // VOR
        case 9:
            for(uint8_t i = 88; i<95; i++){ p_array[i] = 1; }         // VIETEL
            for(uint8_t i = 78; i<81; i++){ p_array[i] = 1; }break;   // VOR
        case 10 : 
            for(uint8_t i = 99; i<103; i++){ p_array[i] = 1; }        // ZEHN
            for(uint8_t i = 78; i<81; i++){ p_array[i] = 1; }break;   // VOR
        case 11 : 
            for(uint8_t i = 117; i<121; i++){ p_array[i] = 1; }       // FÜNF
            for(uint8_t i = 78; i<81; i++){ p_array[i] = 1; }break;   // VOR
  }
}