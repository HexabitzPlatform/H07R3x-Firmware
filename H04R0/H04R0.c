/*
    BitzOS (BOS) V0.0.0 - Copyright (C) 2016 Hexabitz
    All rights reserved

    File Name     : H04R0.c
    Description   : Source code for module H04R0.
										Sound speaker (Knowles) with audio amp (TS4990IST) and headset port 
		
		Required MCU resources : 
		
			>> USARTs 1,2,3,4,5 for module ports.
			>> DMA1 Ch5, DMA1 Ch6, DMA2 Ch2 for port-to-memory messaging.
			>> DMA1 Ch1, DMA2 Ch3 for port-to-port streaming.
			>> DAC_OUT 1 for TS4990IST.
			>> DMA1 Ch3 for DAC Ch 1 DMA.
			>> PB0 for TS4990IST STDBY.
			>> Timer 6 for DAC Ch 1 trigger.
			
*/
	
/* Includes ------------------------------------------------------------------*/
#include "BOS.h"


uint32_t NumberOfTuneWaves = 0;


/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;


/* Private variables ---------------------------------------------------------*/

/* 10-amplitude-sample sine wave: 1.650, 2.620, 3.219, 3.220, 2.622, 1.653, 0.682, 0.081, 0.079, 0.676 */
//const uint16_t sineDigital[10] = {2048, 3251, 3995, 3996, 3253, 2051, 847, 101, 98, 839};

/* 100 samples sine wave <<http://www.daycounter.com/Calculators/Sine-Generator-Calculator2.phtml>> */
const uint16_t sineDigital[100] = {2048,2176,2304,2431,2557,2680,2801,2919,3034,3145,3251,3353,3449,3540,3625,3704,3776,3842,3900,3951,
3995,4031,4059,4079,4091,4095,4091,4079,4059,4031,3995,3951,3900,3842,3776,3704,3625,3540,3449,3353,
3251,3145,3034,2919,2801,2680,2557,2431,2304,2176,2048,1919,1791,1664,1538,1415,1294,1176,1061,950,
844,742,646,555,470,391,319,253,195,144,100,64,36,16,4,0,4,16,36,64,
100,144,195,253,319,391,470,555,646,742,844,950,1061,1176,1294,1415,1538,1664,1791,1919};


/* Musical notes: 12 Tone Equal Tempered Scale. Frequency Table based on A4 = 440Hz 
   (Middle C = C4)
									Octave Number: 		0				1			 2			 3			4				 5			 6				7				8
*/
const float notesFreq[12][9] = {	{16.35, 32.70, 65.41, 130.81, 261.63, 523.25, 1046.50, 2093.00, 4186.01},			// c
																	{17.32,	34.65, 69.30,	138.59,	277.18,	554.37,	1108.73, 2217.46,	4434.92},			// c#
																	{18.35,	36.71, 73.42,	146.83,	293.66,	587.33,	1174.66, 2349.32,	4698.64},			// D
																	{19.45,	38.89, 77.78,	155.56,	311.13,	622.25,	1244.51, 2489.02,	4978.03},			// D#
																	{20.60,	41.20, 82.41,	164.81,	329.63,	659.26,	1318.51, 2637.02,	5274.04},			// E
																	{21.83,	43.65, 87.31,	174.61,	349.23,	698.46,	1396.91, 2793.83,	5587.65},			// F		
																	{23.12,	46.25, 92.50,	185.00,	369.99,	739.99,	1479.98, 2959.96,	5919.91},			// F#
																	{24.50,	49.00, 98.00,	196.00,	392.00,	783.99,	1567.98, 3135.96,	6271.93},			// G
																	{25.96,	51.91, 103.83, 207.65, 415.30, 830.61, 1661.22,	3322.44, 6644.88},		// G#
																	{27.50,	55.00, 110.00, 220.00, 440.00, 880.00, 1760.00,	3520.00, 7040.00},		// A
																	{29.14,	58.27, 116.54, 233.08, 466.16, 932.33, 1864.66,	3729.31, 7458.62},		// A#
																	{30.87,	61.74, 123.47, 246.94, 493.88, 987.77, 1975.53,	3951.07, 7902.13} };	// B


/* Private function prototypes -----------------------------------------------*/	


/* Create CLI commands --------------------------------------------------------*/




/* -----------------------------------------------------------------------
	|												 Private Functions	 														|
   ----------------------------------------------------------------------- 
*/

/* --- H04R0 message processing task. 
*/
H04R0_Status H04R0_MessagingTask(uint16_t code, uint8_t port, uint8_t src, uint8_t dst)
{
	H04R0_Status result = H04R0_OK;
	
	switch (code)
	{

		default:
			result = H04R0_ERR_UnknownMessage;
			break;
	}			

	return result;	
}

/*-----------------------------------------------------------*/



/* -----------------------------------------------------------------------
	|																APIs	 																 	|
   ----------------------------------------------------------------------- 
*/

/* --- H04R0 module initialization. 
*/
void H04R0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Array ports */
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  MX_USART5_UART_Init();
	
	/* DAC */
	MX_DAC_Init();
	
	/* GPIO */
	GPIO_InitStruct.Pin = _STDBY_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(_STDBY_PORT, &GPIO_InitStruct);
  
}
/*-----------------------------------------------------------*/

/* --- Play a pure sine wave (minimum 2.8 Hz at 255 samples). 
*/
void PlaySine(float freq, uint16_t NumOfSamples, float length)
{
	/* Timer trigger frequency */
	float ftrg = freq * NumOfSamples;
	
	/* Number of waves */	
	NumberOfTuneWaves = length * freq;
	
	/* Setup Tim 6: Prescaler = (SystemCoreClock / TIM2 trigger clock) - 1, ARR = TIM2 trigger clock - 1 */
	HAL_TIM_Base_Stop(&htim6);
	htim6.Instance->ARR = 1;
	htim6.Instance->PSC = (uint16_t) ( ((SystemCoreClock / ftrg) / 2) - 1);
	HAL_TIM_Base_Start(&htim6);
	
	/* Start the DAC DMA */
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)sineDigital, NumOfSamples, DAC_ALIGN_12B_R); 
		
	/* Wait indefinitly until DMA transfer is finished */
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	
	/* Reset the DAC DMA and trigger timer */
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
	HAL_TIM_Base_Stop(&htim6);
	NumberOfTuneWaves = 0;

}

/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|															Commands																 	|
   ----------------------------------------------------------------------- 
*/



/*-----------------------------------------------------------*/


/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
