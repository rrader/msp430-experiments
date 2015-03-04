/*
NOTICE
Used code or got an idea from:
    UART: Stefan Wendler - http://gpio.kaltpost.de/?page_id=972
    ADC: http://indiantinker.wordpress.com/2012/12/13/tutorial-using-the-internal-temperature-sensor-on-a-msp430/
    printf: http://forum.43oh.com/topic/1289-tiny-printf-c-version/
*/

#include <msp430g2553.h>

// =========== HEADERS ===============
// UART
void uart_init(void);
void uart_set_rx_isr_ptr(void (*isr_ptr)(unsigned char c));
unsigned char uart_getc();
void uart_putc(unsigned char c);
void uart_puts(const char *str);
void uart_printf(char *, ...);
// ADC
void ADC_init(void);
// =========== /HEADERS ===============


// Trigger on received character
void uart_rx_isr(unsigned char c) {
  P1OUT ^= 0x40;
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1 = CALBC1_8MHZ; //Set DCO to 8Mhz
  DCOCTL = CALDCO_8MHZ; //Set DCO to 8Mhz
  
  P1DIR = 0xff;
  P1OUT = 0x0;
  ADC_init();
  uart_init();
  uart_set_rx_isr_ptr(uart_rx_isr);

  __bis_SR_register(GIE); // global interrupt enable

  // // UART Handshake...
  unsigned char c;
  // while ((c = uart_getc()) != '1');
  uart_puts((char *)"AT\n");
  P1OUT ^= 0x40;

  ADC10CTL0 |= ADC10SC;
  while(1) {
    c = uart_getc();
    // uart_printf("%i\n", getTemperatureCelsius());
    P1OUT ^= 0x80;
    P1OUT ^= 0x80;
  } 
}

// ========================================================
// ADC configured to read temperature
void ADC_init(void) {
    ADC10CTL0 = SREF_1 + REFON + ADC10ON + ADC10SHT_3;
    ADC10CTL1 = INCH_10 + ADC10DIV_3;
}

int getTemperatureCelsius()
{
    int t = 0;
    __delay_cycles(1000);
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & BUSY);
    t = ADC10MEM;
    ADC10CTL0 &=~ ENC;
    return(int) ((t * 27069L - 18169625L) >> 16);
}


// ========================================================
// UART
#include <legacymsp430.h>

#define RXD BIT1
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



// ========================================================
// UART PRINTF
#include "stdarg.h"

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
