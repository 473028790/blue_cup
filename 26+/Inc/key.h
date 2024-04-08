#ifndef __KEY_H
#define __KEY_H
#include "main.h"
#include "stdbool.h"
#include "tim.h"
#include "adc.h"
struct keys
{
	uint8_t  judge_sta;
	uint8_t  key_sta;
	uint8_t  single_flag;
	uint8_t  long_flag;
	int32_t  key_time;
};

void key_proc(void);
void key_task(void);
void adc_task(void);
void pwm_up(void);
void pwm_down(void);
void MAX_speed(void);
#endif
