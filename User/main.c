/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
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


/* Private variables ---------------------------------------------------------*/

/* Embedded wave variables */

uint32_t *waveAdd[MAX_WAVES] =  {
	(uint32_t *) &waveByteCode_HiThere[0],
	(uint32_t *) &waveByteCode_HowAreYou[0],
};

const uint32_t waveLength[MAX_WAVES] =  {
	WAVEBYTECODE_HITHERE_LENGTH,
	WAVEBYTECODE_HOWAREYOU_LENGTH,
};

const uint32_t waveResolution[MAX_WAVES] =  {
	WAVEBYTECODE_HITHERE_BPS,
	WAVEBYTECODE_HOWAREYOU_BPS,
};

const uint32_t waveRate[MAX_WAVES] =  {
	WAVEBYTECODE_HITHERE_SPS,
	WAVEBYTECODE_HOWAREYOU_SPS,
};

const char *waveName[MAX_WAVES] =  {		// Note wave name must be lower case
	"hithere",
	"howareyou",
};

/* Private function prototypes -----------------------------------------------*/



/* Main functions ------------------------------------------------------------*/

int main(void)
{


  /* MCU Configuration----------------------------------------------------------*/

  /* Reset all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all user peripherals */

	/* Initialize BitzOS */
	BOS_Init();

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  while (1)
  {


  }


}

#define SWTRIGR_CH1_Set ((uint32_t)0x00000000)
#define SWTRIGR_CH2_Set ((uint32_t)0x00000001)

uint8_t WaveBuff;
uint32_t* WaveBuff1;

typedef enum {
		WAVE_FILE_OK =1,
		H1BR6x_ID_NOT_FOUND,
		WAVE_FILE_NOT_FOUND,
		H1BR6x_NO_RESPONCE,
}SCAN_STATE;

HAL_StatusTypeDef result = HAL_OK;

bool PlayWaveFromPort(uint8_t _port)
{
		uint32_t WAVEcount=0;
		
		
		//DAC Stop DMA
		HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);	
		DAC_ChannelConfTypeDef sConfig;
		/* DAC Initialization */
		hdac.Instance = DAC;
		HAL_DAC_Init(&hdac);

		/* DAC channel OUT1 config */
		//sConfig.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
		//sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
		HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
	
		//wait H1BR6 to send 0xff to start reading from stream
		//UART_HandleTypeDef *huart=GetUart(_port);
	
		//huart->State = HAL_UART_STATE_READY;
		//result = HAL_UART_Receive(GetUart(_port), &WaveBuff, 1,1000);
		//if(result == HAL_ERROR || result == HAL_TIMEOUT )
		//		{return false;}
		
				IND_blink(100);
				result = HAL_OK;
	
		/* Setup Tim 6:*/
		__TIM6_CLK_ENABLE();	
		/* Peripheral configuration */	
		htim6.Instance = TIM6;
		htim6.Init.Prescaler = (uint16_t)(((SystemCoreClock / SAMPLERATE) / 2) - 1);
		htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim6.Init.Period = 1000000/SAMPLERATE;
		HAL_TIM_Base_Init(&htim6); 
			
		HAL_TIM_Base_Start(&htim6);			
		portENTER_CRITICAL();
				
		/* DAC Initialization */
		hdac.Instance = DAC;
		HAL_DAC_Init(&hdac);

		/* DAC channel OUT1 config */
		sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
		sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
					
		uint32_t alignment = DAC_ALIGN_8B_R;
		/* Start the DAC DMA */
		HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,(uint32_t *) &GetUart(_port)->Instance->RDR, WAVE_SIZE, alignment);
					
