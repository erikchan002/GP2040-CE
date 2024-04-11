#include "ANSG08.h"

#include <algorithm>
#include <cstring>

ANSG08::ANSG08(uint8_t sda, uint8_t scl, i2c_inst_t *i2c,
               int32_t speed, uint8_t address)
    : address(address), i2c(i2c), sda(sda), scl(scl), speed(speed) { }

void ANSG08::begin()
{
	if ((sda + 2 * i2c_hw_index(i2c))%4 != 0) return ;
	if ((scl + 3 + 2 * i2c_hw_index(i2c))%4 != 0) return ;

  i2c_init(i2c, speed);
  gpio_set_function(sda, GPIO_FUNC_I2C);
  gpio_set_function(scl, GPIO_FUNC_I2C);
  gpio_pull_up(sda);
  gpio_pull_up(scl);

  // sleep_ms(200);
  softReset();
}

void ANSG08::softReset()
{
  if (readRegistersIntoBuffer(GLOBAL_CONTROL_1_REGISTER_ADDRESS, 1) >= 0){
    writeRegister(GLOBAL_CONTROL_1_REGISTER_ADDRESS,buffer[0] | 0b1);
    // sleep_ms(1);
    writeRegister(GLOBAL_CONTROL_1_REGISTER_ADDRESS,buffer[0] & ~0b1);
  }
}

void ANSG08::setClockControl(ansgCalibrationSpeed_t rb_sel,
                             ansgClockSpeed_t clk_sel,
                             unsigned int init_cal_opt_seconds)
{
  uint8_t init_cal_opt = std::clamp((init_cal_opt_seconds - 5) / 5, 0U, 0b111U);
  writeRegister(CLOCK_CONTROL_REGISTER_ADDRESS,
                rb_sel | (clk_sel << 2) | (init_cal_opt << 5));
}

void ANSG08::setGlobalControl(bool software_rst, ansgOperationMode_t bf_mode,
                              uint8_t response_ctrl, uint8_t response_off_ctrl,
                              bool clk_off, unsigned int cal_hold_time_seconds,
                              ansgOutputMode_t sin_multi_mode,
                              ansgImpedance_t imp_sel)
{
  uint8_t cal_hold_time = std::clamp(cal_hold_time_seconds / 4, 1U, 0b1111U);
  writeRegister(GLOBAL_CONTROL_1_REGISTER_ADDRESS,
                software_rst | (bf_mode << 1) | ((response_ctrl & 0b111) << 2) |
                    ((response_off_ctrl & 0b111) << 5));
  writeRegister(
      GLOBAL_CONTROL_2_REGISTER_ADDRESS,
      clk_off | (cal_hold_time_seconds == 0 ? 0 : (cal_hold_time << 2)) | (sin_multi_mode << 6) | (imp_sel << 7));
}

void ANSG08::setGlobalControl(ansgOperationMode_t bf_mode,
                              uint8_t response_ctrl, uint8_t response_off_ctrl,
                              bool clk_off, unsigned int cal_hold_time_seconds,
                              ansgOutputMode_t sin_multi_mode,
                              ansgImpedance_t imp_sel)
{
  setGlobalControl(false, bf_mode, response_ctrl, response_off_ctrl, clk_off,
                   cal_hold_time_seconds, sin_multi_mode, imp_sel);
}

void ANSG08::setSensitivity(uint8_t channel, double sensitivityPercentage)
{
  writeRegister(SENSITIVITY_REGISTER_ADDRESS +
                    std::clamp(channel, uint8_t{0}, uint8_t{7}),
                std::clamp(sensitivityPercentage * 40, 0.0, 255.0));
}

void ANSG08::setBFCalibrationSpeed(ansgBFCalibrationSpeed_t sen_bf_down,
                                   ansgBFCalibrationSpeed_t sen_bf_up,
                                   ansgBFCalibrationSpeed_t rnd_bf_down,
                                   ansgBFCalibrationSpeed_t rnd_bf_up)
{
  writeRegister(
      BF_CALIBRATION_SPEED_REGISTER_ADDRESS,
      sen_bf_down | (sen_bf_up << 2) | (rnd_bf_down << 4) | (rnd_bf_up << 6));
}

void ANSG08::setBSCalibrationSpeed(ansgBSCalibrationSpeed_t sen_bs_down,
                                   ansgBSCalibrationSpeed_t sen_bs_up,
                                   ansgBSCalibrationSpeed_t rnd_bs_down,
                                   ansgBSCalibrationSpeed_t rnd_bs_up)
{
  writeRegister(
      BS_CALIBRATION_SPEED_REGISTER_ADDRESS,
      sen_bs_down | (sen_bs_up << 2) | (rnd_bs_down << 4) | (rnd_bs_up << 6));
}

void ANSG08::loadConfig()
{
  writeRegister(
      EEPROM_COMMAND_REGISTER_ADDRESS,
      EEPROM_COMMAND_READ_ALL | EEPROM_COMMAND_READ_START);
  uint8_t eepromCommand;
  do
  {
    sleep_us(10);
    eepromCommand = readRegister(EEPROM_COMMAND_REGISTER_ADDRESS);
  } while (eepromCommand & EEPROM_COMMAND_READ_START);
}

void ANSG08::saveConfig()
{
  writeRegister(EEPROM_CHECK_1_REGISTER_ADDRESS,
                EEPROM_CHECK_1_VALID_VALUE);
  writeRegister(EEPROM_CHECK_2_REGISTER_ADDRESS,
                EEPROM_CHECK_2_VALID_VALUE);
  writeRegister(
      EEPROM_COMMAND_REGISTER_ADDRESS,
      EEPROM_COMMAND_WRITE_ALL | EEPROM_COMMAND_WRITE_START);
  uint8_t eepromCommand;
  do
  {
    sleep_ms(1);
    eepromCommand = readRegister(EEPROM_COMMAND_REGISTER_ADDRESS);
  } while (eepromCommand & EEPROM_COMMAND_WRITE_START);
}

uint8_t ANSG08::readOutput()
{
  return readRegister(OUTPUT_REGISTER_ADDRESS);
}

int ANSG08::readRegistersIntoBuffer(uint8_t startRegisterAddress, size_t length)
{
  i2c_write_blocking(i2c, address, &startRegisterAddress, 1, true);
  return i2c_read_blocking(i2c, address, buffer, std::min(length,BUFFER_SIZE), false);
}

uint8_t ANSG08::readRegister(uint8_t registerAddress)
{
  if (readRegistersIntoBuffer(registerAddress, 1) >= 0){
    return buffer[0];
  }
  return 0;
}

int ANSG08::writeRegistersFromBuffer(uint8_t startRegisterAddress, size_t length)
{
  memmove(buffer+1,buffer,std::min(length,static_cast<size_t>(BUFFER_SIZE-1)));
  buffer[0] = startRegisterAddress;
  return i2c_write_blocking(i2c, address, buffer, std::min(static_cast<size_t>(length+1),BUFFER_SIZE), false);
}

void ANSG08::writeRegister(uint8_t registerAddress, uint8_t data)
{
  buffer[0] = data;
  writeRegistersFromBuffer(registerAddress, 1);
}
