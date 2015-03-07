#include <Energia.h>
#include "Beeps.h"

namespace Beeps {

  void setup() {
    pinMode(BEEP_OUTPUT, OUTPUT);
    digitalWrite(BEEP_OUTPUT, LOW);
  }

  void beep(int d) {
      digitalWrite(BEEP_OUTPUT, HIGH);
      sleep(d);
      digitalWrite(BEEP_OUTPUT, LOW);
      sleep(d);
  }

  void short_beep() {
    beep(50);
  }

  void long_beep() {
    beep(500);
  }

  void short_beep_series(int count) {
    for(int i=0; i<count; i++) {
      short_beep();
    }
  }

}
