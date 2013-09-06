#include <Wire.h>
#include "MPU3050lib.h"


Gyroscope gyro;
bool fail;

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
        fail = true;
        return;
    }

    // set the range
    // 
    // Range
    // +-250 deg/s    MPU3050_RANGE_PM250
    // +-500 deg/s    MPU3050_RANGE_PM500
    // +-1000 deg/s   MPU3050_RANGE_PM1000
    // +-2000 deg/s   MPU3050_RANGE_PM2000
    gyro.setRange(MPU3050_RANGE_PM2000);
}

void loop()
{
    // don't bother reading if we failed to connect
    if (fail)
        return;

    // 3 doubles to store the data
    double x, y, z;

    // Read degrees per second
    // Note: You can also read the raw data by calling
    //       readRaw() - readRaw takes int16_t instead of doubles
    if (gyro.readDegPerSecond(&x, &y, &z) != 0)
    {
        Serial.println("Failed to read gyroscope");
        return;
    }
    
    // print them out
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.print(" Z: ");
    Serial.println(z);

    delay(50);
}
