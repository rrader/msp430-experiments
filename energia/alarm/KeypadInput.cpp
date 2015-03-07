#include "KeypadInput.h"

#include <Energia.h>
#include "../../libraries/Keypad/Keypad.h"

#include "PasswordStorage.h"
#include "Beeps.h"

using namespace Beeps;

namespace KeypadInput {

  bool isChangingPassword = false;

  byte rowPins[] = {P1_4, P1_5, P2_0, P2_1};
  byte colPins[] = {P1_0, P1_1, P1_2, P1_3};

  const byte rows = 4;
  const byte cols = 4;
  char keys[rows][cols] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
  };

  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

  void setup() {
    for(int i=0; i<rows; i++)
      pinMode(rowPins[i], OUTPUT);
    for(int i=0; i<cols; i++)
      pinMode(colPins[i], INPUT);
  }

  unsigned char buf[10];
  int p = 0;
  
  void resetBuffer() {
    p = 0;
    for(int i=0; i<10; i++)
      buf[i] = 0;
  }
  
  void passwordManagement() {
    if (isChangingPassword) {
      PasswordStorage::saveNewPassword(buf, p);
      isChangingPassword = false;
      short_beep_series(10);
      resetBuffer();
    }

    if (PasswordStorage::checkSuperPassword(buf)) {
      PasswordStorage::resetPasswordToDefault();
      short_beep_series(20);
      resetBuffer();
    } else {
      if (PasswordStorage::checkPassword(buf)) {
        short_beep_series(5);
        isChangingPassword = true;
        resetBuffer();
      }
    }
  }

  void characterTyped(char key) {
    buf[p++] = key;
    short_beep();
  }

  void resetInput() {
    resetBuffer();
    long_beep();
  }

  void tryToAuthenticate() {
    if (PasswordStorage::checkPassword(buf))
      short_beep_series(3);
    else
      long_beep();
    resetBuffer();
  }

  void scanKeyboard() {
    char key = keypad.getKey();
    if (!key) return;

    if (key == 'D') {
      passwordManagement();
    }
    else if (key == '*') {
      resetInput();
    }
    else if (key == '#') {
      tryToAuthenticate();
    }
    else {
      characterTyped(key);
    }
  }

}
