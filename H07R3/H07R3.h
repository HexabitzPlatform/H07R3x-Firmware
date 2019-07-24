/*
    BitzOS (BOS) V0.1.6 - Copyright (C) 2017-2019 Hexabitz
    All rights reserved
		
    File Name     : H07R3.c
    Description   : Header file for module H07R3.
										Sound speaker (Knowles) with audio amp (TS4990IST) and headset port 
*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef H07R3_H
#define H07R3_H

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
#include "H07R3_MemoryMap.h"
#include "H07R3_uart.h"	
#include "H07R3_gpio.h"	
#include "H07R3_dma.h"		
#include "H07R3_dac.h"	

/* Exported definitions -------------------------------------------------------*/

#define	modulePN		_H07R3

/* Port-related definitions */
#define	NumOfPorts		5
#define P_PROG 				P2						/* ST factory bootloader UART */

/* Define available ports */
#define _P1 
#define _P2 
#define _P3 
#define _P4 
#define _P5

/* Define available USARTs */
#define _Usart1 1
#define _Usart2 1
#define _Usart3 1
#define _Usart4 1
#define _Usart5 1

/* Port-UART mapping */
#define P1uart &huart4	
#define P2uart &huart2	
#define P3uart &huart3
#define P4uart &huart1
#define P5uart &huart5

/* Port Definitions */
#define	USART1_TX_PIN		GPIO_PIN_9
#define	USART1_RX_PIN		GPIO_PIN_10
#define	USART1_TX_PORT	GPIOA
#define	USART1_RX_PORT	GPIOA
#define	USART1_AF				GPIO_AF1_USART1

#define	USART2_TX_PIN		GPIO_PIN_2
#define	USART2_RX_PIN		GPIO_PIN_3
#define	USART2_TX_PORT	GPIOA
#define	USART2_RX_PORT	GPIOA
#define	USART2_AF				GPIO_AF1_USART2

#define	USART3_TX_PIN		GPIO_PIN_10
#define	USART3_RX_PIN		GPIO_PIN_11
#define	USART3_TX_PORT	GPIOB
#define	USART3_RX_PORT	GPIOB
#define	USART3_AF				GPIO_AF4_USART3

#define	USART4_TX_PIN		GPIO_PIN_0
#define	USART4_RX_PIN		GPIO_PIN_1
#define	USART4_TX_PORT	GPIOA
#define	USART4_RX_PORT	GPIOA
#define	USART4_AF				GPIO_AF4_USART4

#define	USART5_TX_PIN		GPIO_PIN_3
#define	USART5_RX_PIN		GPIO_PIN_4
#define	USART5_TX_PORT	GPIOB
#define	USART5_RX_PORT	GPIOB
#define	USART5_AF				GPIO_AF4_USART5


/* Module-specific Definitions */
#define	_STDBY_PIN			GPIO_PIN_0
#define	_STDBY_PORT			GPIOB
#define	_DAC_PIN				GPIO_PIN_4
#define	_DAC_PORT				GPIOA

#define MusicNotesNumOfSamples	10

#define NUM_MODULE_PARAMS		1

/* H01R0_Status Type Definition */  
typedef enum 
{
  H07R3_OK = 0,
	H07R3_ERR_UnknownMessage = 1,
	H07R3_ERR_DATA,
	H07R3_ERR_PLAY,
	H07R3_ERROR = 255
} Module_Status;

/* Indicator LED */
#define _IND_LED_PORT		GPIOB
#define _IND_LED_PIN		GPIO_PIN_14

/* Exported variables */
extern const float notesFreq[12][9];
extern uint32_t NumberOfTuneWaves;
extern xTaskHandle AudioPlayTaskHandle;
extern xTaskHandle playTask;

/* Macros for TS4990 amplifier standby control */
#define TS4990_ENABLE()							HAL_GPIO_WritePin(_STDBY_PORT, _STDBY_PIN, GPIO_PIN_SET)
#define TS4990_DISABLE()						HAL_GPIO_WritePin(_STDBY_PORT, _STDBY_PIN, GPIO_PIN_SET)

#define BYTE static const uint8_t

/* Macros for musical notes */

#define C						0
#define DO					0
#define Csharp			1
#define DOsharp			1
#define D						2
#define RE					2
#define Dsharp			3
#define REsharp			3
#define E						4
#define MI					4
#define F						5
#define FA					5
#define Fsharp			6
#define FAsharp			6
#define G						7
#define SOL					7
#define Gsharp			8
#define SOLsharp		8
#define A						9
#define LA					9
#define Asharp			10
#define LAsharp			10
#define B						11
#define SI					11



