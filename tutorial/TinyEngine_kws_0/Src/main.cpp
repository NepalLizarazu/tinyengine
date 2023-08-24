/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   main.cpp
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *  - Chuang Gan, ganchuang@csail.mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#include "main.h"
//#include "camera.h"
#include "lcd.h"
#include "profile.h"
#include "stdio.h"
#include "string.h"
#include "testing_data/golden_data.h"
#include "testing_data/images.h"
extern "C" {
#include "genNN.h"
#include "audio_rec.h"
#include "main.h"
#include "tinyengine_function.h"
#include "MFCC.h"
#include "../Inc/arm_const_structs.h"
#include "CycleCounter.h"
//#include "core_cm7.h"
}
#define SHOWIMG

#include "stm32746g_discovery.h"

#include "dummy_inputs.h"
//#include "noised_inputs.h"
//#include "../AudioFiles/on.h"

static void MPU_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Input_Mode_Hint(void);
uint8_t CheckForUserInput(void);

static void CPU_CACHE_Enable(void);
static void MX_GPIO_Init(void);

#define IMAGE_H 80
#define IMAGE_W 80
#define INPUT_CH 160
#define OUTPUT_CH 10
#define IMAGES 6

void SystemClock_Config(void);
void StartDefaultTask(void const *argument);

signed char out_int[OUTPUT_CH];

