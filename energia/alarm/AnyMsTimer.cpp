#include "AnyMsTimer.h"

unsigned long AnyMsTimer::msecs[2];
void (*AnyMsTimer::func[2])();
volatile unsigned long AnyMsTimer::count[2];
volatile char AnyMsTimer::overflowing[2];
volatile boolean AnyMsTimer::running[2];

/*
 MSP430: Wait on all register changes until they call start.
 f is the location of the function
*/
void AnyMsTimer::set(unsigned long ms, void (*f)(), byte index) {
  if (ms == 0)
    msecs[index] = 1;
  else
    msecs[index] = ms;		
  func[index] = f;
  running[index] = false;
}

// lifted from wiring_analog.c so that the do not step on each other
// there should probably be a processor check here but that also exists in MsTimer.h
uint16_t analog_period2 = F_CPU/490, analog_div2 = 0, analog_res2=255; // devide clock with 0, 2, 4, 8
#define PWM_PERIOD analog_period2 // F_CPU/490
#define PWM_DUTY(x) ( (unsigned long)x*PWM_PERIOD / (unsigned long)analog_res2 )

// configure the   registers the same way as they are for analogWrite()
void AnyMsTimer::start(byte index) {
  count[index] = 0;
  overflowing[index] = 0;
  if (index == 1) {
    // identical to the wiring_analog.c pwm setup so is compatible
    TA1CCR0 = PWM_PERIOD;                  // PWM Period
    TA1CTL = TASSEL_2 + MC_1+ analog_div2; // SMCLK, up mode
    TA1CCTL0 |= CCIE;  // CCR0 interrupt enabled
  } else
  if (index == 0) {
    // identical to the wiring_analog.c pwm setup so is compatible
    TA0CCR0 = PWM_PERIOD;                  // PWM Period
    TA0CTL = TASSEL_2 + MC_1+ analog_div2; // SMCLK, up mode
    TA0CCTL0 |= CCIE;  // CCR0 interrupt enabled
  }
  running[index]=true;
}

// turn off the interrupts but don't turn of the timer because may
// be in use for pwm.
void AnyMsTimer::stop(byte index) {
  // disable interrupt
  if (index == 1)
    TA1CCTL0 &= ~CCIE;
  else if (index == 0)
    TA0CCTL0 &= ~CCIE;
  running[index]=false;
}

// called by the ISR every time we get an interrupt
// operates indepndently from timer value
void AnyMsTimer::_ticHandler2MS(byte index)
{
  if (running[index] == true){
    // It is a 500hz interrupt so each interrupt is 2msec
    count[index] += 2;
    if (count[index] >= msecs[index] && !overflowing[index]) {
      overflowing[index] = 1;
      // subtract msecs to catch missed overflows. set to 0 if you don't want this.
      // do this before running supplied function so don't take into account handler time
      count[index] = count[index] - msecs[index]; 
      // call the program supplied function
      (*func[index])();
      overflowing[index] = 0;
    }
  }
}

// Timer interrupt service routine
__attribute__((interrupt(TIMER1_A0_VECTOR)))
void Timer_A1_int(void)
{
  AnyMsTimer::_ticHandler2MS(1);
}

// Timer interrupt service routine
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void Timer_A0_int(void)
{
  AnyMsTimer::_ticHandler2MS(0);
}
