/*
    BitzOS (BOS) V0.0.0 - Copyright (C) 2016 Hexabitz
    All rights reserved
		
    File Name     : H04R0.c
    Description   : Header file for module H04R0.
										Sound speaker (Knowles) with audio amp (TS4990IST) and headset port 
*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef H04R0_H
#define H04R0_H

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

/* Exported definitions -------------------------------------------------------*/

#define	modulePN		_H04R0

/* Define available ports */
#define _P1 
#define _P2 
#define _P4 
#define _P5 
#define _P6

/* Define available USARTs */
#define _Usart1 1
#define _Usart2 1
#define _Usart3 1
#define _Usart4 1
#define _Usart5 1
	
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

#define MusicNotesNumOfSamples	256

/* H01R0_Status Type Definition */  
typedef enum 
{
  H04R0_OK = 0,
	H04R0_ERR_UnknownMessage = 1,
} H04R0_Status;

/* Indicator LED */
#define _IND_LED_PORT		GPIOB
#define _IND_LED_PIN		GPIO_PIN_14

/* Exported variables */
extern const float notesFreq[12][9];
extern uint32_t NumberOfTuneWaves;

/* Macros for TS4990 amplifier standby control */
#define TS4990_ENABLE()			HAL_GPIO_WritePin(_STDBY_PORT,_STDBY_PIN,GPIO_PIN_SET)
#define TS4990_DISABLE()		HAL_GPIO_WritePin(_STDBY_PORT,_STDBY_PIN,GPIO_PIN_SET)

/* Macros for musical notes */
#define C1(t)		PlaySine(notesFreq[0][0], MusicNotesNumOfSamples, t)
#define C2(t)		PlaySine(notesFreq[0][1], MusicNotesNumOfSamples, t)
#define C3(t)		PlaySine(notesFreq[0][2], MusicNotesNumOfSamples, t)
#define C4(t)		PlaySine(notesFreq[0][3], MusicNotesNumOfSamples, t)
#define C5(t)		PlaySine(notesFreq[0][4], MusicNotesNumOfSamples, t)	
#define C6(t)		PlaySine(notesFreq[0][5], MusicNotesNumOfSamples, t)
#define C7(t)		PlaySine(notesFreq[0][6], MusicNotesNumOfSamples, t)
#define C8(t)		PlaySine(notesFreq[0][7], MusicNotesNumOfSamples, t)

#define Csharp1(t)		PlaySine(notesFreq[1][0], MusicNotesNumOfSamples, t)
#define Csharp2(t)		PlaySine(notesFreq[1][1], MusicNotesNumOfSamples, t)
#define Csharp3(t)		PlaySine(notesFreq[1][2], MusicNotesNumOfSamples, t)
#define Csharp4(t)		PlaySine(notesFreq[1][3], MusicNotesNumOfSamples, t)
#define Csharp5(t)		PlaySine(notesFreq[1][4], MusicNotesNumOfSamples, t)	
#define Csharp6(t)		PlaySine(notesFreq[1][5], MusicNotesNumOfSamples, t)
#define Csharp7(t)		PlaySine(notesFreq[1][6], MusicNotesNumOfSamples, t)
#define Csharp8(t)		PlaySine(notesFreq[1][7], MusicNotesNumOfSamples, t)

#define D1(t)		PlaySine(notesFreq[2][0], MusicNotesNumOfSamples, t)
#define D2(t)		PlaySine(notesFreq[2][1], MusicNotesNumOfSamples, t)
#define D3(t)		PlaySine(notesFreq[2][2], MusicNotesNumOfSamples, t)
#define D4(t)		PlaySine(notesFreq[2][3], MusicNotesNumOfSamples, t)
#define D5(t)		PlaySine(notesFreq[2][4], MusicNotesNumOfSamples, t)	
#define D6(t)		PlaySine(notesFreq[2][5], MusicNotesNumOfSamples, t)
#define D7(t)		PlaySine(notesFreq[2][6], MusicNotesNumOfSamples, t)
#define D8(t)		PlaySine(notesFreq[2][7], MusicNotesNumOfSamples, t)

