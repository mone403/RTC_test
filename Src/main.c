/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern RTC_HandleTypeDef hrtc;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char timebuff[15];
char datebuff[15];
char timerbuff[15];
char alarmbuff[15];

uint8_t alarm = 0;
int set = 0; // in clock
int mode = 0; // change mode
int set_timer = 0;
uint16_t setHours = 0x10; 
uint16_t setMinutes = 0x20; 
uint16_t setDate = 0x22; 
uint16_t setMonth = RTC_MONTH_NOVEMBER; 
uint16_t setYear = 0x19; 
uint16_t set_alarm_Hours = 0x10;
uint16_t set_alarm_minute = 0x22;
uint8_t timer_count = 0;
uint8_t timer_minute = 0;
uint8_t timer_second = 0;
uint8_t timerdown_minute = 0;
uint8_t timerdown_second = 0;
uint8_t backup_timer_minute;
uint8_t backup_timer_second ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void get_timer();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void set_time (void){
	
	RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
	
//	setHours = 0x10;
//	setMinutes = 0x20;
//	setDate = 0x22;
//	setMonth = RTC_MONTH_NOVEMBER;
//	setYear = 0x19;
	if(set == 1) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			setHours += 0x1;
			HAL_Delay(500);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
			setHours -= 0x1;
			HAL_Delay(500);
		}
		if(setHours > 0x23){
			setHours = 0x0;
		}
		if(setHours < 0x0) {
			setHours = 0x23;
		}
	}
	else if(set == 2) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			setMinutes += 0x1;
			HAL_Delay(500);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
			setMinutes -= 0x1;
			HAL_Delay(500);
		}
		if(setMinutes > 0x60){
			setMinutes = 0x0;
		}
		if(setMinutes < 0x0){
			setMinutes = 0x59;
		}
	}
	else if(set == 3) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			setDate += 0x1;
			HAL_Delay(500);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
			setDate -= 0x1;
			HAL_Delay(500);
		}
		if(setDate > 0x30){
			setDate = 0x1;
			HAL_Delay(500);
		}
		if(setDate < 0x1){
			setDate = 0x30;
			HAL_Delay(500);
		}
	}
	else if(set == 4) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
				setMonth += 0x1;
				HAL_Delay(500);
			}
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
				setMonth -= 0x1;
				HAL_Delay(500);
			}
			if(setMonth > 0x12){
				setMonth = 0x1;
			}
			if(setMonth < 0x1){
				setMonth = 0x12;
			}
	}
	else if(set==5){
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			setYear += 0x1;
			HAL_Delay(500);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1) {
			setYear -= 0x1;
			HAL_Delay(500);
		}
	}
	
	
	sTime.Hours = setHours ;
  sTime.Minutes = setMinutes;
  sTime.Seconds = 0x30;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 3 */

  /* USER CODE END RTC_Init 3 */

  sDate.WeekDay = RTC_WEEKDAY_FRIDAY;
	sDate.Month = setMonth ;
  sDate.Date = setDate;
  sDate.Year = setYear;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
	

	 /* USER CODE BEGIN RTC_Init 4 */
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2); // backup register
  /* USER CODE END RTC_Init 4 */
}

void set_alarm (void) 
{
	RTC_AlarmTypeDef sAlarm;
	
	if(set == 1) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			set_alarm_Hours += 0x1;
			HAL_Delay(500);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == 1) {
			set_alarm_Hours -= 0x1;
			HAL_Delay(500);
		}
		if(set_alarm_Hours > 0x23){
			set_alarm_Hours = 0x0;
		}
		if(set_alarm_Hours < 0x0) {
			set_alarm_Hours = 0x23;
		}
	}
	else if(set == 2) {
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			set_alarm_minute += 0x1;
			HAL_Delay(500);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == 1) {
			set_alarm_minute -= 0x1;
			HAL_Delay(500);
		}
		if(set_alarm_minute > 0x59){
			set_alarm_minute = 0x0;
		}
		if(set_alarm_minute < 0x0){
			set_alarm_minute = 0x59;
		}
	}
	
	sAlarm.AlarmTime.Hours = set_alarm_Hours;
  sAlarm.AlarmTime.Minutes = set_alarm_minute;
  sAlarm.AlarmTime.Seconds = 0x00;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = setDate;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
	
  /* USER CODE BEGIN RTC_Init 5 */

  /* USER CODE END RTC_Init 5 */
}

void timer_up (void) {
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
		{
			mode = 0;
		}
	
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET ){
		backup_timer_minute = timer_minute;
		backup_timer_second = timer_second;
		set = 4;
	}
	
	if(timer_count % 1000){
		timer_second += 1 ;
		if(timer_second%60 == 0) {
			timer_second = 0;
			timer_minute +=1;
		}
	}
	
}

void timer_down (void) {
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
		{
			mode = 0;
		}
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET ){
		backup_timer_minute = timer_minute;
		backup_timer_second = timer_second;
		set = 4;
	}
	
	if(timer_count % 1000){
		timer_second -= 1 ;
		if(timer_second%60 == 0) {
			if(timer_minute ==0 && timer_second == 0){
				timer_minute = 0;
				timer_second =0;
				backup_timer_minute = timer_minute;
				backup_timer_second = timer_second;
				set = 4;
			}
			timer_second = 59;
			timer_minute -=1;
		}
	}
}

