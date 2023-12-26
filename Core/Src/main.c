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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "Accelerometer.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#define SCREEN_WIDTH 240
//#define SCREEN_HEIGHT 320
#define LINE_THICKNESS 10

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

const int SCREEN_HEIGHT = 320;
const int SCREEN_WIDTH = 240;
int matrix[80][60];
int goal[2] = {0, 0};

int positionsX[600];
int positionsY[600];
//int graint_flag = 0;
//int position_flag = 1;

uint16_t y = 0;
uint16_t x = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void randomSortCurrentPositions(int positionsX[], int positionsY[], int size) {
    srand(time(NULL));

    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        int tempX = positionsX[i];
        positionsX[i] = positionsX[j];
        positionsX[j] = tempX;

		int tempY = positionsY[i];
		positionsY[i] = positionsY[j];
		positionsY[j] = tempY;
    }
}

void randomSortNextPositions(int possible_positions[][2], int size) {
    srand(time(NULL));

    for (int i = size-1; i > 0; --i) {
            int j = rand() % (i + 1);

            int tempY = possible_positions[i][0];
            int tempX = possible_positions[i][1];

            possible_positions[i][0] = possible_positions[j][0];
            possible_positions[i][1] = possible_positions[j][1];

            possible_positions[j][0] = tempY;
            possible_positions[j][1] = tempX;
        }
}


void DrawBigX(uint16_t x, uint16_t y, uint16_t size, uint16_t thickness, uint16_t color) {
  uint16_t i, j;

  for (j = 0; j < thickness; j++) {
    for (i = 0; i < size; i++) {
      ILI9341_DrawPixel(x + i, y + i + j, color);
      if ((y + i + j)/4 >= 40) {
    	  matrix[(y + i + j)/4-1][(x + i)/4] = 2;
      }
      else matrix[(y + i + j)/4][(x + i)/4] = 2;

    }
  }

  for (j = 0; j < thickness; j++) {
    for (i = 0; i < size; i++) {
	  ILI9341_DrawPixel(SCREEN_WIDTH - i-1, y + i + j, color);
	  if ((y + i + j)/4 >= 40) {
		  matrix[(y + i + j)/4-1][(SCREEN_WIDTH - i)/4] = 2;
	  }
	  else matrix[(y + i + j)/4][(SCREEN_WIDTH - i)/4] = 2;

    }
  }

  for (j = 38; j < 44; j++) {
	  for (i = 29; i < 31; i++) {
		  matrix[j][i] = 0;
		  ILI9341_FillRectangle(i*4, j*4, 4, 4, ILI9341_WHITE);
	  }
  }

}

void DrawCenteredBigX(uint16_t color) {
  uint16_t size = SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_HEIGHT : SCREEN_WIDTH;
  uint16_t x = (SCREEN_WIDTH - size) / 2;
  uint16_t y = (SCREEN_HEIGHT - size) / 2;

  DrawBigX(x, y, size, LINE_THICKNESS, color);
}

void FillMatrix() {
	int idx = 0;
	for (size_t i = 0; i < 80; i++) {
		for (size_t j = 0; j < 60; j++) {
			if (i < 10) {
				positionsY[idx] = i;
				positionsX[idx] = j;
				matrix[i][j] = 1;
				idx++;
			} else matrix[i][j] = 0;
		}
	}

}

void RenderMatrix() {
	for (int i = 0; i < SCREEN_HEIGHT ; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			if (matrix[i][j] == 1) ILI9341_FillRectangle(j*4, i*4, 4, 4, ILI9341_RED);
			else if (matrix[i][j] == 0) ILI9341_FillRectangle(j*4, i*4, 4, 4, ILI9341_WHITE);
		}
	}
}



void SetGoal(int *goal_y, int * goal_x) {
	y = ((ReadFromAccelerometer(0x2B) << 8) | ReadFromAccelerometer(0x2A));
	x = ((ReadFromAccelerometer(0x29) << 8) | ReadFromAccelerometer(0x28));

	if ( x < 40000 && x > 5000) {

		if (y < 2000 || y > 64000) {
			*goal_y = 105;
			*goal_x = 30;
		}
		else if (y >= 60000 && y <= 64000) {
			*goal_y = 105;
			*goal_x = 45;
		}
		else if ( y >= 56000 && y <= 59999 ) {
			*goal_y = 80;
			*goal_x = 60;
		}
		else if ( y >= 50000 && y <= 55999 ) {
			*goal_y = 40;
			*goal_x = 1;
		}

		else if ( y >= 2000 && y <= 7000) {
			*goal_y = 105;
			*goal_x = 15;
		}
		else if (y >= 7001 && y <=13000) {
			*goal_y = 80;
			*goal_x = 00;
		}
		else if (y >= 13001 && y <=16000) {
			*goal_y = 40;
			*goal_x = -1;
		}

	}
}

void NextPosition(int y, int x, int *result_y, int *result_x) {
    int possible_positions[8][2] = {{y - 1, x}, {y - 1, x - 1}, {y - 1, x + 1},
                                    {y, x - 1}, {y, x + 1}, {y + 1, x}, {y + 1, x - 1},
									{y + 1, x + 1}};
    randomSortNextPositions(possible_positions, 8);

    int closest_pos[2] = {y, x};
    double min_dist = (goal[0] - closest_pos[0]) * (goal[0] - closest_pos[0]) +
    			(goal[1] - closest_pos[1]) * (goal[1] - closest_pos[1]);

    for (int i = 0; i < 8; i++) {
        int pos[2] = {possible_positions[i][0], possible_positions[i][1]};
        if ( (pos[0] >= 0 && pos[0] < 80) && (pos[1] >= 0 && pos[1] < 60 &&
        		matrix[pos[0]][pos[1]] == 0) ) {
        	double dist = (goal[0] - pos[0]) * (goal[0] - pos[0]) +
							(goal[1] - pos[1]) * (goal[1] - pos[1]);

			if (dist < min_dist) {
				min_dist = dist;
				closest_pos[0] = pos[0];
				closest_pos[1] = pos[1];
			}
        }
    }

    *result_y = closest_pos[0];
    *result_x = closest_pos[1];
}




void RunEpoch(){
	int goal_x = 0;
	int goal_y = 0;
	SetGoal(&goal_y, &goal_x);

	goal[0] = goal_y;
	goal[1] = goal_x;

	for (int i = 0; i < 600; i ++) {
		int y = positionsY[i];
		int x = positionsX[i];
		int result_y = 0;
		int result_x = 0;
		NextPosition(y, x, &result_y, &result_x);

		if ( y != result_y || x != result_x) {
			ILI9341_FillRectangle(x*4, y*4, 4, 4, ILI9341_WHITE);
			ILI9341_FillRectangle(result_x*4, result_y*4, 4, 4, ILI9341_RED);
			matrix[y][x] = 0;
			matrix[result_y][result_x] = 1;

			positionsY[i] = result_y;
			positionsX[i] = result_x;
		}
	}

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
  MX_SPI1_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  AccelerometerInit();
  HAL_Delay(100);

  ILI9341_Init();
  ILI9341_FillScreen(ILI9341_WHITE);
  FillMatrix();
  DrawCenteredBigX(ILI9341_BLACK);
  RenderMatrix();
  randomSortCurrentPositions(positionsX, positionsY, 600);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	RunEpoch();
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISP_RESET_Pin|DISP_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DISP_RESET_Pin DISP_DC_Pin */
  GPIO_InitStruct.Pin = DISP_RESET_Pin|DISP_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