#define C1(t)					PlaySine(notesFreq[0][0], MusicNotesNumOfSamples, t)
#define C2(t)					PlaySine(notesFreq[0][1], MusicNotesNumOfSamples, t)
#define C3(t)					PlaySine(notesFreq[0][2], MusicNotesNumOfSamples, t)
#define C4(t)					PlaySine(notesFreq[0][3], MusicNotesNumOfSamples, t)
#define C5(t)					PlaySine(notesFreq[0][4], MusicNotesNumOfSamples, t)	
#define C6(t)					PlaySine(notesFreq[0][5], MusicNotesNumOfSamples, t)
#define C7(t)					PlaySine(notesFreq[0][6], MusicNotesNumOfSamples, t)
#define C8(t)					PlaySine(notesFreq[0][7], MusicNotesNumOfSamples, t)
#define C9(t)					PlaySine(notesFreq[0][8], MusicNotesNumOfSamples, t)

#define Csharp1(t)		PlaySine(notesFreq[1][0], MusicNotesNumOfSamples, t)
#define Csharp2(t)		PlaySine(notesFreq[1][1], MusicNotesNumOfSamples, t)
#define Csharp3(t)		PlaySine(notesFreq[1][2], MusicNotesNumOfSamples, t)
#define Csharp4(t)		PlaySine(notesFreq[1][3], MusicNotesNumOfSamples, t)
#define Csharp5(t)		PlaySine(notesFreq[1][4], MusicNotesNumOfSamples, t)	
#define Csharp6(t)		PlaySine(notesFreq[1][5], MusicNotesNumOfSamples, t)
#define Csharp7(t)		PlaySine(notesFreq[1][6], MusicNotesNumOfSamples, t)
#define Csharp8(t)		PlaySine(notesFreq[1][7], MusicNotesNumOfSamples, t)
#define Csharp9(t)		PlaySine(notesFreq[1][8], MusicNotesNumOfSamples, t)

#define D1(t)					PlaySine(notesFreq[2][0], MusicNotesNumOfSamples, t)
#define D2(t)					PlaySine(notesFreq[2][1], MusicNotesNumOfSamples, t)
#define D3(t)					PlaySine(notesFreq[2][2], MusicNotesNumOfSamples, t)
#define D4(t)					PlaySine(notesFreq[2][3], MusicNotesNumOfSamples, t)
#define D5(t)					PlaySine(notesFreq[2][4], MusicNotesNumOfSamples, t)	
#define D6(t)					PlaySine(notesFreq[2][5], MusicNotesNumOfSamples, t)
#define D7(t)					PlaySine(notesFreq[2][6], MusicNotesNumOfSamples, t)
#define D8(t)					PlaySine(notesFreq[2][7], MusicNotesNumOfSamples, t)
#define D9(t)					PlaySine(notesFreq[2][8], MusicNotesNumOfSamples, t)

#define Dsharp1(t)		PlaySine(notesFreq[3][0], MusicNotesNumOfSamples, t)
#define Dsharp2(t)		PlaySine(notesFreq[3][1], MusicNotesNumOfSamples, t)
#define Dsharp3(t)		PlaySine(notesFreq[3][2], MusicNotesNumOfSamples, t)
#define Dsharp4(t)		PlaySine(notesFreq[3][3], MusicNotesNumOfSamples, t)
#define Dsharp5(t)		PlaySine(notesFreq[3][4], MusicNotesNumOfSamples, t)	
#define Dsharp6(t)		PlaySine(notesFreq[3][5], MusicNotesNumOfSamples, t)
#define Dsharp7(t)		PlaySine(notesFreq[3][6], MusicNotesNumOfSamples, t)
#define Dsharp8(t)		PlaySine(notesFreq[3][7], MusicNotesNumOfSamples, t)
#define Dsharp9(t)		PlaySine(notesFreq[3][8], MusicNotesNumOfSamples, t)

