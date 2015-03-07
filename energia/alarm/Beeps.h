#ifndef Beeps_h
#define Beeps_h

#define BEEP_OUTPUT P1_6

namespace Beeps {
  void setup();

  void beep(int d);

  void short_beep();
  void short_beep_series(int count);

  void long_beep();
}

#endif
