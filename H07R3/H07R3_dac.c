/*
 BitzOS (BOS) V0.2.6 - Copyright (C) 2017-2022 Hexabitz
 All rights reserved

   File Name          : H07R3_dac.c
   Description        : This file provides code for the configuration
                        of the DAC instances.

*/


/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

DAC_HandleTypeDef hdac;
TIM_HandleTypeDef htim2;	/* DAC update timer */
DMA_HandleTypeDef hdma_dac_ch1;

/* DAC init function */
void MX_DAC_Init(void)
{
  DAC_ChannelConfTypeDef sConfig;

  /* DAC Initialization */
  hdac.Instance = DAC;
  HAL_DAC_Init(&hdac);

  /* DAC channel OUT1 config */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
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
		
		/* Timer 2 clock enable */
		__TIM2_CLK_ENABLE();
		
		/* Timer 2 configuration */
		htim2.Instance = TIM2;
		htim2.Init.Prescaler = 0;
		htim2.Init.CounterMode = TIM_COUNTERMODE_DOWN;
		htim2.Init.Period = 0;
		HAL_TIM_Base_Init(&htim2);

		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
	  /* Start timer 2 */
    HAL_TIM_Base_Start(&htim2);
		
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
