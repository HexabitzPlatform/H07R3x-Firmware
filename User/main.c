/*
 BitzOS (BOS) V0.2.5 - Copyright (C) 2017-2021 Hexabitz
 All rights reserved

 File Name     : main.c
 Description   : Main program body.
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



/* Main function ------------------------------------------------------------*/

int main(void){

	Module_Init();		//Initialize Module &  BitzOS

	//Don't place your code here.
	for(;;){}
}

/*-----------------------------------------------------------*/

/* User Task */
void UserTask(void *argument){

	// put your code here, to run repeatedly.
	while(1){


	}
}

/*-----------------------------------------------------------*/