//		do{
//				WAVEcount++;
//				result = HAL_UART_Receive(GetUart(_port), GetUart(, 1,200);	
//					//*WaveBuff1	= (uint32_t) &WaveBuff;
//					//*WaveBuff1	= (uint32_t) (WaveBuff>>4);	//scale wave buffer from 8 bit to 12 bit
//				//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R ,(uint32_t) WaveBuff); 	/* Set WaveBuff to DAC data holding register */
//				//DAC->SWTRIGR |= SWTRIGR_CH1_Set; 	/* set software trigger; is automatically reset by hardware */
//			} while(1);//while(WAVE_SIZE - WAVEcount); //while(result!=HAL_TIMEOUT);
//		
//		//init DAC to default init
//		MX_DAC_Init();
		return true;
}
/**
	play Wave file from H1BR6 module
				send message to module H1BR6 to start streaming wave file to UART
				, and reseve wave sound data
* @param H1BR6x_ID : H1BR6x ID
					Reset H1BR6x_ID to zero for auto seach about H1BR6x ID from "topology.h" array
*/
bool PlayWaveFromModule(uint8_t H1BR6x_ID)
{	
		uint8_t port = 0;
		
			//search for H1BR6x ID in topology.h array
		if (H1BR6x_ID == 0)
		{
			for (uint8_t _ID=0 ; _ID<_N ; _ID++)
			{
				if (array[_ID][0] == _H1BR6)
					{H1BR6x_ID =  _ID+1;}				
			}
		}
			if (H1BR6x_ID == 0)
					{	return false;	}
	
		port = FindRoute(myID, H1BR6x_ID);
		if (port == NULL)	{return false;}
		
		if(WAVE_SIZE < 0xFFFFFFFE)
		{
			messageParams[0] = myID;
			messageParams[1] = port;
			SendMessageToModule(H1BR6x_ID, CODE_H1BR6_READ_WAVE, 2);
			
			return PlayWaveFromPort(port);
		}
		return false;
}

/**
	Scan Wave file in H1BR6 module
				send message to module H1BR6 for search for wave file by name of file
				, and wait for responce from H1BR6 module
				, try to get responce for three times
* @param Wave_Full_Name : Wave file name with or without file extension '.wav'
* @param H1BR6x_ID : H1BR6x ID
					Reset H1BR6x_ID to zero for auto seach about H1BR6x ID from "topology.h" array
*/
SCAN_STATE ScanWaveFile(char* Wave_Full_Name , uint8_t H1BR6x_ID)
{
	WAVE_SIZE = 0xFFFFFFFE;
	SAMPLERATE = 0xFFFFFFFF;
	uint32_t tickstart = 0;
	uint8_t size_of_wav = strlen(Wave_Full_Name);
	
	//search for H1BR6x ID in topology.h array
	if (H1BR6x_ID == 0)
	{
		for (uint8_t _ID=0 ; _ID<_N ; _ID++)
		{
			if (array[_ID][0] == _H1BR6)
				{H1BR6x_ID =  _ID+1;}				
		}
	}
	
		if (H1BR6x_ID == 0)
	{
		return H1BR6x_ID_NOT_FOUND;
	}
	
	for (uint16_t index=0 ; index <  size_of_wav ; index++)
			{	messageParams[index+1] = (uint8_t) 	Wave_Full_Name[index];}
	
				//try 3 times to get responce from H1BR6
	for (uint8_t _Ntry=1; _Ntry <= 5 ;_Ntry++)
	{				
			//1st param send H07R3 ID
			messageParams[0]=myID;
			//2nd param send wave file name
			//check the for file format ".wav" // .wave will not be sent
			if(Wave_Full_Name[size_of_wav-4]=='.' && Wave_Full_Name[size_of_wav-3]=='w' && Wave_Full_Name[size_of_wav-2]=='a' && Wave_Full_Name[size_of_wav-1]=='v')
					{ SendMessageToModule(H1BR6x_ID, CODE_H1BR6_SCAN_WAVE, size_of_wav-3);}
			else {		SendMessageToModule(H1BR6x_ID, CODE_H1BR6_SCAN_WAVE, size_of_wav+1);	}
			
			//wait for response
			tickstart = HAL_GetTick();
			//wait for Time (ms) until to get responce
			while((HAL_GetTick() - tickstart) < 1000)
			{
				if(WAVE_SIZE != 0xFFFFFFFE && WAVE_SIZE != 0xFFFFFFFF)
					{	
							IND_blink(100);
							return WAVE_FILE_OK; 
					}
			}
	}

	if(WAVE_SIZE == 0xFFFFFFFF)
			{ return WAVE_FILE_NOT_FOUND;}		//wave file not exist in H1BR6 module
	return H1BR6x_NO_RESPONCE;	
}	

/*-----------------------------------------------------------*/
SCAN_STATE responce;
/* User Task */
void UserTask(void * argument)
{

  /* Infinite loop */
	Delay_ms(1000);
	responce = ScanWaveFile("WAV1",1);
	Delay_ms(1000);
//	PlayWaveFromModule(1);

	
  for(;;)
  {
	}
}

/*-----------------------------------------------------------*/

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
