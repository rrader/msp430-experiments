#include <msp430g2553.h>

int delay(int t)
{
  int i, s;
 
  s = 0;
  for (i = 0; i < t; i++)
    s += i;
 
  return s;
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = 0xFF;
 
  while (1)
  {
    P1OUT = 0x40;
    delay(10000);
    P1OUT = 0x00;
    delay(10000);
  }
}
