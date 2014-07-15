#include <msp430g2553.h>

#define A1 BIT1
#define A6 BIT6

void ADC_init(void) {
            // Используем Vcc/Vss(аналоговая земля) для верхнего/нижнего ИОН,
            // 16 x ADC10CLKs (выборка за 16 тактов), включаем АЦП.
    ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
            // Вход A1, делитель ADC10CLK на 1, одноканальный режим.  
    ADC10CTL1 = INCH_6 + SHS_0 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_0;
    ADC10AE0 = A6;      // Разрешаем вход АЦП на порту P1.1
    
    ADC10CTL0 |= ENC;     // Разрешаем преобразования.
} // ADC_init

char EVENT = 0;
double avg = 0;
int norm = 0;

void addValue(int newValue) {
  if (abs(avg) < 0.001) avg = newValue;
  avg = avg + 0.001 * (newValue - avg);  // slowly converge to average

  int v = newValue - avg;
  v = (v>0)?v:-v;
  norm = norm + 0.1 * (v - norm);

  if (norm >= 8)
    EVENT = 1;
  else
    EVENT = 0;
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = 0xff;
  P1OUT = 0;
  ADC_init();
  unsigned int cntOverall = 0;
  unsigned int cntBlink = 5;
  unsigned int cntEvents = 0;
  char flagEventAccounted = 0;


  __delay_cycles(1000);
  while (1)
  {
    __delay_cycles(20000);
    ADC10CTL0 |= ENC + ADC10SC;
    while (ADC10CTL1 & BUSY);
    int val = ADC10MEM;
    ADC10CTL0 &=~ ENC;

    addValue(val);
    if (EVENT && !flagEventAccounted) {
      P1OUT |= BIT0;
      cntBlink = 50;
      cntEvents ++;  // event counter
      cntOverall = 100;
      flagEventAccounted = 1;
    }

    if (cntBlink == 0) {
      P1OUT &= ~(BIT0 | BIT7);
      flagEventAccounted = 0;
    } else {
      cntBlink--;
    }

    if (cntOverall == 0) {
      cntEvents = 0;
    } else {
      cntOverall--;
    }

    if (cntEvents == 4) {
      P1OUT |= BIT7;
      cntEvents = 0;
      cntBlink = 100;
      cntOverall = 100;
    }
  }
}

