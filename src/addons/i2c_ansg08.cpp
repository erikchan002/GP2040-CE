#include "addons/i2c_ansg08.h"

#include "config.pb.h"
#include "helper.h"
#include "storagemanager.h"

bool I2CANSG08Input::available() {
  const ANSG08Options& options =
      Storage::getInstance().getAddonOptions().ansg08Options;
  return (options.enabled && isValidPin(options.i2cSDAPin) &&
          isValidPin(options.i2cSCLPin));
}

void I2CANSG08Input::setup() {
  const ANSG08Options& options =
      Storage::getInstance().getAddonOptions().ansg08Options;

  statuses[0] = statuses[1] = statuses[2] = statuses[3] = 0;

  uIntervalMS = 1;
  nextTimer = getMillis();

  ansg08s[0] = new ANSG08(options.i2cSDAPin, options.i2cSCLPin,
                          options.i2cBlock == 0 ? i2c0 : i2c1, options.i2cSpeed,
                          options.i2cAddress0);
  ansg08s[0]->begin();

  ansg08s[1] = new ANSG08(options.i2cSDAPin, options.i2cSCLPin,
                          options.i2cBlock == 0 ? i2c0 : i2c1, options.i2cSpeed,
                          options.i2cAddress1);
  ansg08s[1]->begin();

  ansg08s[2] = new ANSG08(options.i2cSDAPin, options.i2cSCLPin,
                          options.i2cBlock == 0 ? i2c0 : i2c1, options.i2cSpeed,
                          options.i2cAddress2);
  ansg08s[2]->begin();

  ansg08s[3] = new ANSG08(options.i2cSDAPin, options.i2cSCLPin,
                          options.i2cBlock == 0 ? i2c0 : i2c1, options.i2cSpeed,
                          options.i2cAddress3);
  ansg08s[3]->begin();
}

inline uint8_t bitReverse(uint8_t input) {
  // bit hack
  // https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits
  return static_cast<uint8_t>(((input * 0x0802LU & 0x22110LU) | (input * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
}

void I2CANSG08Input::process() {
  if (nextTimer < getMillis()) {
    for (uint8_t i = 0; i < 4; ++i) {
      statuses[i] = ansg08s[i]->readOutput();
    }
  }

  // MSB RY - RX - LY - LX LSB
  // LSB  0 -  1 -  2 -  3 MSB
  Gamepad* gamepad = Storage::getInstance().GetGamepad();
  gamepad->state.ry = static_cast<uint16_t>((bitReverse(statuses[0]) ^ 0x80) << 8);
  gamepad->state.rx = static_cast<uint16_t>((bitReverse(statuses[1]) ^ 0x80) << 8);
  gamepad->state.ly = static_cast<uint16_t>((bitReverse(statuses[2]) ^ 0x80) << 8);
  gamepad->state.lx = static_cast<uint16_t>((bitReverse(statuses[3]) ^ 0x80) << 8);
}