// TFTLCD.c
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "TFTLCD.h"

static const uint16_t _regValues[] = {
        TFTLCD_START_OSC, 0x0001,     // start oscillator

        TFTLCD_DELAYCMD, 50,          // this will make a delay of 50 milliseconds

        TFTLCD_DRIV_OUT_CTRL, 0x0100, 
        TFTLCD_DRIV_WAV_CTRL, 0x0700,
        TFTLCD_ENTRY_MOD, 0x1030,
        TFTLCD_RESIZE_CTRL, 0x0000,
        TFTLCD_DISP_CTRL2, 0x0202,
        TFTLCD_DISP_CTRL3, 0x0000,
        TFTLCD_DISP_CTRL4, 0x0000,
        TFTLCD_RGB_DISP_IF_CTRL1, 0x0,
        TFTLCD_FRM_MARKER_POS, 0x0,
        TFTLCD_RGB_DISP_IF_CTRL2, 0x0,

        TFTLCD_POW_CTRL1, 0x0000,
        TFTLCD_POW_CTRL2, 0x0007,
        TFTLCD_POW_CTRL3, 0x0000,
        TFTLCD_POW_CTRL4, 0x0000,

        TFTLCD_DELAYCMD, 200,  

        TFTLCD_POW_CTRL1, 0x1690,
        TFTLCD_POW_CTRL2, 0x0227,

        TFTLCD_DELAYCMD, 50,

        TFTLCD_POW_CTRL3, 0x001A,

        TFTLCD_DELAYCMD, 50,

        TFTLCD_POW_CTRL4, 0x1800,
        TFTLCD_POW_CTRL7, 0x002A,

        TFTLCD_DELAYCMD,50,

        TFTLCD_GAMMA_CTRL1, 0x0000,
        TFTLCD_GAMMA_CTRL2, 0x0000,
        TFTLCD_GAMMA_CTRL3, 0x0000,
        TFTLCD_GAMMA_CTRL4, 0x0206,
        TFTLCD_GAMMA_CTRL5, 0x0808,
        TFTLCD_GAMMA_CTRL6, 0x0007,
        TFTLCD_GAMMA_CTRL7, 0x0201,
        TFTLCD_GAMMA_CTRL8, 0x0000,
        TFTLCD_GAMMA_CTRL9, 0x0000,
        TFTLCD_GAMMA_CTRL10, 0x0000,

        TFTLCD_GRAM_HOR_AD, 0x0000,
        TFTLCD_GRAM_VER_AD, 0x0000,
        TFTLCD_HOR_START_AD, 0x0000,
        TFTLCD_HOR_END_AD, 0x00EF,
        TFTLCD_VER_START_AD, 0X0000,
        TFTLCD_VER_END_AD, 0x013F,


        TFTLCD_GATE_SCAN_CTRL1, 0xA700,     // Driver Output Control (R60h)
        TFTLCD_GATE_SCAN_CTRL2, 0x0003,     // Driver Output Control (R61h)
        TFTLCD_GATE_SCAN_CTRL3, 0x0000,     // Driver Output Control (R62h)

        TFTLCD_PANEL_IF_CTRL1, 0X0010,      // Panel Interface Control 1 (R90h)
        TFTLCD_PANEL_IF_CTRL2, 0X0000,
        TFTLCD_PANEL_IF_CTRL3, 0X0003,
        TFTLCD_PANEL_IF_CTRL4, 0X1100,
        TFTLCD_PANEL_IF_CTRL5, 0X0000,
        TFTLCD_PANEL_IF_CTRL6, 0X0000,

  // Display On
        TFTLCD_DISP_CTRL1, 0x0133,     // Display Control (R07h)
    };

/*
* Initialise GPIO ports D and E for FSMC use
* Also initialise PE1 for RESET
* RS will be on A16
*
* Don't know if the ili9235 can take the 100MHz pin clock - is it just the 
* bandwidth for the risetime of the pulse?
*/

