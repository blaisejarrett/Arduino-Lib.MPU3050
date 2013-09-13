/**
 * @file
 * @author  Blaise Jarrett <me@blaisejarrett.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Released to the public domain! Enjoy!
 *
 * @section DESCRIPTION
 *
 * The Gyroscope header for the MPU3050 Gyroscope class.
 */

#ifndef _MPU3050LIB_H_
#define _MPU3050LIB_H_

#include <stdint.h>

/** 
 * The I2C address of the OSEPP Gyroscope sensor
 * when the physical switch is in the ON position.
 */
#define OSEPP_GYRO_SW_ON 0x68
/** 
 * The I2C address of the OSEPP Gyroscope sensor
 * when the physical switch is in the OFF position.
 */
#define OSEPP_GYRO_SW_OFF 0x69
#define MPU3050_I2CADD 0x1E

/** Forgot to call begin() */
#define MPU3050_ERROR_NOT_INITIALIZED -2
/** Device did not respond with the correct ID */
#define MPU3050_ERROR_WRONG_ID -3
/** I2C read error */
#define MPU3050_ERROR_I2CREAD -4
/** I2C write error */
#define MPU3050_ERROR_I2CWRITE -5
/** Lowpass argument must be one of the definitions */
#define MPU3050_ERROR_LOWPASSOUTOFRANGE -6
/** Range argument must be one of the definitions */
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

/** Lowpass at 256hz */
#define MPU3050_LOWPASS_256HZ 0
/** Lowpass at 188hz */
#define MPU3050_LOWPASS_188HZ 1
/** Lowpass at 98hz */
#define MPU3050_LOWPASS_98HZ 2
/** Lowpass at 42hz */
#define MPU3050_LOWPASS_42HZ 3
/** Lowpass at 20hz */
#define MPU3050_LOWPASS_20HZ 4
/** Lowpass at 10hz */
#define MPU3050_LOWPASS_10HZ 5
/** Lowpass at 5hz */
#define MPU3050_LOWPASS_5HZ 6

/** Range: +-250 deg/s */
#define MPU3050_RANGE_PM250 0
/** Range: +-500 deg/s */
#define MPU3050_RANGE_PM500 1
/** Range: +-1000 deg/s */
#define MPU3050_RANGE_PM1000 2
/** Range: +-2000 deg/s */
#define MPU3050_RANGE_PM2000 3

/**
 * Represents a MPU3050 bassed Gyroscope sensor.
 * You must call begin before calling any other methods.
 */
class Gyroscope {
    public:
        Gyroscope();

        /**
         * Attenpts to connect to the device. You must call begin before calling any other method.
         *
         * @param i2cAddr Takes the I2C address of the Gyroscope as an argument. 
         *                Use #OSEPP_GYRO_SW_ON if your address switch is in the ON position. 
         *                Use #OSEPP_GYRO_SW_OFF if your address switch is in the OFF position.
         *
         * @return Returns 0 if it successfully connects to the Gyroscope sensor. 
         *         Returns any other non-zero number if it fails.
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t begin(uint8_t i2cAddr);

        /**
         * Reads the RAW data from the accelerometer as a signed 16 bit integer.
         * This integer does not represent degrees per second. You'll probably 
         * want to use readDegPerSecond instead.
         *
         * @param x A pointer to the variable that will get the x raw data recorded to.
         * @param y A pointer to the variable that will get the y raw data recorded to.
         * @param z A pointer to the variable that will get the z raw data recorded to.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t readRaw(int16_t * x, int16_t * y, int16_t * z);

        /**
         * Reads the degrees per second of angular momentum experienced by the Gyroscope.
         *
         * @param x A pointer to the variable that will get the x data recorded to (in degrees/second).
         * @param y A pointer to the variable that will get the y data recorded to (in degrees/second).
         * @param z A pointer to the variable that will get the z data recorded to (in degrees/second).
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t readDegPerSecond(double * x, double * y, double * z);

        /**
         * The MPU3050 has a configurable low-pass filter. You can use this 
         * to try and filter out high frequencies from the gyroscope readings.
         *
         * @param hz The Lowpass filter definition. Check MPU3050lib.h for possible arugments.
         *           Does not represent HZ directly. Use the definitions.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t setLowPassFilterBw(uint8_t hz);

        /**
         * The MPU3050 has an adjustable range to help you fine tune the sensitivity.
         * Internally calls setDLPFReg. 
         *
         * @param range The range definition. Check MPU3050lib.h for possible arugments.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t setRange(uint8_t range);

        /**
         * Reads the raw signed 16bit temperature data right from the MPU3050s registers. 
         * Does not directly represent degrees. Use readTempDegrees.
         * Internally calls setDLPFReg.
         *
         * @param temp A pointer to the variable that will get the raw data recorded to.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t readRawTemp(int16_t * temp);

        /**
         * Reads the sensors internal temperature sensor in degrees.
         *
         * @param temp A pointer to the variable that will get the data recorded to (in degrees).
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t readTempDegrees(double * temp);

        /**
         * Sets the calibration offset registers on the MPU3050. These
         * offsets are subtracted from the raw data values. A range of +-1000 is assumed.
         * Refer to the calibration example.
         *
         * @param x The x offset value to be set in the MPU3050 registers.
         * @param y The y offset value to be set in the MPU3050 registers.
         * @param z The z offset value to be set in the MPU3050 registers.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t setOffsets(int16_t x, int16_t y, int16_t z);

    protected:
        /**
         * I2C interface function for reading bytes. Uses the Arduino Wire library.
         *
         * @param reg The first register to read from.
         * @param data The array where the data will be read into.
         * @param len The number of bytes to read.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t i2cReadBytes(uint8_t reg, uint8_t * data, uint8_t len);

        /**
         * I2C interface function for writing a single byte. Uses the Arduino Wire library.
         *
         * @param reg The register address to write to.
         * @param data The byte to write.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t i2cWriteByte(uint8_t reg, uint8_t data);

        /**
         * Modifies the DLPF register of the MPU3050.
         *
         * @param lowpass The lowpass definition.
         * @param range The range definition.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t setDLPFReg(uint8_t lowpass, uint8_t range);

        /**
         * Writes a 16bit 2s compliment signed value to two
         * byte registers.
         *
         * @param regmsb The I2C address of the MSB register.
         * @param reglsb The I2C address of the LSB register.
         * @param data The value to be written.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        int8_t writei2c2s(uint8_t regmsb, uint8_t reglsb, int16_t data);

        /** The I2C address of the device. */
        uint8_t i2cAddr_;
        /** The range definition enum of the device. */
        uint8_t range_;
        /** The lowpass definition enum of the device. */
        uint8_t lowpass_;

    private:
        /**
         * Combines two bytes into a 16 bit 2s compliment number.
         *
         * @param LSB The LSB 2s compliment data.
         * @param MSB The MSB 2s compliment data.
         * @param dest The signed 16 bit number result.
         *
         * @return Returns 0 on success.
         *         Returns any other non-zero number if it fails. 
         *         Check MPU3050lib.h for the various ERROR definitions.
         */
        void conv2Byte2Signed16(uint8_t LSB, uint8_t MSB, int16_t * dest);
};


#endif