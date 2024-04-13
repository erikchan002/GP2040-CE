#ifndef _ANSG08_H_
#define _ANSG08_H_

#include "peripheral_i2c.h"
// #include "pico/stdlib.h"
// #include "hardware/gpio.h"
// #include "pico/binary_info.h"
// #include "hardware/i2c.h"

#define ANSG08_DEFAULT_I2C_ADDRESS 0x24

typedef enum {
  ANSG08_CLOCK_SPEED_FAST = 0b00,
  ANSG08_CLOCK_SPEED_NORMAL = 0b01,
  ANSG08_CLOCK_SPEED_SLOW = 0b10,
  ANSG08_CLOCK_SPEED_SLOWEST = 0b11
} ansgClockSpeed_t;

typedef enum { ANSG08_CALIBRATION_SPEED_FAST = 0b00, ANSG08_CALIBRATION_SPEED_NORMAL = 0b10, ANSG08_CALIBRATION_SPEED_SLOW = 0b11 } ansgCalibrationSpeed_t;

typedef enum { ANSG08_OPERATION_MODE_NORMAL = 0b0, ANSG08_OPERATION_MODE_BFMODE = 0b1 } ansgOperationMode_t;

typedef enum { ANSG08_OUTPUT_MODE_SINGLE = 0b0, ANSG08_OUTPUT_MODE_MULTI = 0b1 } ansgOutputMode_t;

typedef enum { ANSG08_IMPEDANCE_HIGH = 0b0, ANSG08_IMPEDANCE_LOW = 0b1 } ansgImpedance_t;

typedef enum {
  ANSG08_BF_CALIBRATION_SPEED_FASTEST = 0b00,
  ANSG08_BF_CALIBRATION_SPEED_FAST = 0b01,
  ANSG08_BF_CALIBRATION_SPEED_NORMAL = 0b10,
  ANSG08_BF_CALIBRATION_SPEED_SLOW = 0b11
} ansgBFCalibrationSpeed_t;

typedef enum {
  ANSG08_BS_CALIBRATION_SPEED_FASTEST = 0b00,
  ANSG08_BS_CALIBRATION_SPEED_FAST = 0b01,
  ANSG08_BS_CALIBRATION_SPEED_NORMAL = 0b10,
  ANSG08_BS_CALIBRATION_SPEED_SLOW = 0b11
} ansgBSCalibrationSpeed_t;

class ANSG08 {
 protected:
  uint8_t address;

 public:
  static constexpr uint8_t OUTPUT_REGISTER_ADDRESS = 0x2a;
  static constexpr uint8_t GLOBAL_CONTROL_1_REGISTER_ADDRESS = 0x36;
  static constexpr uint8_t CLOCK_CONTROL_REGISTER_ADDRESS = 0x34;
  static constexpr uint8_t GLOBAL_CONTROL_2_REGISTER_ADDRESS = 0x38;
  static constexpr uint8_t SENSITIVITY_REGISTER_ADDRESS = 0x39;
  static constexpr uint8_t BF_CALIBRATION_SPEED_REGISTER_ADDRESS = 0x41;
  static constexpr uint8_t BS_CALIBRATION_SPEED_REGISTER_ADDRESS = 0x42;
  static constexpr uint8_t EEPROM_COMMAND_REGISTER_ADDRESS = 0x5c;
  static constexpr uint8_t EEPROM_CHECK_1_REGISTER_ADDRESS = 0x00;
  static constexpr uint8_t EEPROM_CHECK_1_VALID_VALUE = 0b10101010;
  static constexpr uint8_t EEPROM_CHECK_2_REGISTER_ADDRESS = 0x7f;
  static constexpr uint8_t EEPROM_CHECK_2_VALID_VALUE = 0b01010101;

  static constexpr uint8_t EEPROM_COMMAND_READ_START = 1;
  static constexpr uint8_t EEPROM_COMMAND_WRITE_START = 1 << 1;
  static constexpr uint8_t EEPROM_COMMAND_READ_ALL = 1 << 4;
  static constexpr uint8_t EEPROM_COMMAND_WRITE_ALL = 1 << 5;

  static constexpr size_t BUFFER_SIZE = 128;

  ANSG08(PeripheralI2C* i2c, uint8_t address = ANSG08_DEFAULT_I2C_ADDRESS);
  // ANSG08(uint8_t sda, uint8_t scl, i2c_inst_t* i2c,
  //        int32_t speed, uint8_t address = ANSG08_DEFAULT_I2C_ADDRESS);

  void begin();
  void softReset();
  void setClockControl(ansgCalibrationSpeed_t rb_sel, ansgClockSpeed_t clk_sel,
                       unsigned int init_cal_opt_seconds);
  void setGlobalControl(bool software_rst, ansgOperationMode_t bf_mode,
                        uint8_t response_ctrl, uint8_t response_off_ctrl,
                        bool clk_off, unsigned int cal_hold_time_seconds,
                        ansgOutputMode_t sin_multi_mode,
                        ansgImpedance_t imp_sel);
  void setGlobalControl(ansgOperationMode_t bf_mode, uint8_t response_ctrl,
                        uint8_t response_off_ctrl, bool clk_off,
                        unsigned int cal_hold_time_seconds,
                        ansgOutputMode_t sin_multi_mode,
                        ansgImpedance_t imp_sel);
  void setSensitivity(uint8_t channel, double sensitivityPercentage);
  void setBFCalibrationSpeed(ansgBFCalibrationSpeed_t sen_bf_down,
                             ansgBFCalibrationSpeed_t sen_bf_up,
                             ansgBFCalibrationSpeed_t rnd_bf_down,
                             ansgBFCalibrationSpeed_t rnd_bf_up);
  void setBSCalibrationSpeed(ansgBSCalibrationSpeed_t sen_bs_down,
                             ansgBSCalibrationSpeed_t sen_bs_up,
                             ansgBSCalibrationSpeed_t rnd_bs_down,
                             ansgBSCalibrationSpeed_t rnd_bs_up);
  void loadConfig();
  void saveConfig();
  // void factoryResetConfig();

  uint8_t readOutput();

 private:
  PeripheralI2C* i2c;
  // i2c_inst_t* i2c;
  // uint8_t sda, scl;
  // int32_t speed;
  uint8_t buffer[BUFFER_SIZE];

  int readRegistersIntoBuffer(uint8_t startRegisterAddress,
                               size_t length);
  uint8_t readRegister(uint8_t registerAddress);

  int writeRegistersFromBuffer(uint8_t startRegisterAddress,
                                size_t length);
  void writeRegister(uint8_t registerAddress, uint8_t data);

};

#endif