/*
                   NOR/SRAM Controller functions
===============================================================================  

The following sequence should be followed to configure the FSMC to interface with
SRAM, PSRAM, NOR or OneNAND memory connected to the NOR/SRAM Bank:

  1. Enable the clock for the FSMC and associated GPIOs using the following functions:
         RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

  2. FSMC pins configuration 
      - Connect the involved FSMC pins to AF12 using the following function 
         GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_FSMC); 
      - Configure these FSMC pins in alternate function mode by calling the function
         GPIO_Init();    
      
  3. Declare a FSMC_NORSRAMInitTypeDef structure, for example:
         FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
     and fill the FSMC_NORSRAMInitStructure variable with the allowed values of
     the structure member.
     
  4. Initialize the NOR/SRAM Controller by calling the function
         FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  5. Then enable the NOR/SRAM Bank, for example:
         FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);  

  6. At this stage you can read/write from/to the memory connected to the NOR/SRAM Bank. 

*/

void initGPIO(void) 
{

    GPIO_InitTypeDef init={0};

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,ENABLE);
    

// reset

    init.GPIO_Pin=GPIO_Pin_1;                               // reset pin
    init.GPIO_Mode=GPIO_Mode_OUT;
    init.GPIO_Speed=GPIO_Speed_100MHz;
    // init.GPIO_Speed=GPIO_Speed_50MHz;
    // init.GPIO_Speed=GPIO_Speed_2MHz;

    GPIO_Init(GPIOE,&init);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC); // D2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC); // D3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC); // NOE -> RD
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC); // NWE -> WR
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC); // NE1 -> CS
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC); // D13
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC); // D14
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC); // D15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC); // A16 -> RS (C/D in ladyada terminology)
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC); // D0
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC); // D1

// PORTE

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC); // D4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC); // D5
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC); // D6
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC); // D7
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC); // D8
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC); // D9
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC); // D10
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC); // D11
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC); // D12

    init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
        GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
        GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
    init.GPIO_Mode = GPIO_Mode_AF;
    init.GPIO_Speed = GPIO_Speed_100MHz;
    // init.GPIO_Speed=GPIO_Speed_50MHz;
    // init.GPIO_Speed=GPIO_Speed_2MHz;
    init.GPIO_OType = GPIO_OType_PP;
    init.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOD, &init);

// PORTE
    init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
        GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
        GPIO_Pin_15;
    init.GPIO_Mode = GPIO_Mode_AF;
    init.GPIO_Speed = GPIO_Speed_100MHz;
    // init.GPIO_Speed=GPIO_Speed_50MHz;
    // init.GPIO_Speed=GPIO_Speed_2MHz;
    init.GPIO_OType = GPIO_OType_PP;
    init.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOE, &init);
}


/*
* Initialise NOR/SRAM bank 1
*/

void initFSMC() 
{
    // FSMC and TFTLCD
    fsmcData        =(uint16_t*)  0x60020000; // sets a16
    fsmcRegister    =(uint16_t*)  0x60000000; // clears a16


    
    
    FSMC_NORSRAMTimingInitTypeDef timing;//={0};
    FSMC_NORSRAMInitTypeDef init;//={0};

    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);

    timing.FSMC_AddressSetupTime=2; // want 2 : 
    timing.FSMC_DataSetupTime=5;  // keep
    timing.FSMC_AccessMode=FSMC_AccessMode_B/*FSMC_AccessMode_A*/; // B? This is how he does it in ili9325
    timing.FSMC_CLKDivision=1; //  ?
    timing.FSMC_DataLatency=0;
    timing.FSMC_BusTurnAroundDuration=0;
    timing.FSMC_AddressHoldTime=0;
