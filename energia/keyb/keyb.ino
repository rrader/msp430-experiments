#include <MspFlash.h>
#include <Keypad.h>

#define flash SEGMENT_D

unsigned char password[10];
unsigned char superpassword[] = "42AB91$";

bool changing_password = false;

void setup() {                
  pinMode(P1_0, INPUT);
  pinMode(P1_1, INPUT);
  pinMode(P1_2, INPUT);
  pinMode(P1_3, INPUT);
  pinMode(P1_4, OUTPUT);
  pinMode(P1_5, OUTPUT);
  pinMode(P2_0, OUTPUT);
  pinMode(P2_1, OUTPUT);

  pinMode(P1_6, OUTPUT);
  
  Flash.read(flash, password, 10);
  if (password[0] == 0) {
    Flash.write(flash, (unsigned char*) "4242$" ,5);
    Flash.read(flash, password, 10);
  }
}

byte rowPins[] = {P1_4, P1_5, P2_0, P2_1};
byte colPins[] = {P1_0, P1_1, P1_2, P1_3};

int counts[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

unsigned char buf[10];
int p = 0;

void loop() {
  char key = keypad.getKey();

  if (key == 'D') {
    if (changing_password) {
      Flash.erase(flash);
      Flash.write(flash, buf, p);
      Flash.write(flash + p, (unsigned char*) "$", 1);
      Flash.read(flash, password, 10);
      changing_password = false;
      short_beep_series(10);
      reset_buffer();
    }

    if (check_password(superpassword)) {
      Flash.erase(flash);
      Flash.write(flash, (unsigned char*) "4242$" ,5);
      Flash.read(flash, password, 10);
      short_beep_series(20);
      reset_buffer();
    } else {
      if (check_password(password)) {
        short_beep_series(5);
        changing_password = true;
        reset_buffer();
      }
    }
  } else
  if (key == '*') {
    reset_buffer();
    long_beep();
  } else if (key == '#') {
    if (check_password(password))
      short_beep_series(3);
    else
      long_beep();
    reset_buffer();
  } else if (key) {
    buf[p++] = key;
    short_beep();
  }
}

bool check_password(unsigned char* password) {
  int i=0;
  while(buf[i] == password[i] && password[i] != '$') {
    i++;
  }
  return password[i] == '$';
}

void reset_buffer() {
  p = 0;
  for(int i=0; i<10; i++)
    buf[i] = 0;
}

void beep(int d) {
    digitalWrite(P1_6, HIGH);
    sleep(d);
    digitalWrite(P1_6, LOW);
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
