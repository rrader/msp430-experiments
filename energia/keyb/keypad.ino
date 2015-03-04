bool changing_password = false;

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

void scan_keyboard() {
  char key = keypad.getKey();

  if (key == 'D') {
    if (changing_password) {
      PasswordStorage::saveNewPassword(buf, p);
      changing_password = false;
      short_beep_series(10);
      reset_buffer();
    }

    if (PasswordStorage::checkSuperPassword(buf)) {
      PasswordStorage::resetPasswordToDefault();
      short_beep_series(20);
      reset_buffer();
    } else {
      if (PasswordStorage::checkPassword(buf)) {
        short_beep_series(5);
        changing_password = true;
        reset_buffer();
      }
    }
  } else
  if (key == '*') {
    //reset
    reset_buffer();
    long_beep();
  } else if (key == '#') {
    if (PasswordStorage::checkPassword(buf))
      short_beep_series(3);
    else
      long_beep();
    reset_buffer();
  } else if (key) {
    buf[p++] = key;
    short_beep();
  }
}

void reset_buffer() {
  p = 0;
  for(int i=0; i<10; i++)
    buf[i] = 0;
}

