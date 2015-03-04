
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(P1_6, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(P1_6, HIGH);
  sleep(1000);
  digitalWrite(P1_6, LOW);
  sleep(1000);
}