// itiming.nitialise how the FSMC will work and then enable it

    init.FSMC_Bank=FSMC_Bank1_NORSRAM1; // keep
    init.FSMC_DataAddressMux=FSMC_DataAddressMux_Disable; // keep
    init.FSMC_MemoryType=FSMC_MemoryType_SRAM; // keep
    init.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b; // make 8 bit?
    init.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable; // keep
    init.FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;  // keep 
    init.FSMC_WrapMode=FSMC_WrapMode_Disable;  // keep 
    init.FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState; // keep
    init.FSMC_WriteOperation=FSMC_WriteOperation_Enable; // keep
    init.FSMC_WaitSignal=FSMC_WaitSignal_Disable; // keep
    init.FSMC_ExtendedMode=FSMC_ExtendedMode_Disable; // keep
    init.FSMC_WriteBurst=FSMC_WriteBurst_Disable; // keep 
    init.FSMC_ReadWriteTimingStruct=&timing;
    init.FSMC_WriteTimingStruct=&timing;
    init.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; // keep

    FSMC_NORSRAMInit(&init);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1,ENABLE);
}

/*
* start counting millis
*/

/**
* @brief  uDelay
*         This function provides delay time in micro sec
* @param  usec : Value of delay required in micro sec
* @retval None
*/
void uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  if (usec==0) return;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}

/*
* Delay for the given milliseconds
*/

void delayMillis(uint32_t millis) 
{
    uDelay(millis * 1000);  
}

/*
* SysTick interrupt handler
*/
// if this chokes, then move it to stm32fxxx_it.c?

// put this back in stm32fxxx_it.c, get stm32fxxx_it.h, and include it and TFTLCD.h in stm32fxxx_it.c.(see the Systick example)

//     void SysTick_Handler(void) 
// {
//     millisecondCounter++;
// }



void initDisplay(void) 
{
    uint16_t a, d;
    uint8_t i;
    rotation = 0;
    _width = TFTWIDTH;
    _height = TFTHEIGHT;


    cursor_y = cursor_x = 0;
    textsize = 1;
    textcolor = 0xFFFF;
    GPIO_SetBits(GPIOE, GPIO_Pin_1);
    delayMillis(2); 
    reset();
  
  
  
  uint16_t identifier = readRegister(0x0);
  printf ("TFTLCD Driver ID: 0x%04X\r\n",identifier);
  if (identifier == 0x9325) 
  {
    printf("Found ILI9325");
  } else if (identifier == 0x9328) {
    printf("Found ILI9328\r\n");
  } else {
    printf("Unknown driver chip\r\n");
    
    return;
  }
    for (i = 0; i < sizeof(_regValues) / 4; i++) {
        a = *(_regValues + i*2);
        d = *(_regValues + i*2 + 1);

        if (a == 0xFF) {
            delayMillis(d);
        } else {
            writeRegister(a, d);
      //printf("addr: %d data: 0x%04X\r\n",a,d); 
      
        }
    }
}


void reset(void) 
{
    GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    // digitalWrite(_reset, LOW);
    delayMillis(2); 
    
    // digitalWrite(_reset, HIGH);
    GPIO_SetBits(GPIOE, GPIO_Pin_1);
    printf("Reset TFT - \n\r");

  // resync
    writeData(0);
    writeData(0);
    writeData(0);  
    writeData(0);
}

void setRotation(uint8_t x)
{
    writeRegister(TFTLCD_ENTRY_MOD, 0x1030);

    x %= 4;  // cant be higher than 3
    rotation = x;
    switch (x) {
        case 0:
        _width = TFTWIDTH; 
        _height = TFTHEIGHT;
        break;
        case 1:
        _width = TFTHEIGHT; 
        _height = TFTWIDTH;
        break;
        case 2:
        _width = TFTWIDTH; 
        _height = TFTHEIGHT;
        break;
        case 3:
        _width = TFTHEIGHT; 
        _height = TFTWIDTH;
        break;
    }
}

uint8_t getRotation(void) 
{
    return rotation;
}
void goHome(void) 
{
    goTo(0,0);
}