float labels[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
char keywords[10][14] = { " KW: SILENCE ", " KW: UNKNOWN ", "   KW: YES   ",
		"   KW: NO    ", "   KW: UP    ", "  KW: DOWN   ", "  KW: LEFT   ",
		"  KW: RIGHT  ", "   KW: ON    ", "  KW: OFF   " };

//Modify this value: depending on the model
float x_scale = 0.6830914665670956; // Input scale calculated for GSC v2

int in_mode = 0; // 0 is using dummy inputs; 1 is using microphone
int t_mode = 0; //0 for Inference, 1 for Training
void train(int cls) {
	for (int i = 0; i < 10; i++) { //One-hot encoding
		if (i == cls) {
			labels[i] = 1.0f;
		} else {
			labels[i] = 0.0f;
		}
	}
	invoke(labels);
}

void invoke_new_weights_givenimg(signed char *out_int8) {
	invoke_inf();
	signed char *output = (signed char*) getOutput();
	for (int i = 0; i < 10; i++)
		out_int8[i] = output[i];
}

#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_10
#define BUTTON2_GPIO_Port GPIOF

#define GPIO_PORT GPIOG // Replace GPIOx with the actual GPIO port (e.g., GPIOA)
#define GPIO_PIN GPIO_PIN_7 // Replace GPIO_PIN_x with the actual GPIO pin number (e.g., GPIO_PIN_0)


#define RES_W 128
#define RES_H 120

#define ENABLE_TRAIN

#define LCD_BACKLIGHT_PIN GPIO_PIN_3
#define LCD_BACKLIGHT_PORT GPIOK

LTDC_HandleTypeDef hltdc;
int main(void) {
	char showbuf[150];
	static uint8_t first = 1;
	uint8_t in_idx = 0;

	/* Configure the MPU attributes */
	MPU_Config();
	CPU_CACHE_Enable();
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();

	ResetTimer(); //Cycle counter

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	lcdsetup();

	__set_CONTROL(0);
//   This code is for completely turning off the LCD Screen
//   __HAL_RCC_GPIOJ_CLK_ENABLE();
//
//	 GPIO_InitTypeDef GPIO_InitStruct;
//	 GPIO_InitStruct.Pin = LCD_BACKLIGHT_PIN;
//	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	 GPIO_InitStruct.Pull = GPIO_NOPULL;
//	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	 HAL_GPIO_Init(LCD_BACKLIGHT_PORT, &GPIO_InitStruct);
////	  Turn off the LCD backlight
//	 HAL_GPIO_WritePin(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_RESET);

//	 HAL_SuspendTick();
//	 HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
//	 while(1){}

	// Enable the clock for GPIO port G
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	// Configure PG7 as output
	GPIOG->MODER |= GPIO_MODER_MODER7_0;

	const uint8_t shift = 20; // 20 ms
	const uint8_t window_length = 40; // 40 ms
	const uint8_t num_mfcc_coef = 10;
	const uint16_t sampFreq = 16000;
	MFCCData mfcc_data = MFCCInitialize(window_length * 16, sampFreq,
			num_mfcc_coef); //640 samples window size, 16KHz sampling freq., and 10 MFCC features

	signed char *input = getInput(); // &buffer0[8000]

	int got_input = 0;
	int16_t *mono_audio;
	Input_Mode_Hint();
	while (1) {

		uint8_t button = BSP_PB_GetState(BUTTON_KEY) != RESET;

		char s[1];
		s[0] = 'c';
		recieveChar(s);
		if (s[0] == 'T') { 	//Training
			t_mode = 1;
			first = 1;
			got_input = 0;
			Input_Mode_Hint();

		} else if (s[0] == 'I') { 	//Inference
			t_mode = 0;
			got_input = 0;
			first = 1;
			in_idx = 0;
			Input_Mode_Hint();
		} else if (s[0] == '+') { 	//Go to next sample in dummy inputs
			in_idx++;
			if (in_idx == 20) {
				in_idx = 0;
			}
		} else if (s[0] == 'R') { 	//Input Mode: Record audio
			in_mode = 1;
			Input_Mode_Hint();
			got_input = 0;
		} else if (s[0] == 'D') {		//Input Mode: Dummy Inputs
			in_mode = 0;
			first = 1;
			Input_Mode_Hint();
		}

		static float x_scale = 0.6830914665670956; // Input scale calculated for GSC v2

		if (in_mode == 1) {
			if (button) {
				got_input = 1;
				Input_Mode_Hint();
//				int start_cycles = getCycles();
//				StartTimer();
				// Turn on the GPIO pin
				GPIO_PORT->BSRR = GPIO_PIN;
				mono_audio = AudioRec();
				// Turn off PG7
				GPIO_PORT->BSRR = GPIO_PIN << 16;
//				StopTimer();
//				int end_cycles = getCycles();
//				sprintf(showbuf, "%d", (end_cycles-start_cycles));
//				printLog(showbuf);
				Input_Mode_Hint();
				for (int i = 0; i < 320; i++) { //To eliminate the noise caused by pressing the button
					mono_audio[i] = 0;
				}
				//mono_audio = &audio_array[0];
				for (uint8_t i = 0; i < 49; i++) {
//					if (i == 25){
//						i = 25;
//					}
					MFCCCompute((int16_t*) (&mono_audio[shift * 16 * i]),
							mfcc_data);
					for (uint8_t j = 0; j < num_mfcc_coef; j++) {
						input[i * 10 + j] = (int8_t) (round(
								mfcc_data.melCoefficients[j] / x_scale) - 134);
					}
				}
			}
		} else if (in_mode == 0) {
			if (first || (s[0] == '+')) {
				first = 0;
				got_input = 1;
				//Converts one of the dummy inputs in a 1D array to input the inference
				for (int i = 0; i < 490; i++) {
					input[i] = (int8_t) (round(ins[in_idx][i] / x_scale) - 134);
				}
			}
		}

		if (t_mode) {
			if (got_input) {
				if ((s[0] == '0' || s[0] == '1' || s[0] == '2' || s[0] == '3'
						|| s[0] == '4' || s[0] == '5' || s[0] == '6'
						|| s[0] == '7' || s[0] == '8' || s[0] == '9')) {
					Input_Mode_Hint();
					BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 115,
							(char*) "  TRAINING MODE  ", CENTER_MODE);
					int label = 0;

					if (s[0] == '0') {
						label = 0;
						sprintf(showbuf, " Training KW 0 ");
					} else if (s[0] == '1') {
						label = 1;
						sprintf(showbuf, " Training KW 1 ");
					} else if (s[0] == '2') {
						label = 2;
						sprintf(showbuf, " Training KW 2 ");
					} else if (s[0] == '3') {
						label = 3;
						sprintf(showbuf, " Training KW 3 ");
					} else if (s[0] == '4') {
						label = 4;
						sprintf(showbuf, " Training KW 4 ");
					} else if (s[0] == '5') {
						label = 5;
						sprintf(showbuf, " Training KW 5 ");
					} else if (s[0] == '6') {
						label = 6;
						sprintf(showbuf, " Training KW 6 ");
					} else if (s[0] == '7') {
						label = 7;
						sprintf(showbuf, " Training KW 7 ");
					} else if (s[0] == '8') {
						label = 8;
						sprintf(showbuf, " Training KW 8 ");
					} else if (s[0] == '9') {
						label = 9;
						sprintf(showbuf, " Training KW 9 ");
					}
					start = HAL_GetTick();
					invoke_new_weights_givenimg(out_int);
					uint8_t inf_class = 0;

					for (int i = 0; i < OUTPUT_CH; i++) {
						if (out_int[inf_class] < out_int[i]) {
							inf_class = i;
						}
					}
					char showbuf2 [20];
					for (int i = 0; i < (10); i++){
						sprintf(showbuf2, "%d, ", (int)(out_int[i]) );
						printLog(showbuf2);
					}
					int answer_right = 0;

					if (label == inf_class) {
						answer_right = 1;
					}

					BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95,
							(char*) showbuf, CENTER_MODE);

					sprintf(showbuf, "Inferred Class: %d", inf_class);
					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80,
							(char*) showbuf, CENTER_MODE);

					sprintf(showbuf, "Ground Truth: %d", label);
					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65,
							(char*) showbuf, CENTER_MODE);

					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 50,
							(char*) "Training...", CENTER_MODE);

					start = HAL_GetTick();
					// Turn on PG7
//					GPIOG->BSRR = GPIO_PIN_7;
//					int start_cycles = getCycles();
//					StartTimer();
					train(label);
//					StopTimer();
//					int end_cycles = getCycles();
//					sprintf(showbuf, "%d", (end_cycles-start_cycles));
//					printLog(showbuf);
					// Turn off PG7
//					GPIOG->BSRR = GPIO_PIN_7 << 16;
					end = HAL_GetTick();

					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 50,
							(char*) "Training Done", CENTER_MODE);
					got_input = 0;
				}else{
					BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
					BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 150,
							(char*) "INPUT THE CLASS NUMBER TO TRAIN", CENTER_MODE);
				}
			}
		} else {
			if (got_input) {
				invoke_new_weights_givenimg(out_int);
				uint8_t inf_class = 0;

				for (int i = 0; i < OUTPUT_CH; i++) {
					if (out_int[inf_class] < out_int[i]) {
						inf_class = i;
					}
				}
				for (int i = 0; i < (10); i++){
					sprintf(showbuf, "%d, ", (int)(out_int[i]) );
					printLog(showbuf);
				}
				BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
				BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
				BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 115,
						(char*) "INFERENCE MODE", CENTER_MODE);

				sprintf(showbuf, "   Inferred Class: %d   ", inf_class);
				BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95,
						(char*) showbuf, CENTER_MODE);

				BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80,
						(char*) keywords[inf_class], CENTER_MODE);
				got_input = 0;
