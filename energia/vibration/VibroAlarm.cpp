#include "VibroAlarm.h"
#include "Energia.h"

namespace VibroAlarm {

unsigned long pulseDelay;
unsigned long lastPulse;

long pulseInterval = 0;

bool siren = false;
bool warning = false;

const float ALPHA = 0.1;
const long MAX_DELAY = 60000;

void readPulse() {
  pulseInterval = max(pulseInterval, pulseIn(P2_2, HIGH));
}

void setup() {
  pinMode(P2_2, INPUT);

  lastPulse = MAX_DELAY;
  pulseDelay = lastPulse;
  
  attachInterrupt(P2_2, readPulse, RISING);
}

void vibrationMeasure() {
  long measurement = pulseInterval;
  pulseInterval = 0;

  if (measurement > 500){
    lastPulse = millis();
  }
  unsigned long diff = millis() - lastPulse;
  if (diff > MAX_DELAY)
    diff = MAX_DELAY;
  pulseDelay = ALPHA*diff + (1-ALPHA)*pulseDelay;
 
  siren = (pulseDelay < 1000);
  warning = (pulseDelay < 10000);
}

}
