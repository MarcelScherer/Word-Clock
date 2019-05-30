#ifndef WORD_ARRAY_HDL
#define WORD_ARRAY_HDL

#include "../time_hdl/time_hdl.h"

uint8_t * create_word_array(clock_time_t actaul_time);
bool check_if_display_on(clock_time_t actaul_time);

#endif