#define E1(t)					PlaySine(notesFreq[4][0], MusicNotesNumOfSamples, t)
#define E2(t)					PlaySine(notesFreq[4][1], MusicNotesNumOfSamples, t)
#define E3(t)					PlaySine(notesFreq[4][2], MusicNotesNumOfSamples, t)
#define E4(t)					PlaySine(notesFreq[4][3], MusicNotesNumOfSamples, t)
#define E5(t)					PlaySine(notesFreq[4][4], MusicNotesNumOfSamples, t)	
#define E6(t)					PlaySine(notesFreq[4][5], MusicNotesNumOfSamples, t)
#define E7(t)					PlaySine(notesFreq[4][6], MusicNotesNumOfSamples, t)
#define E8(t)					PlaySine(notesFreq[4][7], MusicNotesNumOfSamples, t)
#define E9(t)					PlaySine(notesFreq[4][8], MusicNotesNumOfSamples, t)

#define F1(t)					PlaySine(notesFreq[5][0], MusicNotesNumOfSamples, t)
#define F2(t)					PlaySine(notesFreq[5][1], MusicNotesNumOfSamples, t)
#define F3(t)					PlaySine(notesFreq[5][2], MusicNotesNumOfSamples, t)
#define F4(t)					PlaySine(notesFreq[5][3], MusicNotesNumOfSamples, t)
#define F5(t)					PlaySine(notesFreq[5][4], MusicNotesNumOfSamples, t)	
#define F6(t)					PlaySine(notesFreq[5][5], MusicNotesNumOfSamples, t)
#define F7(t)					PlaySine(notesFreq[5][6], MusicNotesNumOfSamples, t)
#define F8(t)					PlaySine(notesFreq[5][7], MusicNotesNumOfSamples, t)
#define F9(t)					PlaySine(notesFreq[5][8], MusicNotesNumOfSamples, t)

#define Fsharp1(t)		PlaySine(notesFreq[6][0], MusicNotesNumOfSamples, t)
#define Fsharp2(t)		PlaySine(notesFreq[6][1], MusicNotesNumOfSamples, t)
#define Fsharp3(t)		PlaySine(notesFreq[6][2], MusicNotesNumOfSamples, t)
#define Fsharp4(t)		PlaySine(notesFreq[6][3], MusicNotesNumOfSamples, t)
#define Fsharp5(t)		PlaySine(notesFreq[6][4], MusicNotesNumOfSamples, t)	
#define Fsharp6(t)		PlaySine(notesFreq[6][5], MusicNotesNumOfSamples, t)
#define Fsharp7(t)		PlaySine(notesFreq[6][6], MusicNotesNumOfSamples, t)
#define Fsharp8(t)		PlaySine(notesFreq[6][7], MusicNotesNumOfSamples, t)
#define Fsharp9(t)		PlaySine(notesFreq[6][8], MusicNotesNumOfSamples, t)

#define G1(t)					PlaySine(notesFreq[7][0], MusicNotesNumOfSamples, t)
#define G2(t)					PlaySine(notesFreq[7][1], MusicNotesNumOfSamples, t)
#define G3(t)					PlaySine(notesFreq[7][2], MusicNotesNumOfSamples, t)
#define G4(t)					PlaySine(notesFreq[7][3], MusicNotesNumOfSamples, t)
#define G5(t)					PlaySine(notesFreq[7][4], MusicNotesNumOfSamples, t)	
#define G6(t)					PlaySine(notesFreq[7][5], MusicNotesNumOfSamples, t)
#define G7(t)					PlaySine(notesFreq[7][6], MusicNotesNumOfSamples, t)
#define G8(t)					PlaySine(notesFreq[7][7], MusicNotesNumOfSamples, t)
#define G9(t)					PlaySine(notesFreq[7][8], MusicNotesNumOfSamples, t)

#define Gsharp1(t)		PlaySine(notesFreq[8][0], MusicNotesNumOfSamples, t)
#define Gsharp2(t)		PlaySine(notesFreq[8][1], MusicNotesNumOfSamples, t)
#define Gsharp3(t)		PlaySine(notesFreq[8][2], MusicNotesNumOfSamples, t)
#define Gsharp4(t)		PlaySine(notesFreq[8][3], MusicNotesNumOfSamples, t)
#define Gsharp5(t)		PlaySine(notesFreq[8][4], MusicNotesNumOfSamples, t)	
#define Gsharp6(t)		PlaySine(notesFreq[8][5], MusicNotesNumOfSamples, t)
#define Gsharp7(t)		PlaySine(notesFreq[8][6], MusicNotesNumOfSamples, t)
#define Gsharp8(t)		PlaySine(notesFreq[8][7], MusicNotesNumOfSamples, t)
#define Gsharp9(t)		PlaySine(notesFreq[8][8], MusicNotesNumOfSamples, t)

