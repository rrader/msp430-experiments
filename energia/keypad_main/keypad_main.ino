#include <MspFlash.h>
#include <Keypad.h>
#include "AnyMsTimer.h"
#include "PasswordStorage.h"
#include "Beeps.h"
#include "KeypadInput.h"


void setup() {
  KeypadInput::setup();
  Beeps::setup();
  PasswordStorage::setup();

  AnyMsTimer::set(50, KeypadInput::scanKeyboard);
  AnyMsTimer::start();
}

void loop() {
}

