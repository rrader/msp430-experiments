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
 
  int n = 0x10;
  while (1)
  {
    P1OUT = n;
    delay(10000);
    // P1OUT = 0x00;
    // delay(10000);
    n >>= 1;
    if (n == 0) n = 0x10;
  }
}
