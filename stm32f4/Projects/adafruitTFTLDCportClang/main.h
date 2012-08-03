/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_DEMO_H
#define __STM32F4_DISCOVERY_DEMO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
// #include "usbd_cdc_core.h"
// #include "usbd_usr.h"
// #include "usbd_desc.h"
// #include "usbd_cdc_vcp.h"

//Library config for this project!!!!!!!!!!!
#include "stm32f4xx_conf.h"

// does this work?
// #include <stdio.h>
// yes, but it's a bloated hog. Use custon printf instead.

// adafruit TFTLCD test suite
void testFillRoundRect();
void testRoundRect();
void testtriangles();
void testfilltriangles();
void testtext(uint16_t color);
void testfillcircles(uint8_t radius, uint16_t color);
void testdrawcircles(uint8_t radius, uint16_t color);
void testfillrects(uint16_t color1, uint16_t color2);
void testdrawrects(uint16_t color);
void testfastlines(uint16_t color1, uint16_t color2);
void testlines(uint16_t color);
void testBars(void);





/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* TIM2 Autoreload and Capture Compare register values */
#define TIM_ARR                          (uint16_t)1999
#define TIM_CCR                          (uint16_t)1000




/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void Fail_Handler(void);

#endif /* __STM32F4_DISCOVERY_DEMO_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
