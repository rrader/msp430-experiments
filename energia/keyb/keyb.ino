#include <MspFlash.h>
#include <Keypad.h>
#include "AnyMsTimer.h"
#include "PasswordStorage.h"

#define BEEP_OUTPUT P1_6


void setup() {                
  pinMode(P1_0, INPUT);
  pinMode(P1_1, INPUT);
  pinMode(P1_2, INPUT);
  pinMode(P1_3, INPUT);
  pinMode(P1_4, OUTPUT);
  pinMode(P1_5, OUTPUT);
  pinMode(P2_0, OUTPUT);
  pinMode(P2_1, OUTPUT);

  pinMode(BEEP_OUTPUT, OUTPUT);
  
  PasswordStorage::readPassword();

  AnyMsTimer::set(50, scan_keyboard);
  AnyMsTimer::start();
}

void loop() {
}

