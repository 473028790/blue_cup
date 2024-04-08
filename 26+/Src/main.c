/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "stdio.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t  cc1_value_2 = 0; 
uint32_t  f40 = 0;
float speed=0;
extern int R,K;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	cc1_value_2 = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_2);
//  cc1_value_2 = __HAL_TIM_GET_COUNTER(&htim3);

	__HAL_TIM_SetCounter(&htim3,0);
	f40 = 1000000/cc1_value_2;
	speed=(f40*2*3.14*R)/100*K;
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 500);
	HAL_TIM_Base_Start_IT(&htim4);
		__HAL_TIM_CLEAR_IT(&htim3,TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
		__HAL_TIM_CLEAR_IT(&htim3,TIM_IT_UPDATE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int lcd_interface=0;
char text_title[30];
int cnt=20;
float cnt1=20.0;
uint8_t frequency=0;//0 is low
int R=1,K=1;
extern int high_or_low_number;
int dut=0;
extern int high_max_speed,low_max_speed;
int tongji_tim=0;
extern uint8_t Duty_loke;
extern int psc_finish_down_flag,psc_finish_up_flag;
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM8 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM8) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM4) {
		tongji_tim++;
		if(tongji_tim%50==0)
		{
			MAX_speed();
		}
		
		if(lcd_interface == 0)
		{
			sprintf(text_title,"        DATA          ");
			LCD_DisplayStringLine(Line1, (uint8_t *)text_title);
			if(frequency==0)
			{
				sprintf(text_title,"   M = L              ");
				LCD_DisplayStringLine(Line3, (uint8_t *)text_title);
			}
			else if(frequency==1)
			{
				sprintf(text_title,"   M = H              ");
				LCD_DisplayStringLine(Line3, (uint8_t *)text_title);
			}
			sprintf(text_title,"   P = %d%%          ",dut/10);
			LCD_DisplayStringLine(Line4, (uint8_t *)text_title);
			sprintf(text_title,"   V = %.1f          ",speed);
			LCD_DisplayStringLine(Line5, (uint8_t *)text_title);
						sprintf(text_title,"                       ");
			LCD_DisplayStringLine(Line6, (uint8_t *)text_title);
		}
		else if(lcd_interface == 1)
		{
			sprintf(text_title,"        PARA          ");
			LCD_DisplayStringLine(Line1, (uint8_t *)text_title);
			sprintf(text_title,"                       ");
			LCD_DisplayStringLine(Line3, (uint8_t *)text_title);
			sprintf(text_title,"   R = %d          ",R);
			LCD_DisplayStringLine(Line4, (uint8_t *)text_title);
			sprintf(text_title,"   K = %d          ",K);
			LCD_DisplayStringLine(Line5, (uint8_t *)text_title);
						sprintf(text_title,"                       ");
			LCD_DisplayStringLine(Line6, (uint8_t *)text_title);
		}
		else if(lcd_interface == 2)
		{
			sprintf(text_title,"        RECD          ");
			LCD_DisplayStringLine(Line1, (uint8_t *)text_title);
			sprintf(text_title,"   N = %d          ",high_or_low_number);
			LCD_DisplayStringLine(Line4, (uint8_t *)text_title);
			sprintf(text_title,"   MH = %d          ",high_max_speed);
			LCD_DisplayStringLine(Line5, (uint8_t *)text_title);
			sprintf(text_title,"   ML = %d          ",low_max_speed);
			LCD_DisplayStringLine(Line6, (uint8_t *)text_title);
						sprintf(text_title,"                       ");
			LCD_DisplayStringLine(Line3, (uint8_t *)text_title);
		}

		key_proc();
		key_task();
		adc_task();
		
		if(lcd_interface == 0)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		}
		if(Duty_loke==1) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
		else HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);
		
		if(psc_finish_down_flag==0 || psc_finish_up_flag==0)
		{
			if(tongji_tim%10==0)
			{
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
			}
		}
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
		
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
