int incomingByte = 0;

void setup(){
  Serial.begin(9600);
  pinMode(P1_6, OUTPUT);
  pinMode(P1_7, OUTPUT);
  digitalWrite(P1_6, LOW);
  digitalWrite(P1_7, LOW);
}

int isOK() {
  if (incomingByte == 'O') {
    incomingByte = Serial.read();
    if (incomingByte == 'K') {
      return 1;
    }
  }
  return 0;
}

// the loop routine runs over and over again forever:
void loop() {
  boolean ok=false;
  do {
    do {
      digitalWrite(P1_6, HIGH);
      Serial.println("AT");
      digitalWrite(P1_6, LOW);
      delay(500);
    } while (!Serial.available());
    incomingByte = Serial.read();
    while(Serial.available() && incomingByte == 255)
      incomingByte = Serial.read();
    while(Serial.available()) {
      if (isOK())
        ok = true;
      else
        incomingByte = Serial.read();
    }
  } while (!ok);
  digitalWrite(P1_7, HIGH);
//  incomingByte = Serial.read();
//  delay(1000);
  digitalWrite(P1_6, HIGH);

  delay(1000);
  Serial.println("AT+IPR=9600");
  delay(1000);
//  Serial.println("at+cusd=1,\"*101#\"");
  Serial.println("AT+GMM");
  delay(1000);
  Serial.println("AT+CSQ");
  delay(1000);
  Serial.println("AT+CSPN?");
  delay(1000);
  Serial.println("AT+COPS");
  delay(1000);
  Serial.println("AT+CPAS");
  delay(1000);
  Serial.println("AT+CSCS");
  delay(1000);
  Serial.println("AT+CMEE=1");
  delay(1000);
  Serial.println("AT+CFUN=1");
  delay(1000);
  Serial.println("AT+COPS?");
  delay(1000);
  Serial.println("AT+COPS?");
  delay(1000);
  Serial.println("AT+CPAS");
  delay(1000);
  Serial.println("AT+CREG=1");
  delay(1000);
  Serial.println("AT+CREG?");
  delay(1000);
  Serial.println("AT+CFUN?");
  delay(1000);
  Serial.println("AT+CPIN?");
  delay(1000);
  Serial.println("at+cusd=1,\"*101#\",15");

  while(1);
}
