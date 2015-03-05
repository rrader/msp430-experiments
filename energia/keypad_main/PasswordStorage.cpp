#include <Energia.h>
#include "../../libraries/MspFlash/MspFlash.h"
#include "PasswordStorage.h"

#define flash SEGMENT_D


namespace PasswordStorage{
  unsigned const int PASSWORD_LENGTH = 10;
  
  unsigned char currentPassword[PASSWORD_LENGTH];  // current password
  unsigned char superPassword[] = "42AB91$";

  void setup() {
    readPassword();
  }
  
  void readPassword() {
    Flash.read(flash, currentPassword, PASSWORD_LENGTH);
  }

  void resetPasswordToDefault() {
    Flash.erase(flash);
    Flash.write(flash, (unsigned char*) "4242$" ,5);
    Flash.read(flash, currentPassword, PASSWORD_LENGTH);
  }
  
  void saveNewPassword(unsigned char* newPassword, unsigned int len) {
    Flash.erase(flash);
    Flash.write(flash, newPassword, len);
    Flash.write(flash + len, (unsigned char*) "$", 1);
    Flash.read(flash, currentPassword, PASSWORD_LENGTH);
  }

  bool check(unsigned char* password, unsigned char* expected) {
    int i=0;
    while(password[i] == expected[i] && expected[i] != '$') {
      i++;
    }
    return expected[i] == '$';
  }

  bool checkSuperPassword(unsigned char* password) {
    return check(password, superPassword);
  }

  bool checkPassword(unsigned char* password) {
    return check(password, currentPassword);
  }
}