void goTo(int x, int y) 
{
    writeRegister(0x0020, x);     // GRAM Address Set (Horizontal Address) (R20h)
    writeRegister(0x0021, y);     // GRAM Address Set (Vertical Address) (R21h)
    writeCommand(0x0022);            // Write Data to GRAM (R22h)
}

void setCursor(uint16_t x, uint16_t y)
{
    cursor_x = x;
    cursor_y = y;
}

void setTextSize(uint8_t s)
{
    textsize = s;
}

void setTextColor(uint16_t c)
{
    textcolor = c;
}
uint16_t width(void)
{
    return _width;
}
uint16_t height(void)
{
    return _height;
}

void write(uint8_t c) 
{
    if (c == '\n') 
    {
        cursor_y += textsize*8;
        cursor_x = 0;
    } 
    else if (c == '\r')
    {
    // skip em
    }
    else
    {
        drawChar(cursor_x, cursor_y, c, textcolor, textsize);
        cursor_x += textsize*6;
    }
}

void drawString(uint16_t x, uint16_t y, char *c, uint16_t color, uint8_t size)
{
    while (c[0] != 0)
    {
        drawChar(x, y, c[0], color, size);
        x += size*6;
        c++;
    }
}
void tft_putc(void *p, uint8_t c)
{
    write (c);
}
// void tftprintf(char *format)
// {
//     uint8_t *line;
//     sprintf(/*(uint8_t*)*/line,format);
//     uint16_t idx=0;
//     while (line[idx] != 0)
//     {
//         write(line[idx]);
//         idx++;
//     }
//     write('\n');
// }
// draw a character
void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size)
{
    uint8_t i,j;
    
    for ( i =0; i<5; i++ ) 
    {
        uint8_t line = *(font+(c*5)+i);
        for (j = 0; j<8; j++) 
        {
            if (line & 0x1) 
            {
                if (size == 1) // default size
                    drawPixel(x+i, y+j, color);
                else 
                {  // big size
                    fillRect(x+i*size, y+j*size, size, size, color);
                } 
            }
            line >>= 1;
        }
    }
}

// draw a triangle!
void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color); 
}

void fillTriangle ( int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t color)
{
    if (y0 > y1)
    {
        swap(y0, y1); swap(x0, x1);
    }
    if (y1 > y2)
    {
        swap(y2, y1); swap(x2, x1);
    }
    if (y0 > y1)
    {
        swap(y0, y1); swap(x0, x1);
    }

    int32_t dx1, dx2, dx3; // Interpolation deltas
    int32_t sx1, sx2, sy; // Scanline co-ordinates

    sx2=(int32_t)x0 * (int32_t)1000; // Use fixed point math for x axis values
    sx1 = sx2;
    sy=y0;

  // Calculate interpolation deltas
    if (y1-y0 > 0) dx1=((x1-x0)*1000)/(y1-y0);
    else dx1=0;
    if (y2-y0 > 0) dx2=((x2-x0)*1000)/(y2-y0);
    else dx2=0;
    if (y2-y1 > 0) dx3=((x2-x1)*1000)/(y2-y1);
    else dx3=0;

  // Render scanlines (horizontal lines are the fastest rendering method)
    if (dx1 > dx2)
    {
        for(; sy<=y1; sy++, sx1+=dx2, sx2+=dx1)
        {
            drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
        }
        sx2 = x1*1000;
        sy = y1;
        for(; sy<=y2; sy++, sx1+=dx2, sx2+=dx3)
        {
            drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
        }
    }
    else
    {
        for(; sy<=y1; sy++, sx1+=dx1, sx2+=dx2)
        {
            drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
        }
        sx1 = x1*1000;
        sy = y1;
        for(; sy<=y2; sy++, sx1+=dx3, sx2+=dx2)
        {
            drawHorizontalLine(sx1/1000, sy, (sx2-sx1)/1000, color);
        }
    }
}


uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t c;
    c = r >> 3;
    c <<= 6;
    c |= g >> 2;
    c <<= 5;
    c |= b >> 3;

    return c;
}

// draw a rectangle
void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  // smarter version
    drawHorizontalLine(x, y, w, color);
    drawHorizontalLine(x, y+h-1, w, color);
    drawVerticalLine(x, y, h, color);
    drawVerticalLine(x+w-1, y, h, color);
}

// draw a rounded rectangle
void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
  // smarter version
    drawHorizontalLine(x+r, y, w-2*r, color);
    drawHorizontalLine(x+r, y+h-1, w-2*r, color);
    drawVerticalLine(x, y+r, h-2*r, color);
    drawVerticalLine(x+w-1, y+r, h-2*r, color);
  // draw four corners
    drawCircleHelper(x+r, y+r, r, 1, color);
    drawCircleHelper(x+w-r-1, y+r, r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r, y+h-r-1, r, 8, color);
}


// fill a rounded rectangle
void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
  // smarter version
    fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

// fill a circle
void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    writeRegister(TFTLCD_ENTRY_MOD, 0x1030);
    drawVerticalLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

// used to do circles and roundrects!
void fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint16_t color) 
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x<y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            drawVerticalLine(x0+x, y0-y, 2*y+1+delta, color);
            drawVerticalLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2)
        {
            drawVerticalLine(x0-x, y0-y, 2*y+1+delta, color);
            drawVerticalLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}


// draw a circle outline

void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) 
{
  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  drawCircleHelper(x0, y0, r, 0xF, color);
}

void drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color) 
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;


    while (x<y) 
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) 
        {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        } 
        if (cornername & 0x2) 
        {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) 
        {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) 
        {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

// fill a rectangle
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t fillcolor) 
{
  // smarter version
    while (h--)
        drawHorizontalLine(x, y++, w, fillcolor);
}

void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    if (x >= _width) return;

    drawFastLine(x,y,length,color,1);
}

void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    if (y >= _height) return;
    drawFastLine(x,y,length,color,0);
}


void drawFastLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color, uint8_t rotflag)
{
    uint16_t newentrymod;

    switch (rotation) 
    {
        case 0:
        if (rotflag)
            newentrymod = 0x1028;   // we want a 'vertical line'
        else 
            newentrymod = 0x1030;   // we want a 'horizontal line'
        break;
        case 1:
        swap(x, y);
    // first up fix the X
        x = TFTWIDTH - x - 1;
        if (rotflag)
            newentrymod = 0x1000;   // we want a 'vertical line'
        else 
            newentrymod = 0x1028;   // we want a 'horizontal line'
        break;
        case 2:
        x = TFTWIDTH - x - 1;
        y = TFTHEIGHT - y - 1;
        if (rotflag)
            newentrymod = 0x1008;   // we want a 'vertical line'
        else 
            newentrymod = 0x1020;   // we want a 'horizontal line'
        break;
        case 3:
        swap(x,y);
        y = TFTHEIGHT - y - 1;
        if (rotflag)
            newentrymod = 0x1030;   // we want a 'vertical line'
        else 
            newentrymod = 0x1008;   // we want a 'horizontal line'
        break;
    }

    writeRegister(TFTLCD_ENTRY_MOD, newentrymod);

    writeRegister(TFTLCD_GRAM_HOR_AD, x); // GRAM Address Set (Horizontal Address) (R20h)
    writeRegister(TFTLCD_GRAM_VER_AD, y); // GRAM Address Set (Vertical Address) (R21h)
    writeCommand(TFTLCD_RW_GRAM);  // Write Data to GRAM (R22h)

    while (length--) 
    {
        writeData(color); 
    }

  // set back to default

    writeRegister(TFTLCD_ENTRY_MOD, 0x1030);
}