//
			}
		}
	}

	while (1) {
	}
}
void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if (ret != HAL_OK) {
		while (1) {
			;
		}
	}

	ret = HAL_PWREx_EnableOverDrive();
	if (ret != HAL_OK) {
		while (1) {
			;
		}
	}

	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if (ret != HAL_OK) {
		while (1) {
			;
		}
	}
}
static void Error_Handler(void) {

	BSP_LED_On(LED1);
	while (1) {
	}
}

static void CPU_CACHE_Enable(void) {

	SCB_EnableICache();

	SCB_EnableDCache();
}

static void MPU_Config(void) {
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU as Strongly ordered for not defined regions */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x00;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as WT for SDRAM */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0xC0000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU QSPI flash */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x90000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x0;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes FMC control registers */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0xA0000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_8KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER3;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x0;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin,
			GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOI, ARDUINO_D7_Pin | ARDUINO_D8_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(DCMI_PWR_EN_GPIO_Port, DCMI_PWR_EN_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin,
			GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ULPI_D7_Pin | ULPI_D6_Pin | ULPI_D5_Pin | ULPI_D3_Pin
			|
			ULPI_D2_Pin | ULPI_D1_Pin | ULPI_D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SPDIF_RX0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF8_SPDIFRX;
	HAL_GPIO_Init(SPDIF_RX0_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Audio_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = OTG_FS_P_Pin | OTG_FS_N_Pin | OTG_FS_ID_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ARDUINO_D7_Pin | ARDUINO_D8_Pin | LCD_DISP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = uSD_Detect_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TP3_Pin | NC2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ARDUINO_SCK_D13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(ARDUINO_SCK_D13_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DCMI_PWR_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DCMI_PWR_EN_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LCD_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ULPI_NXT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ULPI_STP_Pin | ULPI_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RMII_RXER_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RMII_RXER_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ULPI_CLK_Pin | ULPI_D0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ARDUINO_MISO_D12_Pin | ARDUINO_MOSI_PWM_D11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BUTTON1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(BUTTON1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BUTTON2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(BUTTON2_GPIO_Port, &GPIO_InitStruct);
}

uint8_t CheckForUserInput(void) {
	if (BSP_PB_GetState(BUTTON_KEY) != RESET) {
		HAL_Delay(10);
		while (BSP_PB_GetState(BUTTON_KEY) != RESET)
			;
		return 1;
	}
	return 0;
}

static void Input_Mode_Hint(void) {
	/* Set LCD Foreground Layer  */
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

	BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
	/* Clear the LCD */
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	/* Set Audio Demo description */
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
	BSP_LCD_SetFont(&Font24);
	if (in_mode == 0) {
		BSP_LCD_DisplayStringAt(0, 0, (char*) "USING GSC v2 INPUTS",
				CENTER_MODE);
	} else {
		BSP_LCD_SetFont(&Font24);
		BSP_LCD_DisplayStringAt(0, 0, (char*) "1 SEC AUDIO RECORDING",
				CENTER_MODE);
		BSP_LCD_SetFont(&Font12);
			BSP_LCD_DisplayStringAt(0, 60, (char*) "PRESS USER BUTTON TO RECORD NEW SAMPLE",
					CENTER_MODE);
	}
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_DisplayStringAt(0, 30, (char*) "ON-DEVICE KEY WORD SPOTTING",
			CENTER_MODE);

	/* Set the LCD Text Color */
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20,
			BSP_LCD_GetYSize() - 110);
	BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22,
			BSP_LCD_GetYSize() - 112);

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {

  while (1) {
  }
}
#endif
