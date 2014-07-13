#include <msp430g2553.h>

#define A6 BIT6


//UART.h

/**
* Initialize soft UART
*/
void uart_init(void);

/**
* Set pointer for ISR to call when data was received.
*
* @param[in] *isr_ptr pointer to ISR
*/
void uart_set_rx_isr_ptr(void (*isr_ptr)(unsigned char c));

/**
* Read one character from UART blocking.
*
* @return character received
*/
unsigned char uart_getc();

/**
* Write one chracter to the UART blocking.
*
* @param[in] *c the character to write
*/
void uart_putc(unsigned char c);

/**
* Write string to the UART blocking.
*
* @param[in] *str the 0 terminated string to write
*/
void uart_puts(const char *str);


void ADC_init(void) {
            // Используем Vcc/Vss(аналоговая земля) для верхнего/нижнего ИОН,
            // 16 x ADC10CLKs (выборка за 16 тактов), включаем АЦП.
    ADC10CTL0 = SREF_1 + REFON + ADC10ON + ADC10SHT_3;
            // Вход A1, делитель ADC10CLK на 1, одноканальный режим.  
    ADC10CTL1 = INCH_10+ ADC10DIV_3;
} // ADC_init

int micro = 0;

int addValue(int newValue) {
  // micro = micro + 0.9 * (newValue - micro);
  micro = newValue;
}

//uart_PRINTF.h
void uart_printf(char *, ...);                 // uart_printf() output pointed to UART through putc()

//MY APP

#define MAX 0xff
#define MIN 0x00


void uart_rx_isr(unsigned char c) {
  // uart_putc(c);
  P1OUT ^= 0x40;  // toggle P1.0 (red led)
}

int tempOut()
{
    int t=0;
    __delay_cycles(1000);              //wait 4 ref to settle
    ADC10CTL0 |= ENC + ADC10SC;      //enable conversion and start conversion
    while(ADC10CTL1 & BUSY);         //wait..i am converting..pum..pum..
    t=ADC10MEM;                       //store val in t
    ADC10CTL0&=~ENC;                     //disable adc conv
    return(int) ((t * 27069L - 18169625L) >> 16); //convert and pass
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  
    BCSCTL1 = CALBC1_8MHZ; //Set DCO to 8Mhz
    DCOCTL = CALDCO_8MHZ; //Set DCO to 8Mhz
  
  P1DIR = 0xff;
  P1OUT = 0x1;
  ADC_init(); // !!!!!!
  uart_init();
  uart_set_rx_isr_ptr(uart_rx_isr);

  __bis_SR_register(GIE); // global interrupt enable

  unsigned char c;
  while ((c = uart_getc()) != '1');
  uart_puts((char *)"\nOK\n");

  volatile unsigned long i = 5000;
  int max = 0;

  ADC10CTL0 |= ADC10SC;
  while(1) {
    // __delay_cycles(500000); //wait and set break point
    // __delay_cycles(500000);  //wait and set breakpoint
    // __delay_cycles(500000); //wait and set break point
    // __delay_cycles(500000);  //wait and set breakpoint
    // __delay_cycles(500000); //wait and set break point
    // __delay_cycles(500000);  //wait and set breakpoint
    // __delay_cycles(500000); //wait and set break point
    // __delay_cycles(500000);  //wait and set breakpoint
    // __delay_cycles(500000); //wait and set break point
    // __delay_cycles(500000);  //wait and set breakpoint
    // __delay_cycles(500000); //wait and set break point
    // __delay_cycles(500000);  //wait and set breakpoint
    // uart_puts((char *)"\n***************\n");
    uart_printf("%i\n", tempOut());
    P1OUT ^= 0x1;
  } 
}





/*
* This file is part of the MSP430 hardware UART example.
*
* Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/******************************************************************************
* Hardware UART example for MSP430.
*
* Stefan Wendler
* sw@kaltpost.de
* http://gpio.kaltpost.de
******************************************************************************/

#include <msp430.h>
#include <legacymsp430.h>

/**
* Receive Data (RXD) at P1.1
*/
#define RXD BIT1

/**
* Transmit Data (TXD) at P1.2
*/
#define TXD BIT2

/**
* Callback handler for receive
*/
void (*uart_rx_isr_ptr)(unsigned char c);

void uart_init(void)
{
  uart_set_rx_isr_ptr(0L);

  P1SEL = RXD + TXD;
  P1SEL2 = RXD + TXD;
  //    UCA0CTL1 |= UCSSEL_2; // SMCLK
  //    UCA0BR0 = 104; // 1MHz 9600
  //    UCA0BR1 = 0; // 1MHz 9600
  //    UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
  //    UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine

  UCA0CTL1 |= UCSSEL_2; //SMCLK
  //8,000,000Hz, 9600Baud, UCBRx=52, UCBRSx=0, UCBRFx=1
  UCA0BR0 = 52; //8MHz, OSC16, 9600
  UCA0BR1 = 0; //((8MHz/9600)/16) = 52.08333
  UCA0MCTL = 0x10|UCOS16; //UCBRFx=1,UCBRSx=0, UCOS16=1
  UCA0CTL1 &= ~UCSWRST; //USCI state machine
  IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}

void uart_set_rx_isr_ptr(void (*isr_ptr)(unsigned char c))
{
  uart_rx_isr_ptr = isr_ptr;  
}

unsigned char uart_getc()
{
  while (!(IFG2&UCA0RXIFG)); // USCI_A0 RX buffer ready?
  return UCA0RXBUF;
}

void uart_putc(unsigned char c)
{
  while (!(IFG2&UCA0TXIFG)); // USCI_A0 TX buffer ready?
     UCA0TXBUF = c; // TX
}

void uart_puts(const char *str)
{
  while(*str) uart_putc(*str++);
}

interrupt(USCIAB0RX_VECTOR) USCI0RX_ISR(void)
{
  if(uart_rx_isr_ptr != 0L) {
   (uart_rx_isr_ptr)(UCA0RXBUF);
  }
}




/// uart_PRINTF

#include "msp430g2231.h"
#include "stdarg.h"

// void uart_putc(unsigned char);
// void uart_puts(char *);

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max     
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};

static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            uart_putc(c);
        } while(!(d & 1));
    } else
        uart_putc('0');
}

static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    uart_putc(hex[n & 15]);
}
 
void uart_printf(char *format, ...)
{
    char c;
    int i;
    long n;
    
    va_list a;
    va_start(a, format);
    while(c = *format++) {
        if(c == '%') {
            switch(c = *format++) {
                case 's':                       // String
                    uart_puts(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    uart_putc(va_arg(a, char));
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, uart_putc('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, uart_putc('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        } else
bad_fmt:    uart_putc(c);
    }
    va_end(a);
}