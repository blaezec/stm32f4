/**
******************************************************************************
* @file    main.c 
* @author  MCD Application Team
* @version V1.0.0
* @date    19-September-2011
* @brief   Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "TFTLCD.h"
#include "stm32f4xx_it.h"
/** @addtogroup STM32F4-Discovery_Demo
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TESTRESULT_ADDRESS         0x080FFFFC
#define ALLTEST_PASS               0x00000000
#define ALLTEST_FAIL               0x55555555

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
    __ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

    __IO uint32_t TimingDelay;


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
* @brief  Main program.
* @param  None
* @retval None
*/
int main(void)
{
    uint8_t currentLED=0,idx;
    uint16_t xx,yy;
    uint16_t testWord; 
    uint8_t shifter=0;

  /* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/

    __IO uint32_t i = 0;  
    uint8_t buf[255];
    uint8_t len;
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);
    // STM_EVAL_LEDInit(LED5); //GPIOD 14 -> FSMC D0
    // STM_EVAL_LEDInit(LED6); //GPIOD 14 -> FSMC D1

    STM_EVAL_LEDToggle(currentLED);

    // flash the LEDs ina circle to test delayMillis(uint32_t timedelay)



    for (idx=0;idx<8;idx++)
    {
        STM_EVAL_LEDToggle(currentLED);
        currentLED=(currentLED+1)%2;
        STM_EVAL_LEDToggle(currentLED);
        delayMillis(250);
    }
    // 
    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);

    // init the printf
    init_printf(NULL,VCP_put_char_printf);
    init_tft_printf(NULL,tft_putc);
    VCP_send_str("\r\n");
    // printf("hello printf! %d\n\r",23123);
    // printf("hex: 0x%08X\n\r",0x0f0a0def);
    // printf("signed %d\n\r",-23123);
    initGPIO();
    // uDelay(1000);
    initFSMC();
    uDelay(1000);
    reset();
    initDisplay();
    // uDelay(1000);
    // 
    // drawPixel(10, 10,BLACK);
    // uDelay(1000);

    // drawPixel(10, 11,BLACK);
    // uDelay(1000);
    // drawPixel(10, 12,BLACK);

    // fillScreen(BLACK);
    // uDelay(1000);
    // testfillcircles(10, MAGENTA);

    fillScreen(BLACK);
    setCursor(0, 0);
    setTextColor(CYAN);
    setTextSize(1);
    setRotation(1); 
    tft_printf("Please connect to virtual COM port...");
    delayMillis(2000);
    testlines(CYAN);
    delayMillis(2500);
    testfastlines(RED, BLUE);
    delayMillis(2500);
    testdrawrects(GREEN);
    delayMillis(2500);
    testfillrects(YELLOW, MAGENTA);
    delayMillis(2500);
    fillScreen(BLACK);
    testfillcircles(10, MAGENTA);
    testdrawcircles(10, WHITE);
    delayMillis(2500); 
    testtriangles();
    delayMillis(2500); 
    testfilltriangles();
    delayMillis(2500); 
    testRoundRect();
    delayMillis(2500); 
    testFillRoundRect();
    delayMillis(2500); 
    fillScreen(GREEN);
    delayMillis(2500); 
    for (idx=0;idx<8;idx++)
    {
        setRotation(idx%4); 
        testtext(RED);
        delayMillis(2500); 
    }
    
    // fsmcData        =  0x60020000; // sets a16
    // fsmcRegister    =  0x60000000; // clears a16
    // printf("fsmcData:\t0x%08X\r\n",fsmcData);
    // printf("fsmcRegister:\t0x%08X\r\n",fsmcRegister);

    // fillScreen(BLACK);
    // setCursor(0, 20);
    // setTextColor(color);
    // setTextSize(1);
    // write("Hello World!");
    // setTextSize(2);
    // write(1234.56);
    // setTextSize(3);
    // 
    // println(0xDEADBEEF, HEX);


    // printf("0xFF00>>8 0x%04X\r\n",0xff00>>8);
    // VCP_send_str(&buf[0]);
    // printf("SysTick_Config(SystemCoreClock/1000)\t %d\r\n",SysTick_Config(SystemCoreClock/1000));
    // millisecondCounter=0;
    // for (idx=0;idx<100;idx++)
    // {
    //     printf("millisecondCounter:\t%d",millisecondCounter);
    // }
    // void delayMillis(uint32_t millis) 
    //     {
    //         uint32_t target;
    // 
    //         target=millisecondCounter+millis;
    //         while(millisecondCounter<target);
    //     }

   
    // From stm32f4_discovery.h:
    // typedef enum 
    // {
    // LED4 = 0,
    // LED3 = 1,
    // LED5 = 2,
    // LED6 = 3
    // } Led_TypeDef;
    while(1)
    {
        if (VCP_get_char(buf))
        {
            // show a different LED every time we read over the USB COM.
            STM_EVAL_LEDToggle(currentLED);
            currentLED=(currentLED+1)%2;
            STM_EVAL_LEDToggle(currentLED);
            VCP_put_char(buf[0]);

            if(buf[0]=='\r')  VCP_put_char('\n');

            buf[0]=0;

        } 

        

    }


}




/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
// void TimingDelay_Decrement(void)
// {
//     if (TimingDelay != 0x00)
//     { 
//         TimingDelay--;
//     }
// }

/**
* @brief  This function handles the test program fail.
* @param  None
* @retval None
*/
void Fail_Handler(void)
{
  // /* Erase last sector */ 
  //   FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);
  // /* Write FAIL code at last word in the flash memory */
  //   FLASH_ProgramWord(TESTRESULT_ADDRESS, ALLTEST_FAIL);

    while(1)
    {
    /* Toggle Red LED */
        STM_EVAL_LEDToggle(LED5);
        delayMillis(50);
    }
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
      while (1)
  {
  }
}
#endif

/*
* graphicstest from adafruit
*/
    void testFillRoundRect()
{
    fillScreen(BLACK);
    uint16_t x;
    for (x=width(); x > 20 ; x-=6)
    {
        fillRoundRect(width()/2 -x/2, height()/2 -x/2 , x, x, x/8,  Color565(0, x, 0));
    }
}

void testRoundRect()
{
    fillScreen(BLACK);
    uint16_t x;
    for (x=0; x < width(); x+=6)
    {
        drawRoundRect(width()/2 -x/2, height()/2 -x/2 , x, x, x/8, Color565(x, 0, 0));
    }
}

void testtriangles()
{
    fillScreen(BLACK);
    uint16_t i;
    for (i=0; i<width()/2; i+=5)
    {
        drawTriangle(width()/2, height()/2-i,
            width()/2-i, height()/2+i,
            width()/2+i, height()/2+i, Color565(0, 0, i));
    }
}

void testfilltriangles()
{
    fillScreen(BLACK);
    uint16_t i;
    for (i=width()/2; i>10; i-=5)
    {
        fillTriangle(width()/2, height()/2-i,
            width()/2-i, height()/2+i,
            width()/2+i, height()/2+i, 
            Color565(0, i, i));
        drawTriangle(width()/2, height()/2-i,
            width()/2-i, height()/2+i,
            width()/2+i, height()/2+i, Color565(i, i, 0));    
    }
}
void testtext(uint16_t color)
{
    fillScreen(BLACK);
    setCursor(0, 20);
    setTextColor(color);
    setTextSize(1);
    // write('A');
    tft_printf("Hello World!\n");
    setTextSize(2);
    tft_printf("1234.56\n");
    setTextSize(3);
    tft_printf("0x%X\n",0xDEADBEEF);
}


// }
void testfillcircles(uint8_t radius, uint16_t color) 
{
    uint16_t x,y;
    for (x=radius; x < width(); x+=radius*2) 
    {
        for (y=radius; y < height(); y+=radius*2)
        {
            fillCircle(x, y, radius, color);
        }
    }  
}
void testdrawcircles(uint8_t radius, uint16_t color)
{
    uint16_t x,y;
    for ( x=0; x < width()+radius; x+=radius*2)
    {
        for ( y=0; y < height()+radius; y+=radius*2)
        {
            drawCircle(x, y, radius, color);
        }
    }  
}


void testfillrects(uint16_t color1, uint16_t color2)
{
    uint16_t x;
    fillScreen(BLACK);
    for ( x=width()-1; x > 6; x-=6)
    {
   //Serial.println(x, DEC);
        fillRect(width()/2 -x/2, height()/2 -x/2 , x, x, color1);
        drawRect(width()/2 -x/2, height()/2 -x/2 , x, x, color2);
    }
}

void testdrawrects(uint16_t color)
{
    uint16_t x;
    fillScreen(BLACK);
    for ( x=0; x < width(); x+=6)
    {
        drawRect(width()/2 -x/2, height()/2 -x/2 , x, x, color);
    }
}

void testfastlines(uint16_t color1, uint16_t color2)
{
    uint16_t x,y;
    fillScreen(BLACK);
    for ( y=0; y < height(); y+=5)
    {
        drawHorizontalLine(0, y, width(), color1);
    }
    for ( x=0; x < width(); x+=5)
    {
        drawVerticalLine(x, 0, height(), color2);
    }

}

void testlines(uint16_t color)
{
    uint16_t x,y;
    fillScreen(BLACK);
    for ( x=0; x < width(); x+=6)
    {
        drawLine(0, 0, x, height()-1, color);
    }
    // delayMillis(1500);
    for ( y=0; y < height(); y+=6)
    {
        drawLine(0, 0, width()-1, y, color);
        // delayMillis(500);
    }
    delayMillis(1500);
    fillScreen(BLACK);
    for ( x=0; x < width(); x+=6)
    {
        drawLine(width()-1, 0, x, height()-1, color);
        // delayMillis(500);
    }
    for ( y=0; y < height(); y+=6)
    {
        drawLine(width()-1, 0, 0, y, color);
        // delayMillis(500);
    }
    delayMillis(1500);
    fillScreen(BLACK);
    for ( x=0; x < width(); x+=6)
    {
        drawLine(0, height()-1, x, 0, color);
    }
    for ( y=0; y < height(); y+=6)
    {
        drawLine(0, height()-1, width()-1, y, color);
    }
    delayMillis(1500);
    fillScreen(BLACK);
    for ( x=0; x < width(); x+=6)
    {
        drawLine(width()-1, height()-1, x, 0, color);
    }
    for ( y=0; y < height(); y+=6)
    {
        drawLine(width()-1, height()-1, 0, y, color);
    }
    // printf("Done testlines\r\n");

}


void testBars()
{
    uint16_t i,j;
    for(i=0; i < height(); i++)

    {
        for(j=0; j < width(); j++)

        {
            if(i>279) writeData(WHITE);
            else if(i>239) writeData(BLUE);
            else if(i>199) writeData(GREEN);
            else if(i>159) writeData(CYAN);
            else if(i>119) writeData(RED);
            else if(i>79) writeData(MAGENTA);
            else if(i>39) writeData(YELLOW);
            else writeData(BLACK);
        }
    }
    return;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
