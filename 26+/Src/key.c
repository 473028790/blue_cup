#include "key.h"
extern int lcd_interface;
extern uint8_t frequency;
struct keys key[4]={0,0,0,0,0};

void key_proc(void)
{
		key[0].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
		key[1].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
		key[2].key_sta=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
		key[3].key_sta=HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);

		for(int i=0;i<4;i++)
		{
			switch (key[i].judge_sta)
			{
				case 0:
				{
					if(key[i].key_sta==0) 
					{
						key[i].judge_sta=1;
						key[i].key_time=0;
					}
				}
				break;
				case 1:
				{
					if(key[i].key_sta==0)
					{
						key[i].judge_sta=2;
					}
					else key[i].judge_sta=0;
				}
				break;
				case 2:
				{
					if(key[i].key_sta==1)
					{
						key[i].judge_sta=0;					
						if(key[i].key_time<20)
						{
							key[i].single_flag=1;
						}
					}
					else 
					{
						key[i].key_time++;
						if(key[i].key_time>20)  key[i].long_flag=1;
					}
				}
				break;
				
			}
		
		}
}	
int R_OR_K=0;
extern int R,K;
uint8_t Duty_loke=0;
int high_or_low_number=0;
extern int psc_finish_up_flag,psc_finish_down_flag;
extern int psc_number_down,psc_number;
void key_task(void)
{
	if(psc_finish_up_flag==0)
	{
		pwm_up();
	}
	if(psc_finish_down_flag==0)
	{
		pwm_down();
	}
	if(key[0].single_flag==1)
	{
		lcd_interface++;
		if(lcd_interface>=3) lcd_interface=0;
		key[0].single_flag=0;
	}
	if(key[1].single_flag==1) //B2
	{
		if(lcd_interface==0)
		{
			if(frequency==1)
			{
				frequency=0;
				psc_number=10;
				psc_finish_up_flag=0;
				//__HAL_TIM_SET_PRESCALER(&htim2,20-1);
				high_or_low_number++;
			}
			else if(frequency==0)
			{
				frequency=1;
				psc_number_down=19;
				psc_finish_down_flag=0;
				//__HAL_TIM_SET_PRESCALER(&htim2,10-1);
				high_or_low_number++;
			}
		}
		if(lcd_interface==1)
		{
			if(R_OR_K==1)
			{
				R_OR_K=0;
			}
			else if(R_OR_K==0)
			{
				R_OR_K=1;
			}
		}
		key[1].single_flag=0;
	}
	
	if(key[2].single_flag==1)//B3 jiafa
	{
		if(lcd_interface==1)
		{
			if(R_OR_K==0)
			{
				R++;
				if(R>10) R=1;
			}
			else if(R_OR_K==1)
			{
				K++;
				if(K>10) K=1;
			}
		}
		key[2].single_flag=0;
	}
	
	if(key[3].long_flag==1)//B4 jianfa long
	{
		if(lcd_interface==0)
		{
			if(Duty_loke==1)
			{
				Duty_loke=0;
			}
			else if(Duty_loke==0)
			{
				Duty_loke=1;
			}
		}
		key[3].long_flag=0;
	}
		
	if(key[3].single_flag==1)//B4 jianfa
	{
		
		if(lcd_interface==1)
		{
			if(R_OR_K==0)
			{
				R--;
				if(R<1) R=10;
			}
			else if(R_OR_K==1)
			{
				K--;
				if(K<1) K=10;
			}
		}
		key[3].single_flag=0;
	}
	
}


extern ADC_HandleTypeDef hadc2;
uint16_t getADC(void)
{
	uint16_t adc = 0;
	
	HAL_ADC_Start(&hadc2);
	adc = HAL_ADC_GetValue(&hadc2);
	
	return adc;
}
float adc_number=0;
extern int dut;
void adc_task(void)
{
	adc_number=getADC()*3.3/4096;
	if(Duty_loke==0)
	{
		if(adc_number<=1.0f)
		{
			dut=100;
		}
		else if(adc_number>=3.0f)
		{
			dut=850;
		}
		else
		{
			dut=(((adc_number-1)/2)*(850-100))+100;
		}
	}
	__HAL_TIM_SetCompare(&htim2,TIM_CHANNEL_2,dut);
}
int pwm_up_tim=0,psc_number=10,psc_finish_up_flag=3;
void pwm_up(void)
{
	pwm_up_tim++;
	if(pwm_up_tim%6==0)
	{
		psc_number++;
		if(psc_number==19)
		{
			pwm_up_tim=0;
			psc_finish_up_flag=3;
			psc_number=19;
		}
	}
	__HAL_TIM_SET_PRESCALER(&htim2,psc_number);
}
int pwm_down_tim=0,psc_number_down=19,psc_finish_down_flag=3;
void pwm_down(void)
{
	pwm_down_tim++;
	if(pwm_down_tim%6==0)
	{
		psc_number_down--;
		if(psc_number_down==10)
		{
			pwm_down_tim=0;
			psc_finish_down_flag=3;
			psc_number_down=10;
		}
	}
	__HAL_TIM_SET_PRESCALER(&htim2,psc_number_down);
}




extern float speed;
int high_max_speed=0,high_max_speed_last=0;
int low_max_speed=0,low_max_speed_last=0;
void MAX_speed(void)
{
	if(frequency==1)
	{
		high_max_speed_last=high_max_speed;
		if(speed>high_max_speed) high_max_speed=speed;
	}
	if(frequency==0)
	{
		low_max_speed_last=low_max_speed;
		if(speed>low_max_speed) low_max_speed=speed;
	}
}