#define Dsharp1(t)		PlaySine(notesFreq[3][0], MusicNotesNumOfSamples, t)
#define Dsharp2(t)		PlaySine(notesFreq[3][1], MusicNotesNumOfSamples, t)
#define Dsharp3(t)		PlaySine(notesFreq[3][2], MusicNotesNumOfSamples, t)
#define Dsharp4(t)		PlaySine(notesFreq[3][3], MusicNotesNumOfSamples, t)
#define Dsharp5(t)		PlaySine(notesFreq[3][4], MusicNotesNumOfSamples, t)	
#define Dsharp6(t)		PlaySine(notesFreq[3][5], MusicNotesNumOfSamples, t)
#define Dsharp7(t)		PlaySine(notesFreq[3][6], MusicNotesNumOfSamples, t)
#define Dsharp8(t)		PlaySine(notesFreq[3][7], MusicNotesNumOfSamples, t)

#define E1(t)		PlaySine(notesFreq[4][0], MusicNotesNumOfSamples, t)
#define E2(t)		PlaySine(notesFreq[4][1], MusicNotesNumOfSamples, t)
#define E3(t)		PlaySine(notesFreq[4][2], MusicNotesNumOfSamples, t)
#define E4(t)		PlaySine(notesFreq[4][3], MusicNotesNumOfSamples, t)
#define E5(t)		PlaySine(notesFreq[4][4], MusicNotesNumOfSamples, t)	
#define E6(t)		PlaySine(notesFreq[4][5], MusicNotesNumOfSamples, t)
#define E7(t)		PlaySine(notesFreq[4][6], MusicNotesNumOfSamples, t)
#define E8(t)		PlaySine(notesFreq[4][7], MusicNotesNumOfSamples, t)

#define F1(t)		PlaySine(notesFreq[5][0], MusicNotesNumOfSamples, t)
#define F2(t)		PlaySine(notesFreq[5][1], MusicNotesNumOfSamples, t)
#define F3(t)		PlaySine(notesFreq[5][2], MusicNotesNumOfSamples, t)
#define F4(t)		PlaySine(notesFreq[5][3], MusicNotesNumOfSamples, t)
#define F5(t)		PlaySine(notesFreq[5][4], MusicNotesNumOfSamples, t)	
#define F6(t)		PlaySine(notesFreq[5][5], MusicNotesNumOfSamples, t)
#define F7(t)		PlaySine(notesFreq[5][6], MusicNotesNumOfSamples, t)
#define F8(t)		PlaySine(notesFreq[5][7], MusicNotesNumOfSamples, t)

#define Fsharp1(t)		PlaySine(notesFreq[6][0], MusicNotesNumOfSamples, t)
#define Fsharp2(t)		PlaySine(notesFreq[6][1], MusicNotesNumOfSamples, t)
#define Fsharp3(t)		PlaySine(notesFreq[6][2], MusicNotesNumOfSamples, t)
#define Fsharp4(t)		PlaySine(notesFreq[6][3], MusicNotesNumOfSamples, t)
#define Fsharp5(t)		PlaySine(notesFreq[6][4], MusicNotesNumOfSamples, t)	
#define Fsharp6(t)		PlaySine(notesFreq[6][5], MusicNotesNumOfSamples, t)
#define Fsharp7(t)		PlaySine(notesFreq[6][6], MusicNotesNumOfSamples, t)
#define Fsharp8(t)		PlaySine(notesFreq[6][7], MusicNotesNumOfSamples, t)

#define G1(t)		PlaySine(notesFreq[7][0], MusicNotesNumOfSamples, t)
#define G2(t)		PlaySine(notesFreq[7][1], MusicNotesNumOfSamples, t)
#define G3(t)		PlaySine(notesFreq[7][2], MusicNotesNumOfSamples, t)
#define G4(t)		PlaySine(notesFreq[7][3], MusicNotesNumOfSamples, t)
#define G5(t)		PlaySine(notesFreq[7][4], MusicNotesNumOfSamples, t)	
#define G6(t)		PlaySine(notesFreq[7][5], MusicNotesNumOfSamples, t)
#define G7(t)		PlaySine(notesFreq[7][6], MusicNotesNumOfSamples, t)
#define G8(t)		PlaySine(notesFreq[7][7], MusicNotesNumOfSamples, t)

