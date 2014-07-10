#include <msp430g2553.h>

void delay_ms(unsigned int ms )
{
    unsigned int i;
    for (i = 0; i<= ms; i++)
       __delay_cycles(500); //Built-in function that suspends the execution for 500 cicles
}

void delay_us(unsigned int us )
{
    unsigned int i;
    for (i = 0; i<= us/2; i++) 
       __delay_cycles(1);
}

//This function generates the square wave that makes the piezo speaker sound at a determinated frequency.
void beep(unsigned int note, unsigned int duration)
{ 
    int i;  
    long delay = (long)(80000/note);  //This is the semiperiod of each note. 
    long time = (long)((duration*400)/(delay*2));  //This is how much time we need to spend on the note.
    for (i=0;i<time;i++)
    {    
        P1OUT |= BIT6;     //Set P1.2...
        delay_us(delay);   //...for a semiperiod...
        P1OUT &= ~BIT6;    //...then reset it...
        delay_us(delay);   //...for the other semiperiod.
    }
    //delay_ms(20); //Add a little delay to separate the single notes
}

#define mindelay 40
#define maxdelay 200
#define step 2
int max(int a, int b) {
    if (a < b) return b;
    return a;
}

void siren()
{
    long delay = maxdelay;
    int i;
    int count = 0;
    int dir = 1;
    while (1) {
        if (dir) {
            delay -= step;
            if (delay <= mindelay) {
                dir = !dir;
                // break;
            }
        } else {
            delay += step;
            if (delay >= maxdelay) {
                dir = !dir;
                break;
            }
        }

        for(i=0;i<=1;i++) {
            P1OUT |= BIT6;
            delay_us(delay);
            P1OUT &= ~BIT6;
            delay_us(delay);
        }
    }
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = 0xFF;
  P1OUT = 0x00;
 
  while (1)
  {
    int i;
    for(i=0;i<4;i++)
        siren();
    delay_ms(500);      //Add a 2 sec. delay to avoid replaying right after the end.
  }
}
