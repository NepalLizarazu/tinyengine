/**
  ******************************************************************************
  * @file    BSP/Src/audio.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          stm32746g_discovery driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "audio_rec.h"
#include <stdio.h>
#include "string.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;

//#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE) //7FFE = 32K //FFFE = 64K //3FFE = 16K
#define AUDIO_BLOCK_SIZE   ((uint32_t)0x7FFE)
#define AUDIO_NB_BLOCKS    ((uint32_t)1)


//extern AUDIO_ErrorTypeDef AUDIO_Start(uint32_t audio_start_address, uint32_t audio_file_size);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];

/* Global variables ---------------------------------------------------------*/
uint32_t  audio_rec_buffer_state;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
int16_t* AudioRec (void)
{
  uint32_t  block_number=0;

  //AudioRec_SetHint();

  /* Initialize Audio Recorder */
  if (BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT OK  ", CENTER_MODE);
  }
  else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  audio_rec_buffer_state = BUFFER_OFFSET_NONE;

  /* Display the state on the screen */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"       RECORDING...     ", CENTER_MODE);

  /* Start Recording */
  BSP_AUDIO_IN_Record(internal_buffer, AUDIO_BLOCK_SIZE);

  for (block_number = 0; block_number < AUDIO_NB_BLOCKS; block_number++)
  {
    /* Wait end of half block recording */
    while(audio_rec_buffer_state != BUFFER_OFFSET_HALF){}
    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
    /* Copy recorded 1st half block in SDRAM */
    memcpy((uint32_t *)(AUDIO_REC_START_ADDR + (block_number * AUDIO_BLOCK_SIZE * 2)),
           internal_buffer,
           AUDIO_BLOCK_SIZE);

    /* Wait end of one block recording */
    while(audio_rec_buffer_state != BUFFER_OFFSET_FULL){}
    audio_rec_buffer_state = BUFFER_OFFSET_NONE;
    /* Copy recorded 2nd half block in SDRAM */
    memcpy((uint32_t *)(AUDIO_REC_START_ADDR + (block_number * AUDIO_BLOCK_SIZE * 2) + (AUDIO_BLOCK_SIZE)),
           (uint16_t *)(&internal_buffer[AUDIO_BLOCK_SIZE/2]),
           AUDIO_BLOCK_SIZE);
  }

  /* Stop recorder */
  BSP_AUDIO_IN_Stop(CODEC_PDWN_SW);

  //Transform into mono audio
  int16_t * recorded_audio = (int16_t *)AUDIO_REC_START_ADDR;
  for (int i = 0; i < AUDIO_BLOCK_SIZE/2; i++){
	  recorded_audio[i] = ( recorded_audio[i*2] + recorded_audio[(i*2)+1] )/2;
  }


  return (int16_t *)AUDIO_REC_START_ADDR;
}

/**
  * @brief  Display Audio Record demo hint
  * @param  None
  * @retval None
  */
void AudioRec_SetHint(void)
{
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
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"1 SEC AUDIO RECORDING", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"ON-DEVICE KEY WORD SPOTTING", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);

}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32746g_discovery_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief Manages the DMA Transfer complete interrupt.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_FULL;
  return;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_HALF;
  return;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_CallBack(void)
{
  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
  /* Display message on the LCD screen */
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_KEY) != RESET)
  {
    return;
  }
  /* could also generate a system reset to recover from the error */
  /* .... */
}



/**
  * @}
  */

/**
  * @}
  */