#define Gsharp1(t)		PlaySine(notesFreq[8][0], MusicNotesNumOfSamples, t)
#define Gsharp2(t)		PlaySine(notesFreq[8][1], MusicNotesNumOfSamples, t)
#define Gsharp3(t)		PlaySine(notesFreq[8][2], MusicNotesNumOfSamples, t)
#define Gsharp4(t)		PlaySine(notesFreq[8][3], MusicNotesNumOfSamples, t)
#define Gsharp5(t)		PlaySine(notesFreq[8][4], MusicNotesNumOfSamples, t)	
#define Gsharp6(t)		PlaySine(notesFreq[8][5], MusicNotesNumOfSamples, t)
#define Gsharp7(t)		PlaySine(notesFreq[8][6], MusicNotesNumOfSamples, t)
#define Gsharp8(t)		PlaySine(notesFreq[8][7], MusicNotesNumOfSamples, t)

#define A1(t)		PlaySine(notesFreq[9][0], MusicNotesNumOfSamples, t)
#define A2(t)		PlaySine(notesFreq[9][1], MusicNotesNumOfSamples, t)
#define A3(t)		PlaySine(notesFreq[9][2], MusicNotesNumOfSamples, t)
#define A4(t)		PlaySine(notesFreq[9][3], MusicNotesNumOfSamples, t)
#define A5(t)		PlaySine(notesFreq[9][4], MusicNotesNumOfSamples, t)	
#define A6(t)		PlaySine(notesFreq[9][5], MusicNotesNumOfSamples, t)
#define A7(t)		PlaySine(notesFreq[9][6], MusicNotesNumOfSamples, t)
#define A8(t)		PlaySine(notesFreq[9][7], MusicNotesNumOfSamples, t)

#define Asharp1(t)		PlaySine(notesFreq[10][0], MusicNotesNumOfSamples, t)
#define Asharp2(t)		PlaySine(notesFreq[10][1], MusicNotesNumOfSamples, t)
#define Asharp3(t)		PlaySine(notesFreq[10][2], MusicNotesNumOfSamples, t)
#define Asharp4(t)		PlaySine(notesFreq[10][3], MusicNotesNumOfSamples, t)
#define Asharp5(t)		PlaySine(notesFreq[10][4], MusicNotesNumOfSamples, t)	
#define Asharp6(t)		PlaySine(notesFreq[10][5], MusicNotesNumOfSamples, t)
#define Asharp7(t)		PlaySine(notesFreq[10][6], MusicNotesNumOfSamples, t)
#define Asharp8(t)		PlaySine(notesFreq[10][7], MusicNotesNumOfSamples, t)

#define B1(t)		PlaySine(notesFreq[11][0], MusicNotesNumOfSamples, t)
#define B2(t)		PlaySine(notesFreq[11][1], MusicNotesNumOfSamples, t)
#define B3(t)		PlaySine(notesFreq[11][2], MusicNotesNumOfSamples, t)
#define B4(t)		PlaySine(notesFreq[11][3], MusicNotesNumOfSamples, t)
#define B5(t)		PlaySine(notesFreq[11][4], MusicNotesNumOfSamples, t)	
#define B6(t)		PlaySine(notesFreq[11][5], MusicNotesNumOfSamples, t)
#define B7(t)		PlaySine(notesFreq[11][6], MusicNotesNumOfSamples, t)
#define B8(t)		PlaySine(notesFreq[11][7], MusicNotesNumOfSamples, t)


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




/* -----------------------------------------------------------------------
	|														Message Codes	 														 	|
   ----------------------------------------------------------------------- 
*/



	
/* -----------------------------------------------------------------------
	|																APIs	 																 	|
   ----------------------------------------------------------------------- 
*/

extern void H04R0_Init(void);
extern H04R0_Status H04R0_MessagingTask(uint16_t code, uint8_t port, uint8_t src, uint8_t dst);
extern void PlaySine(float freq, uint16_t NumOfSamples, float length);

/* -----------------------------------------------------------------------
	|															Commands																 	|
   ----------------------------------------------------------------------- 
*/




#endif /* H04R0_H */

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
