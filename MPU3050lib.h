// Code by Blaise Jarrett @ OSEPP http://blaisejarrett.com
// Released to the public domain! Enjoy!

#ifndef _MPU3050LIB_H_
#define _MPU3050LIB_H_

#include <stdint.h>

#define OSEPP_GYRO_SW_ON 0x68
#define OSEPP_GYRO_SW_OFF 0x69
#define MPU3050_I2CADD 0x1E

#define MPU3050_ERROR_NOT_INITIALIZED -2
#define MPU3050_ERROR_WRONG_ID -3
#define MPU3050_ERROR_I2CREAD -4
#define MPU3050_ERROR_I2CWRITE -5
#define MPU3050_ERROR_LOWPASSOUTOFRANGE -6
#define MPU3050_ERROR_RANGEOUTOFRANGE -7

// Register Map
#define MPU3050_REG_WHOAMI 0
#define MPU3050_REG_XOFFH 12
#define MPU3050_REG_XOFFL 13
#define MPU3050_REG_YOFFH 14
#define MPU3050_REG_YOFFL 15
#define MPU3050_REG_ZOFFH 16
#define MPU3050_REG_ZOFFL 17
#define MPU3050_REG_DLPF 0x16
#define MPU3050_REG_TEMP 0x1B
#define MPU3050_REG_DATAXYZ 0x1D

#define MPU3050_LOWPASS_256HZ 0
#define MPU3050_LOWPASS_188HZ 1
#define MPU3050_LOWPASS_98HZ 2
#define MPU3050_LOWPASS_42HZ 3
#define MPU3050_LOWPASS_20HZ 4
#define MPU3050_LOWPASS_10HZ 5
#define MPU3050_LOWPASS_5HZ 6

#define MPU3050_RANGE_PM250 0
#define MPU3050_RANGE_PM500 1
#define MPU3050_RANGE_PM1000 2
#define MPU3050_RANGE_PM2000 3


class Gyroscope {
    public:
        Gyroscope();
        int8_t begin(uint8_t i2cAddr);
        int8_t readRaw(int16_t * x, int16_t * y, int16_t * z);
        int8_t readDegPerSecond(double * x, double * y, double * z);
        int8_t setLowPassFilterBw(uint8_t hz);
        int8_t setRange(uint8_t range);
        int8_t readRawTemp(int16_t * temp);
        int8_t readTempDegrees(double * temp);
        int8_t setOffsets(int16_t x, int16_t y, int16_t z);
    protected:
        int8_t i2cReadBytes(uint8_t reg, uint8_t * data, uint8_t len);
        int8_t i2cWriteByte(uint8_t reg, uint8_t data);
        int8_t setDLPFReg(uint8_t lowpass, uint8_t range);
        int8_t writei2c2s(uint8_t regmsb, uint8_t reglsb, int16_t data);
        uint8_t i2cAddr_;
        uint8_t range_;
        uint8_t lowpass_;
    private:
        void conv2Byte2Signed16(uint8_t LSB, uint8_t MSB, int16_t * dest);
};


#endif