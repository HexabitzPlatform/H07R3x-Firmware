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
		MODIFIED by Hexabitz for BitzOS (BOS) V0.1.3 - Copyright (C) 2017 Hexabitz
    All rights reserved
*/

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
#include "wave.h"


/* Private variables ---------------------------------------------------------*/


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
  while (1) {


  }
}

/*-----------------------------------------------------------*/

/* FrontEndTask function */
void FrontEndTask(void * argument)
{
  /* Infinite loop */
  for(;;) {
		
#if 0
		
		C8(1); D8(1); E8(1); F8(1); G8(1); A8(1); B8(1);
		
		Delay_ms(1000);
		
		B8(1); A8(1); G8(1); F8(1); E8(1); D8(1); C8(1);
		
		Delay_ms(1000);
		
		/* Mary had a little lamb! */
		
		E7(0.5); D7(0.5); C7(0.5); D7(0.5); E7(0.5); E7(0.5); E7(1);
		
		D7(0.5); D7(0.5); D7(1); E7(0.5); G7(0.5); G7(1);
		
		E7(0.5); D7(0.5); C7(0.5); D7(0.5); E7(0.5); E7(0.5); E7(0.5); E7(0.5);
		
		D7(0.5); D7(0.5); E7(0.5); D7(0.5); C7(1); 
		
#endif

#if 0
		PlayWave((uint8_t *)waveByteCode_HiThere, WAVEBYTECODE_HITHERE_LENGTH, 16000);
		Delay_ms(500);
		PlayWave((uint8_t *)waveByteCode_HowAreYou, WAVEBYTECODE_HOWAREYOU_LENGTH, 16000);
		
		Delay_ms(3000);
#endif
	}
}


/*-----------------------------------------------------------*/

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
