#include <Wire.h>
#include "MPU3050lib.h"
#include <math.h>

#define SAMPLES 300

Gyroscope gyro;

void setup()
{
    Serial.begin(57600);
    
    // set the I2C address of the gyroscope
    //
    // With the OSEPP Accelerometer use:
    //      OSEPP_GYRO_SW_ON    - Switch is in the ON position
    //      OSEPP_GYRO_SW_OFF   - Switch is in the OFF position
    //
    // begin will return 0 on success
    if (gyro.begin(OSEPP_GYRO_SW_ON) != 0)
    {
        Serial.println("Error connecting to gyroscope");
        return;
    }

    // set the range
    // 
    // Range
    // +-250 deg/s    MPU3050_RANGE_PM250
    // +-500 deg/s    MPU3050_RANGE_PM500
    // +-1000 deg/s   MPU3050_RANGE_PM1000
    // +-2000 deg/s   MPU3050_RANGE_PM2000
    gyro.setRange(MPU3050_RANGE_PM250);

    //gyro.setOffsets(0, 0, 0);
    gyro.setOffsets(-85, 26, 16);


    double avgX = 0;
    double avgY = 0;
    double avgZ = 0;

    int16_t x, y, z;

    for (uint16_t i = 0; i < SAMPLES; ++i)
    {
        if (gyro.readRaw(&x, &y, &z) != 0)
        {
            Serial.println("Failed to read gyroscope");
            return;
        }

        avgX += (x - avgX) / (double)(i + 1);
        avgY += (y - avgY) / (double)(i + 1);
        avgZ += (z - avgZ) / (double)(i + 1);

        delay(10);
    }

    avgX /= 4;
    avgY /= 4;
    avgZ /= 4;

    Serial.println(round(avgX) * -1);
    Serial.println(round(avgY) * -1);
    Serial.println(round(avgZ) * -1);
}

void loop() {}
