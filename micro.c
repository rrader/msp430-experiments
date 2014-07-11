#include <msp430g2553.h>

#define A1 BIT1
#define A6 BIT6

int delay(int t)
{
  int i, s;
 
  s = 0;
  for (i = 0; i < t; i++)
    s += i;
 
  return s;
}

void ADC_init(void) {
            // Используем Vcc/Vss(аналоговая земля) для верхнего/нижнего ИОН,
            // 16 x ADC10CLKs (выборка за 16 тактов), включаем АЦП.
    ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
            // Вход A1, делитель ADC10CLK на 1, одноканальный режим.  
    ADC10CTL1 = INCH_6 + SHS_0 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_0;
    ADC10AE0 = A6;      // Разрешаем вход АЦП на порту P1.1
    
    ADC10CTL0 |= ENC;     // Разрешаем преобразования.
} // ADC_init

unsigned int pow(unsigned int a) {
  return a*a;
}

#define TICKS 500

unsigned int micro = 0;
unsigned int counter = TICKS;
unsigned int maxValue = 0;
unsigned int prev = 0;

int addValue(int newValue) {
  counter --;
  if (counter == 0) {
    micro = maxValue;
    prev = maxValue;
    maxValue = newValue;
    counter = TICKS;
  }
  if (maxValue < newValue)
    maxValue = newValue;
  // maxValue = 
}

#define MAX 0xff  // 0xff**2
#define MIN 0x00

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = 0xf9;
  ADC_init();
 
  int n;
  while (1)
  {
    ADC10CTL0 |= ADC10SC;
    while ((ADC10CTL1 & ADC10BUSY) == 0x01);
    n = 0x00;
    addValue(ADC10MEM);
    if (micro >= MIN)
      n |= 0x10;
    if (micro >= MIN + (MAX-MIN)/5)
      n |= 0x08;
    if (micro >= MIN + (MAX-MIN)/5*2)
      n |= 0x80;
    if (micro >= MIN + (MAX-MIN)/5*3)
      n |= 0x01;
    if (micro >= MIN + (MAX-MIN)/5*4)
      n |= 0x20;
    P1OUT = n;
  }
}

