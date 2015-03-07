#include "AnyMsTimer.h"
#include "VibroAlarm.h"

void setup()
{
  VibroAlarm::setup();
  AnyMsTimer::set(50, VibroAlarm::vibrationMeasure);
  AnyMsTimer::start();

  pinMode(P1_6, OUTPUT);
  digitalWrite(P1_6, LOW);
}

void loop() {
  if (VibroAlarm::siren) {
    beep2();
  } else
  if (VibroAlarm::warning) {
    beep();
    delay(200);
  }
}

void beep() {
  digitalWrite(P1_6, HIGH);
  delay(20);
  digitalWrite(P1_6, LOW);
  delay(20);
}

void beep2() {
  digitalWrite(P1_6, HIGH);
  delay(1000);
  digitalWrite(P1_6, LOW);
}
