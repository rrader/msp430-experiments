#include "AnyMsTimer.h"

unsigned long pulseDelay;
unsigned long lastPulse;

long pulseInterval = 0;

bool siren = false;
bool warning = false;

const float ALPHA = 0.1;
const long MAX_DELAY = 60000;

void setup()
{
  pinMode(P1_6, OUTPUT);
  digitalWrite(P1_6, LOW);
  pinMode(P2_2, INPUT);
  
  lastPulse = MAX_DELAY;
  pulseDelay = lastPulse;
  
  attachInterrupt(P2_2, readPulse, RISING); // Interrupt is fired whenever button is pressed
  
  Serial.begin(9600);
  Serial.println("----------------------Vibration demo------------------------");

  AnyMsTimer::set(50, vibrationMeasure);
  AnyMsTimer::start();
}

void loop() {
  if (siren) {
    beep2();
  } else
  if (warning) {
    beep();
    delay(200);
  }
//    Serial.print("#");
}

void vibrationMeasure() {
  long measurement = pulseInterval;
  pulseInterval = 0;

  if (measurement > 500){
    lastPulse = millis();
    Serial.print(">> ");
    Serial.println(pulseDelay);
  }
  unsigned long diff = millis() - lastPulse;
  if (diff > MAX_DELAY)
    diff = MAX_DELAY;
  pulseDelay = ALPHA*diff + (1-ALPHA)*pulseDelay;
  //Serial.println(pulseDelay);
 
  if (pulseDelay < 1000) {
    if (!siren) {
      Serial.println("S");
      siren = true;
    }
  } else {
    if (siren) {
      Serial.println("!S");
      siren = false;
    }
  }

  if (pulseDelay < 10000) {
    if (!warning) {
      Serial.println("W");
      warning = true;
//      digitalWrite(P1_6, HIGH);
    }
  } else {
    if (warning) {
      Serial.println("!W");
      warning = false;
//      digitalWrite(P1_6, LOW);
    }
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

long TP_init(){
//  delay(10);
  long measurement=pulseIn(P2_2, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}

void readPulse() {
  pulseInterval = pulseIn(P2_2, HIGH);
}
