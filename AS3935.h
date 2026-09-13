#ifndef AS3935_H
#define AS3935_H

#include <stdint.h>

// AS3935 Register Addresses
#define AS3935_AFE_GAIN         0x00
#define AS3935_THRESHOLD        0x01
#define AS3935_INT_CFG          0x02
#define AS3935_INT_MASK         0x03
#define AS3935_ENERGY_LSBS      0x04
#define AS3935_ENERGY_MIDS      0x05
#define AS3935_ENERGY_MSBS      0x06
#define AS3935_DISTANCE         0x07
#define AS3935_DISP_LCAP        0x08
#define AS3935_DISP_SRCO        0x3D
#define AS3935_DISP_TRCO        0x3E
#define AS3935_DISP_PRESET      0x3F

// Interrupt Types
#define AS3935_INT_NOISE        0x01
#define AS3935_INT_DISTURB      0x02
#define AS3935_INT_LIGHTNING    0x08

// Default I2C Address
#define AS3935_I2C_ADDR         0x03

class AS3935 {
public:
    AS3935();
    
    // Initialization
    bool begin(uint8_t i2c_addr = AS3935_I2C_ADDR);
    void reset();
    
    // Configuration
    void setIndoorOutdoor(bool indoor);
    void setNoiseLevel(uint8_t level);
    void setWatchdogThreshold(uint8_t level);
    void setAFEGain(uint8_t gain);
    
    // Interrupt handling
    uint8_t getInterruptType();
    uint8_t getDistance();
    uint32_t getEnergyLevel();
    
    // Read/Write operations
    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);
    
private:
    uint8_t _i2c_addr;
    uint8_t _noise_level;
};

#endif // AS3935_H