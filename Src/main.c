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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "input.h"
#include "pulse_processor.h"
#include "circular_buffer.h"
#include "UART_Print.h"
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
uint16_t timestamp;
Input input;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
  UART_Send_Start();
  HAL_TIM_Base_Start(&htim16);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  BaseStationGeometryDef bs_0 = {{0.682646, 1.712605, 0.298152},
		  	  	  	  	  	  	 {0.356806, -0.017381, 0.934017, 0.001791, 0.999838, 0.017922, -0.934177, -0.004722, 0.356779}};

  BaseStationGeometryDef bs_1 = {{0.780941, 2.300994, -0.204002},
  	  	  	  	  	  	  	  	 {-0.184830, -0.411017, 0.892694, 0.104180, 0.895032, 0.433664, -0.977233, 0.173155, -0.122609}};

  SensorLocalGeometry s_loc_geo = {0, {0.0, 0.0, 0.0}};
  GeometryBuilder geometry_builder = {{bs_0, bs_1}, {s_loc_geo}};

  PulseProcessor pulse_processor = _PulseProcessor(1);
  pulse_processor.next = &geometry_builder;
  input.next = &pulse_processor;

  Pulse dummyPulses[] = {{0, 0, 65}, {0, 400, 84}, {0, 1500, 10},
		 				 {0, 8333, 65}, {0, 8733, 85}, {0, 12333, 15},
						 {0, 16666, 85}, {0, 17066, 66}, {0, 21666, 9},
						 {0, 24999, 84}, {0, 25399, 66}, {0, 27999, 10},

						 {0, 33332, 65}, {0, 33732, 84}, {0, 35332, 10},
						 {0, 41665, 65}, {0, 42065, 85}, {0, 44665, 15},
						 {0, 49998, 85}, {0, 50398, 66}, {0, 53998, 9},
						 {0, 58331, 84}, {0, 58731, 66}, {0, 63331, 10}};
  int i = 0;
  uint64_t timer = 0;
  while (1)
  {

	/*
	if(timestamp >= dummyPulses[i].start_time + dummyPulses[i].pulse_len){
		enqueue_pulse(&input, timestamp - dummyPulses[i].pulse_len, dummyPulses[i].pulse_len);
		i += 1;
	}

	do_work_input(&input, timestamp);
	do_work_pulse_processor(&pulse_processor, timestamp);
	timestamp += 1;
	*/

	do_work_input(&input, __HAL_TIM_GET_COUNTER(&htim16));
	do_work_pulse_processor(&pulse_processor, __HAL_TIM_GET_COUNTER(&htim16));


	if(timer % 65536 == 0){
		float x = geometry_builder.pos_.pos[0];
		float y = geometry_builder.pos_.pos[1];
		UART_Print_float(x);
	}
	timer += 1;
	//HAL_Delay(1000);

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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_USART1;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	//timestamp = HAL_GetTick();
	timestamp = __HAL_TIM_GET_COUNTER(&htim16);

	GPIO_PinState pin_state = HAL_GPIO_ReadPin(PC0_GPIO_Port, PC0_Pin);
	if (pin_state == GPIO_PIN_SET) {
		//Rising edge
		input.rise_time_ = timestamp;
		input.rise_valid_ = 1;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	}
	else if (input.rise_valid_ && pin_state == GPIO_PIN_RESET) {
		//Falling edge
		enqueue_pulse(&input, input.rise_time_, timestamp - input.rise_time_);
		input.rise_valid_ = 0;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	}
}

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
