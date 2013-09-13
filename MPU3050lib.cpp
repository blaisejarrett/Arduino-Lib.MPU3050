// Code by Blaise Jarrett @ OSEPP http://blaisejarrett.com
// Released to the public domain! Enjoy!

#include "MPU3050lib.h"
#include <stdio.h>
#include <math.h>
#include <Wire.h>

#define WIRE Wire

#if (ARDUINO >= 100)
 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#else
 #include <WProgram.h>
#endif

Gyroscope::Gyroscope() : i2cAddr_(0) {}

int8_t Gyroscope::begin(uint8_t i2cAddr)
{
    uint8_t id;
    i2cAddr_ = i2cAddr;

    // Join the I2C bus as master
    WIRE.begin();

    // perform a simple check to see if we're talking to the right device
    // read the who am I register
    if (i2cReadBytes(MPU3050_REG_WHOAMI, &id, 1) != 0)
        return MPU3050_ERROR_I2CREAD;

    // compare the who am I register
    if ((id & 0x7E) != 0x68)
        return MPU3050_ERROR_WRONG_ID;

    // set defaults
    uint8_t ret = setDLPFReg(MPU3050_LOWPASS_188HZ, MPU3050_RANGE_PM250);
    if (ret != 0)
        return ret;

    return 0;
}

int8_t Gyroscope::readRaw(int16_t * x, int16_t * y, int16_t * z)
{
    if (i2cAddr_ == 0)
        return MPU3050_ERROR_NOT_INITIALIZED;

    uint8_t data[6];

    if (i2cReadBytes(MPU3050_REG_DATAXYZ, data, 6) != 0)
        return MPU3050_ERROR_I2CREAD;

    conv2Byte2Signed16(data[1], data[0], x);
    conv2Byte2Signed16(data[3], data[2], y);
    conv2Byte2Signed16(data[5], data[4], z);

    return 0;
}

int8_t Gyroscope::readDegPerSecond(double * x, double * y, double * z)
{
    int16_t x_r, y_r, z_r;

    int8_t ret = readRaw(&x_r, &y_r, &z_r);
    // pass the error
    if (ret != 0)
        return ret;

    // perform the float conversion
    double divisor;

    switch (range_)
    {
        case MPU3050_RANGE_PM250:
            divisor = 131;
            break;
        case MPU3050_RANGE_PM500:
            divisor = 65.5;
            break;
        case MPU3050_RANGE_PM1000:
            divisor = 32.8;
            break;
        case MPU3050_RANGE_PM2000:
            divisor = 16.4;
            break;

        default:
            return MPU3050_ERROR_RANGEOUTOFRANGE;
    }

    *x = (double)x_r / divisor;
    *y = (double)y_r / divisor;
    *z = (double)z_r / divisor;

    return 0;
}

int8_t Gyroscope::readRawTemp(int16_t * temp)
{
    uint8_t data[2];

    if (i2cReadBytes(MPU3050_REG_TEMP, data, 2) != 0)
        return MPU3050_ERROR_I2CREAD;

    conv2Byte2Signed16(data[1], data[0], temp);

    return 0;
}

int8_t Gyroscope::readTempDegrees(double * temp)
{
    int16_t temp_r;

    int8_t ret = readRawTemp(&temp_r);
    // pass the error
    if (ret != 0)
        return ret;

    int16_t tmp;
    // convert it to degrees
    // subtract room temperature offset
    tmp = temp_r + 13200;
    // convert to degrees
    *temp = (double)tmp / 280;
    // add room offset
    *temp = *temp + 35;

    return 0;
}

int8_t Gyroscope::setLowPassFilterBw(uint8_t hz)
{
    return setDLPFReg(hz, range_);
}

int8_t Gyroscope::setRange(uint8_t range)
{
    return setDLPFReg(lowpass_, range);
}

int8_t Gyroscope::setDLPFReg(uint8_t lowpass, uint8_t range)
{
    uint8_t byte, tmp;

    if (lowpass > 6)
        return MPU3050_ERROR_LOWPASSOUTOFRANGE;

    if (range > 3)
        return MPU3050_ERROR_RANGEOUTOFRANGE;

    lowpass_ = lowpass;
    range_ = range;

    byte = lowpass_;
    tmp = range_ << 3;

    byte = byte | tmp;
    byte = byte & 0x1F;

    if (i2cWriteByte(MPU3050_REG_DLPF, byte) != 0)
        return MPU3050_ERROR_I2CWRITE;

    // make a reading to apply
    int16_t x, y, z;
    readRaw(&x, &y, &z);

    return 0;
} 

int8_t Gyroscope::setOffsets(int16_t x, int16_t y, int16_t z)
{
    int8_t ret;

    ret = writei2c2s(MPU3050_REG_XOFFH, MPU3050_REG_XOFFL, x);

    if (ret != 0)
        return ret;

    ret = writei2c2s(MPU3050_REG_YOFFH, MPU3050_REG_YOFFL, y);

    if (ret != 0)
        return ret;

    ret = writei2c2s(MPU3050_REG_ZOFFH, MPU3050_REG_ZOFFL, z);

    if (ret != 0)
        return ret;

    return 0;
}

int8_t Gyroscope::writei2c2s(uint8_t regmsb, uint8_t reglsb, int16_t data)
{
    uint8_t msb_data, lsb_data;

    uint16_t tmp;
    tmp = (uint16_t)data;
    tmp &= 0x00FF;

    lsb_data = tmp;

    tmp = (uint16_t)data;
    tmp = tmp >> 8;
    tmp &= 0x00FF;

    msb_data = tmp;

    int8_t ret;

    ret = i2cWriteByte(regmsb, msb_data);
    if (ret != 0)
        return ret;

    ret = i2cWriteByte(reglsb, lsb_data);
    if (ret != 0)
        return ret;

    return 0;
}

void Gyroscope::conv2Byte2Signed16(uint8_t LSB, uint8_t MSB, int16_t * dest)
{
    *dest = 0;

    *dest = (int16_t)LSB;

    *dest |= ((int16_t)MSB << 8);
}

int8_t Gyroscope::i2cReadBytes(uint8_t reg, uint8_t *data, uint8_t len)
{
    if (i2cAddr_ == 0)
        return MPU3050_ERROR_NOT_INITIALIZED;

    WIRE.beginTransmission(i2cAddr_);
    // request a read from this address
    WIRE.write(reg);
    // end to allow the response
    WIRE.endTransmission();

    // Read a byte from the device
    WIRE.requestFrom(i2cAddr_, len); 

    uint8_t i = 0;

    while (WIRE.available())
    {
        data[i] = WIRE.read();
        ++i;

        if (i == len)
            break;
    }

    if (i != len)
        return -1;

    return 0;
}

int8_t Gyroscope::i2cWriteByte(uint8_t reg, uint8_t data)
{
    if (i2cAddr_ == 0)
        return MPU3050_ERROR_NOT_INITIALIZED;

    // Begin the write sequence 
    WIRE.beginTransmission(i2cAddr_);

    // First byte is to set the register pointer
    WIRE.write(reg);

    // Write the data byte
    WIRE.write(data);

    // End the write sequence; bytes are actually transmitted now 
    WIRE.endTransmission();

    return 0;
}

