#include "AS3935.h"
#include <Wire.h>

AS3935::AS3935() : _i2c_addr(AS3935_I2C_ADDR), _noise_level(2) {
}

bool AS3935::begin(uint8_t i2c_addr) {
    _i2c_addr = i2c_addr;
    Wire.begin();
    
    // Check if device is present
    Wire.beginTransmission(_i2c_addr);
    if (Wire.endTransmission() != 0) {
        return false;
    }
    
    reset();
    return true;
}

void AS3935::reset() {
    // Write preset default values
    writeRegister(AS3935_DISP_PRESET, 0x96);
    delay(2);
}

void AS3935::setIndoorOutdoor(bool indoor) {
    uint8_t reg = readRegister(AS3935_AFE_GAIN);
    if (indoor) {
        reg |= (1 << 5);  // Set indoor mode
    } else {
        reg &= ~(1 << 5); // Set outdoor mode
    }
    writeRegister(AS3935_AFE_GAIN, reg);
}

void AS3935::setNoiseLevel(uint8_t level) {
    if (level > 7) level = 7;
    _noise_level = level;
    
    uint8_t reg = readRegister(AS3935_THRESHOLD);
    reg &= 0x0F;  // Clear upper 4 bits
    reg |= (level << 4);
    writeRegister(AS3935_THRESHOLD, reg);
}

void AS3935::setWatchdogThreshold(uint8_t level) {
    if (level > 15) level = 15;
    
    uint8_t reg = readRegister(AS3935_THRESHOLD);
    reg &= 0xF0;  // Clear lower 4 bits
    reg |= level;
    writeRegister(AS3935_THRESHOLD, reg);
}

void AS3935::setAFEGain(uint8_t gain) {
    if (gain > 7) gain = 7;
    
    uint8_t reg = readRegister(AS3935_AFE_GAIN);
    reg &= 0xF1;  // Clear gain bits
    reg |= (gain << 1);
    writeRegister(AS3935_AFE_GAIN, reg);
}

uint8_t AS3935::getInterruptType() {
    uint8_t reg = readRegister(AS3935_INT_MASK);
    return (reg & 0x0F);
}

uint8_t AS3935::getDistance() {
    return readRegister(AS3935_DISTANCE) & 0x3F;
}

uint32_t AS3935::getEnergyLevel() {
    uint32_t msb = readRegister(AS3935_ENERGY_MSBS);
    uint32_t mid = readRegister(AS3935_ENERGY_MIDS);
    uint32_t lsb = readRegister(AS3935_ENERGY_LSBS);
    
    return ((msb & 0x0F) << 16) | (mid << 8) | lsb;
}

uint8_t AS3935::readRegister(uint8_t reg) {
    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(_i2c_addr, 1);
    return Wire.read();
}

void AS3935::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}