// bresenham's algorithm - thx wikpedia
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
{
  // if you're in rotation 1 or 3, we need to swap the X and Y's

    int16_t steep = abs((int32_t) y1 - (int32_t)y0) > abs((int32_t)x1 - (int32_t)x0);
    if (steep) 
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if (x0 > x1) {
        swap(x0, x1);
        swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
  //dy = ABS(y1 - y0);
    dy = abs((int32_t)y1 -(int32_t)y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) 
    {
        ystep = 1;
    } 
    else 
    {
        ystep = -1;
    }

    for (; x0<=x1; x0++) 
    {
        if (steep) 
        {
            drawPixel(y0, x0, color);
        }
        else 
        {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) 
        {
            y0 += ystep;
            err += dx;
        }
    }
}


void fillScreen(uint16_t color) 
{
    goHome();
    uint32_t i;

    i = 320;
    i *= 240;


    while (i--) 
    {
        uDelay(0);
        writeData(color); 
    }


}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  // check rotation, move pixel around if necessary
  switch (rotation) {
  case 1:
    swap(x, y);
    x = TFTWIDTH - x - 1;
    break;
  case 2:
    x = TFTWIDTH - x - 1;
    y = TFTHEIGHT - y - 1;
    break;
  case 3:
    swap(x, y);
    y = TFTHEIGHT - y - 1;
    break;
  }
    
  if ((x >= TFTWIDTH) || (y >= TFTHEIGHT)) return;
  writeRegister(TFTLCD_GRAM_HOR_AD, x); // GRAM Address Set (Horizontal Address) (R20h)
  // uDelay(0);
  writeRegister(TFTLCD_GRAM_VER_AD, y); // GRAM Address Set (Vertical Address) (R21h)
  // uDelay(0);
  writeCommand(TFTLCD_RW_GRAM);  // Write Data to GRAM (R22h)
  // uDelay(0);
  writeData(color);
}
/********************************** low level readwrite interface */

// the RS (C/D) pin is high during write
void writeData(uint16_t data) 
{
    
    
    // *retvl=(uint8_t)data >> 8;
    // *(retvl+1)=(uint8_t)data;
    *fsmcData=(data >> 8);
    uDelay(0);
    *fsmcData=data;
    // return retvl;
}
// d[8:15] version
// void writeData(uint16_t data) 
// {
//     
//     
//     // *retvl=(uint8_t)data >> 8;
//     // *(retvl+1)=(uint8_t)data;
//     *fsmcData=data;
//     // uDelay(0);
//     *fsmcData=data<<8;
//     // return retvl;
// }

// the RS (C/D) pin is low during write
void writeCommand(uint16_t cmd) 
{
    *fsmcRegister=(cmd >> 8);
    // uDelay(0);
    *fsmcRegister=cmd;
}
// d[8:15] version
// void writeCommand(uint16_t cmd) 
// {
//     *fsmcRegister=cmd;
//     // uDelay(0);
//     *fsmcRegister=cmd<<8;
// }

uint16_t readData() 
{
    uint16_t d = 0;

    d = *fsmcData;
    d <<= 8;
    // uDelay(0);
    d |= *fsmcData;
    return d;
}
// d[8:15] version
// uint16_t readData() 
// {
//     uint16_t d = 0;
// 
//     d = *fsmcData;
//     // d <<= 8;
//     // uDelay(0);
//     d |= *fsmcData>>8;
//     return d;
// }
/************************************* medium level data reading/writing */

uint16_t readRegister(uint16_t addr) 
{
   writeCommand(addr);
   // uDelay(10);
   return readData();
}


void writeRegister(uint16_t addr, uint16_t data) 
{
    writeCommand(addr);
    uDelay(10);
    writeData(data);
}

// // need abs
uint16_t abs(int32_t _numIn)
{
    if (_numIn<0)
    {
      return( (uint16_t)(_numIn*=-1));
    }
    else
    {
        return ((uint16_t)_numIn);
    }
}
