#ifndef _I2C_ANSG08_H
#define _I2C_ANSG08_H

#include <ANSG08.h>

#include "GamepadEnums.h"
#include "gpaddon.h"

#ifndef I2C_ANSG08_ENABLED
#define I2C_ANSG08_ENABLED 0
#endif

#ifndef I2C_ANSG08_SDA_PIN
#define I2C_ANSG08_SDA_PIN -1
#endif

#ifndef I2C_ANSG08_SCL_PIN
#define I2C_ANSG08_SCL_PIN -1
#endif

#ifndef I2C_ANSG08_BLOCK
#define I2C_ANSG08_BLOCK i2c0
#endif

#ifndef I2C_ANSG08_SPEED
#define I2C_ANSG08_SPEED 400000
#endif

#ifndef I2C_ANSG08_ADDRESS_0
#define I2C_ANSG08_ADDRESS_0 ANSG08_DEFAULT_I2C_ADDRESS
#endif

#ifndef I2C_ANSG08_ADDRESS_1
#define I2C_ANSG08_ADDRESS_1 ANSG08_DEFAULT_I2C_ADDRESS + 1
#endif

#ifndef I2C_ANSG08_ADDRESS_2
#define I2C_ANSG08_ADDRESS_2 ANSG08_DEFAULT_I2C_ADDRESS + 2
#endif

#ifndef I2C_ANSG08_ADDRESS_3
#define I2C_ANSG08_ADDRESS_3 ANSG08_DEFAULT_I2C_ADDRESS + 3
#endif

#define I2CANSG08Name "I2CANSG08"

class I2CANSG08Input : public GPAddon {
 public:
  virtual bool available();
  virtual void setup();
  virtual void preprocess() {}
  virtual void process();
  virtual std::string name() { return I2CANSG08Name; }

 private:
  ANSG08* ansg08s[4];
  uint8_t statuses[4];
  uint32_t uIntervalMS;
  uint32_t nextTimer;
};

#endif  // _I2C_ANSG08_H
