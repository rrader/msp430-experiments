#ifndef PasswordStorage_h
#define PasswordStorage_h
#include "Energia.h"


namespace PasswordStorage {
  void readPassword();
  void resetPasswordToDefault();
  void saveNewPassword(unsigned char* newPassword, unsigned int len);

  bool check(unsigned char* password, unsigned char* expected);
  bool checkSuperPassword(unsigned char* password);
  bool checkPassword(unsigned char* password);
}

#endif
