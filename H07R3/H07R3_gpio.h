/*
 BitzOS (BOS) V0.2.6 - Copyright (C) 2017-2022 Hexabitz
 All rights reserved

 File Name     : H07R3_gpio.h
 Description   : Header file contains all the functions prototypes for
 the GPIO .

 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __H07R3_gpio_H
#define __H07R3_gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"


extern void GPIO_Init(void);
extern void IND_LED_Init(void);
	 


#ifdef __cplusplus
}
#endif
#endif /*__H07R3_gpio_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
