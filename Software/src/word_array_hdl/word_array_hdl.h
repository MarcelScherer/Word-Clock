#ifndef WORD_ARRAY_HDL
#define WORD_ARRAY_HDL

#include "../time_hdl/time_hdl.h"

/* get the pointer of the led pixel array */
uint8_t * create_word_array(clock_time_t actaul_time);

/* return if the display shold be off or on depend on config file */
bool check_if_display_on(clock_time_t actaul_time);

#endif