#define A1(t)					PlaySine(notesFreq[9][0], MusicNotesNumOfSamples, t)
#define A2(t)					PlaySine(notesFreq[9][1], MusicNotesNumOfSamples, t)
#define A3(t)					PlaySine(notesFreq[9][2], MusicNotesNumOfSamples, t)
#define A4(t)					PlaySine(notesFreq[9][3], MusicNotesNumOfSamples, t)
#define A5(t)					PlaySine(notesFreq[9][4], MusicNotesNumOfSamples, t)	
#define A6(t)					PlaySine(notesFreq[9][5], MusicNotesNumOfSamples, t)
#define A7(t)					PlaySine(notesFreq[9][6], MusicNotesNumOfSamples, t)
#define A8(t)					PlaySine(notesFreq[9][7], MusicNotesNumOfSamples, t)
#define A9(t)					PlaySine(notesFreq[9][8], MusicNotesNumOfSamples, t)

#define Asharp1(t)		PlaySine(notesFreq[10][0], MusicNotesNumOfSamples, t)
#define Asharp2(t)		PlaySine(notesFreq[10][1], MusicNotesNumOfSamples, t)
#define Asharp3(t)		PlaySine(notesFreq[10][2], MusicNotesNumOfSamples, t)
#define Asharp4(t)		PlaySine(notesFreq[10][3], MusicNotesNumOfSamples, t)
#define Asharp5(t)		PlaySine(notesFreq[10][4], MusicNotesNumOfSamples, t)	
#define Asharp6(t)		PlaySine(notesFreq[10][5], MusicNotesNumOfSamples, t)
#define Asharp7(t)		PlaySine(notesFreq[10][6], MusicNotesNumOfSamples, t)
#define Asharp8(t)		PlaySine(notesFreq[10][7], MusicNotesNumOfSamples, t)
#define Asharp9(t)		PlaySine(notesFreq[10][8], MusicNotesNumOfSamples, t)

#define B1(t)					PlaySine(notesFreq[11][0], MusicNotesNumOfSamples, t)
#define B2(t)					PlaySine(notesFreq[11][1], MusicNotesNumOfSamples, t)
#define B3(t)					PlaySine(notesFreq[11][2], MusicNotesNumOfSamples, t)
#define B4(t)					PlaySine(notesFreq[11][3], MusicNotesNumOfSamples, t)
#define B5(t)					PlaySine(notesFreq[11][4], MusicNotesNumOfSamples, t)	
#define B6(t)					PlaySine(notesFreq[11][5], MusicNotesNumOfSamples, t)
#define B7(t)					PlaySine(notesFreq[11][6], MusicNotesNumOfSamples, t)
#define B8(t)					PlaySine(notesFreq[11][7], MusicNotesNumOfSamples, t)
#define B9(t)					PlaySine(notesFreq[11][8], MusicNotesNumOfSamples, t)

/* Other macros */


/* Export UART variables */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

/* Define UART Init prototypes */
extern void MX_USART1_UART_Init(void);
extern void MX_USART2_UART_Init(void);
extern void MX_USART3_UART_Init(void);
extern void MX_USART4_UART_Init(void);
extern void MX_USART5_UART_Init(void);

#define AUDIO_DESC_QUE_SIZE								100

typedef struct AudioDesc_s {
	uint32_t *pBuffer;
	uint32_t lenOfBuf;
	uint32_t numOfBitsInACode;
	
	int32_t numOfRepeats;
	uint16_t delay;
	
	float rate;
} AudioDesc_t;

extern AudioDesc_t currentAudioDesc;

	
/* -----------------------------------------------------------------------
	|																APIs	 																 	|
   ----------------------------------------------------------------------- 
*/
bool PlayAudioNonBlock(AudioDesc_t *pDesc);
void PlayAudio(uint32_t *pBuffer, uint32_t length, uint32_t numOfRepeats, uint8_t dataPointSize, float rate);
bool PlaySine(float freq, uint16_t NumOfSamples, float durationInSeconds);
bool PlayWave(char *name, int32_t repeats, uint16_t delayInMs);

/* -----------------------------------------------------------------------
	|															Commands																 	|
   ----------------------------------------------------------------------- 
*/




#endif /* H07R3_H */

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
