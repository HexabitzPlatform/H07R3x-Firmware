/**
  ******************************************************************************
  * File Name          : H07R3_dac.c
  * Description        : This file provides code for the configuration
  *                      of the DAC instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/*
		MODIFIED by Hexabitz for BitzOS (BOS) V0.1.6 - Copyright (C) 2017-2019 Hexabitz
    All rights reserved
*/


/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

DAC_HandleTypeDef hdac;
TIM_HandleTypeDef htim6;	/* DAC update timer */
DMA_HandleTypeDef hdma_dac_ch1;

//static __IO uint16_t TIM6ARRValue = 1088;		// Need to check this value! Taken from ST EVAL


/* DAC init function */
void MX_DAC_Init(void)
{
  DAC_ChannelConfTypeDef sConfig;

  /* DAC Initialization */
  hdac.Instance = DAC;
  HAL_DAC_Init(&hdac);

  /* DAC channel OUT1 config */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  //sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);


}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	TIM_MasterConfigTypeDef sMasterConfig;
	
  if(hdac->Instance==DAC)
  {
    /* Peripheral clock enable */
    __DAC1_CLK_ENABLE();
  
    /**DAC GPIO Configuration    
    PA4     ------> DAC_OUT1 
    */
    GPIO_InitStruct.Pin = _DAC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_DAC_PORT, &GPIO_InitStruct);
		
		/* Timer 6 clock enable */
		__TIM6_CLK_ENABLE();
		
		/* Timer 6 configuration */
		htim6.Instance = TIM6;
		htim6.Init.Prescaler = 0;
		htim6.Init.CounterMode = TIM_COUNTERMODE_DOWN;
		htim6.Init.Period = 0;
		HAL_TIM_Base_Init(&htim6);

		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);
	
	  /* Start timer 6 */
    HAL_TIM_Base_Start(&htim6);
		
    /* DAC DMA 2 Ch 3 init */
    hdma_dac_ch1.Instance = DMA2_Channel3;
    hdma_dac_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_dac_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_dac_ch1.Init.Mode = DMA_CIRCULAR;
    hdma_dac_ch1.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma_dac_ch1);

    __HAL_DMA2_REMAP(HAL_DMA2_CH3_DAC_CH1);

    __HAL_LINKDMA(hdac,DMA_Handle1,hdma_dac_ch1);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
		
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
