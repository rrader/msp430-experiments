#include <MspFlash.h>
#include <Keypad.h>
#include "AnyMsTimer.h"
#include "PasswordStorage.h"
#include "Beeps.h"
#include "KeypadInput.h"
#include "VibroAlarm.h"


void setup()
{
  // put your setup code here, to run once:
  pinMode(P1_6, OUTPUT);
  pinMode(P1_7, OUTPUT);
  
  AnyMsTimer::set(1000, timer, 0);
  AnyMsTimer::start(0);
  AnyMsTimer::set(3000, timer2, 1);
  AnyMsTimer::start(1);
}

bool t0 = false;
bool t1 = false;

void loop()
{
  if (t0) {
    t0 = false;
    digitalWrite(P1_6, HIGH);
    sleep(50);
    digitalWrite(P1_6, LOW);
    sleep(50);
  }

  if (t1) {
    t1 = false;
    digitalWrite(P1_7, HIGH);
    sleep(50);
    digitalWrite(P1_7, LOW);
    sleep(50);
  }
}

void timer() {
  t0 = true;
}

void timer2() {
  t1 = true;
}
