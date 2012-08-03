/*
File: printf.c

Copyright (C) 2004  Kustaa Nyholm

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "printf.h"

typedef void (*putcf) (void*,uint8_t);
static putcf stdout_putf;
typedef void (*putctft) (void*,uint8_t);
static putctft stdout_puttft;
static void* stdout_putp;


#ifdef PRINTF_LONG_SUPPORT

static void uli2a(unsigned long int num, unsigned int base, int uc,uint8_t * bf)
{
    int n=0;
    unsigned int d=1;
    while (num/d >= base)
        d*=base;         
    while (d!=0) {
        int dgt = num / d;
        num%=d;
        d/=base;
        if (n || dgt>0|| d==0) {
            *bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
            ++n;
        }
    }
    *bf=0;
}

static void li2a (long num, uint8_t * bf)
{
    if (num<0) {
        num=-num;
        *bf++ = '-';
    }
    uli2a(num,10,0,bf);
}

#endif

static void ui2a(unsigned int num, unsigned int base, int uc,uint8_t * bf)
{
    int n=0;
    unsigned int d=1;
    while (num/d >= base)
        d*=base;        
    while (d!=0) {
        int dgt = num / d;
        num%= d;
        d/=base;
        if (n || dgt>0 || d==0) {
            *bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
            ++n;
        }
    }
    *bf=0;
}

static void i2a (int num, uint8_t * bf)
{
    if (num<0) {
        num=-num;
        *bf++ = '-';
    }
    ui2a(num,10,0,bf);
}

static int a2d(uint8_t ch)
{
    if (ch>='0' && ch<='9') 
        return ch-'0';
    else if (ch>='a' && ch<='f')
        return ch-'a'+10;
    else if (ch>='A' && ch<='F')
        return ch-'A'+10;
    else return -1;
}

static uint8_t a2i(uint8_t ch, uint8_t** src,int base,int* nump)
{
    uint8_t* p= *src;
    int num=0;
    int digit;
    while ((digit=a2d(ch))>=0) {
        if (digit>base) break;
        num=num*base+digit;
        ch=*p++;
    }
    *src=p;
    *nump=num;
    return ch;
}

static void putchw(void* putp,putcf putf,int n, uint8_t z, uint8_t* bf)
{
    uint8_t fc=z? '0' : ' ';
    uint8_t ch;
    uint8_t* p=bf;
    while (*p++ && n > 0)
        n--;
    while (n-- > 0) 
        putf(putp,fc);
    while ((ch= *bf++))
        putf(putp,ch);
}

void tfp_format(void* putp,putcf putf,uint8_t *fmt, va_list va)
{
    uint8_t bf[12];

    uint8_t ch;


    while ((ch=*(fmt++))) {
        if (ch!='%') 
            putf(putp,ch);
        else {
            uint8_t lz=0;
#ifdef  PRINTF_LONG_SUPPORT
            uint8_t lng=0;
#endif
            int w=0;
            ch=*(fmt++);
            if (ch=='0') {
                ch=*(fmt++);
                lz=1;
            }
            if (ch>='0' && ch<='9') {
                ch=a2i(ch,&fmt,10,&w);
            }
#ifdef  PRINTF_LONG_SUPPORT
            if (ch=='l') {
                ch=*(fmt++);
                lng=1;
            }
#endif
            switch (ch) {
                case 0: 
                goto abort;
                case 'u' : {
#ifdef  PRINTF_LONG_SUPPORT
                    if (lng)
                        uli2a(va_arg(va, unsigned long int),10,0,bf);
                    else
#endif
                        ui2a(va_arg(va, unsigned int),10,0,bf);
                    putchw(putp,putf,w,lz,bf);
                    break;
                }
                case 'd' :  {
#ifdef  PRINTF_LONG_SUPPORT
                    if (lng)
                        li2a(va_arg(va, unsigned long int),bf);
                    else
#endif
                        i2a(va_arg(va, int),bf);
                    putchw(putp,putf,w,lz,bf);
                    break;
                }
                case 'x': case 'X' : 
#ifdef  PRINTF_LONG_SUPPORT
                if (lng)
                    uli2a(va_arg(va, unsigned long int),16,(ch=='X'),bf);
                else
#endif
                    ui2a(va_arg(va, unsigned int),16,(ch=='X'),bf);
                putchw(putp,putf,w,lz,bf);
                break;
                case 'c' : 
                putf(putp,(uint8_t)(va_arg(va, int)));
                break;
                case 's' : 
                putchw(putp,putf,w,0,va_arg(va, uint8_t*));
                break;
                case '%' :
                putf(putp,ch);
                default:
                break;
            }
        }
    }
    abort:;
}


void init_printf(void* putp,void (*putf) (void*,uint8_t))
{
    stdout_putf=putf;
    stdout_putp=putp;
}

void tfp_printf(uint8_t *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(stdout_putp,stdout_putf,fmt,va);
    va_end(va);
}

void tft_printf(uint8_t *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(stdout_putp,stdout_puttft,fmt,va);
    va_end(va);
}

static void putcp(void* p,uint8_t c)
{
    *(*((uint8_t**)p))++ = c;
}

void init_tft_printf(void* putp,void (*putf) (void*,uint8_t))
{
    stdout_puttft=putf;

}




// typedef void (*putcf) (void*,uint8_t);
// static putcf stdout_putf;

// typedef void (*putctft) (void*,uint8_t);
// static putctft stdout_puttft;


void tfp_sprintf(uint8_t* s,uint8_t *fmt, ...)
{
    va_list va;
    va_start(va,fmt);
    tfp_format(&s,putcp,fmt,va);
    putcp(&s,0);
    va_end(va);
}