void checkstate_timer (void) {
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
		{
			mode = 2 ;
		}
	
	if(set == 1){
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET ) {
			timer_minute += 1;
		} 
		else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET ) {
			timer_minute -=1;
		}
		if(timer_minute > 59) {
			timer_minute = 0;
		}
		if(timer_minute <0 ) {
			timer_minute = 59;
		}
		get_timer();
	}
	else if(set==2){
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET ) {
			timer_second += 1;
		} 
		else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_SET ) {
			timer_second -=1;
		}
		if(timer_second > 59) {
			timer_second = 0;
		}
		if(timer_second <0 ) {
			timer_second = 59;
		}
		get_timer();
	}
	else if (set==3) {
		if(timer_minute == 0 && timer_second == 0) 
		{
			timer_count = 0;
			while(set==3) {
				timer_up();
				get_timer();
			}
		} else {
			timer_count = 0;
			while(set==3){
				timer_down();
				get_timer();
			}
		}
	}
	else if (set == 4) {
		timer_count = 0;
		sprintf(timerbuff, "%02d : %02d\n\r" , backup_timer_minute, backup_timer_second);
		HAL_UART_Transmit(&huart3, (uint8_t*) timerbuff ,strlen(timerbuff),1000);
		HAL_Delay(1000);
	}
}



void get_timer(void) {
	sprintf(timerbuff, "%02d : %02d\r" , timer_minute, timer_second);
	HAL_UART_Transmit(&huart3, (uint8_t*) timerbuff ,strlen(timerbuff),1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart3, (uint8_t*) '\r' ,2,1000);
}

// current value
void get_time(void)
{
	
	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;
	// get RTC current time
	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	// get RTC current Date
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
	// format hh:mm:ss
	sprintf(timebuff, "%02d:%02d:%02d\n\r", gTime.Hours , gTime.Minutes, gTime.Seconds);
	// format dd - mm - yy
	sprintf(datebuff, "%02d-%02d-%2d\n\r", gDate.Date , gDate.Month, 2000 + gDate.Year);
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	
	alarm = 1;
}

void to_do_alarm (void) {
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == 0)
	{
		char str[] = "ALARM!!!";
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
		while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET) {}
		HAL_UART_Transmit(&huart3, (uint8_t*) str ,strlen(str),1000);
		HAL_Delay(1000);
		
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
	alarm = 0;

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
  MX_RTC_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	
	// back up register time not reset
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
	{
		set_time();
	}
//	set_time();
	set_alarm();
	char setbuff[5];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		get_time();
//		while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET) {}
//		HAL_UART_Transmit(&huart3, (uint8_t*) timebuff ,strlen(timebuff),1000);
//		while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET) {}
//		HAL_UART_Transmit(&huart3, (uint8_t*) datebuff ,strlen(datebuff),1000);
//		HAL_Delay(1000);
			
//		while(alarm == 1)
//		{
//			to_do_alarm();
//		}
//		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
//		{
//			mode += 1;
//			HAL_Delay(500);
//		}
		
		while(mode == 0){
			while (alarm == 1){
				to_do_alarm();
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
			{
				set = 1;
				mode = 1;
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 1)
			{
				set += 1;
				if(set > 5) {
					set = 0;
				}
			}
//			sprintf(setbuff, "set : %d\n\r", set);
//			HAL_UART_Transmit(&huart3, (uint8_t*) setbuff ,strlen(setbuff),1000);
//			HAL_Delay(1000);
			get_time();
			while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET) {}
			HAL_UART_Transmit(&huart3, (uint8_t*) timebuff ,strlen(timebuff),1000);
			while(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TC)==RESET) {}
			HAL_UART_Transmit(&huart3, (uint8_t*) datebuff ,strlen(datebuff),1000);
			HAL_Delay(1000);
		}
		// timer mode
		while(mode == 1){
			while (alarm == 1){
				to_do_alarm();
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
			{
				set = 1;
				mode = 2;
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_SET)
			{
				set++ ;
				if(set > 4) {
					set = 1;
				}
			}
			sprintf(setbuff, "set : %d\n\r", set);
			HAL_UART_Transmit(&huart3, (uint8_t*) setbuff ,strlen(setbuff),1000);
			checkstate_timer();

		}
		// set alarm
		while(mode == 2){
			while (alarm == 1){
				to_do_alarm();
			}
			sprintf(setbuff, "set : %d\n\r", set);
			HAL_UART_Transmit(&huart3, (uint8_t*) setbuff ,strlen(setbuff),1000);
			HAL_Delay(1000);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_SET)
			{
				set = 1;
				mode = 0;
			}
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == GPIO_PIN_SET)
			{
				set++ ;
				if(set > 2) {
					set = 1;
				}
			}
			
			set_alarm();
			sprintf(alarmbuff, "%02d:%02d", set_alarm_Hours, set_alarm_minute);
			HAL_UART_Transmit(&huart3, (uint8_t*) alarmbuff ,strlen(alarmbuff),1000);

		}
			
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART3;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
