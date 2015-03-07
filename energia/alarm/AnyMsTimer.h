#ifndef AnyMsTimer_h
#define AnyMsTimer_h

#include "Energia.h"


namespace AnyMsTimer {
  extern unsigned long msecs[2];
  extern void (*func[2])();
  extern volatile unsigned long count[2];
  extern volatile char overflowing[2];
  extern volatile boolean running[2];

  void set(unsigned long ms, void (*f)(), byte index);
  void start(byte index);
  void stop(byte index);
  void _ticHandler2MS(byte index);
}

#endif
