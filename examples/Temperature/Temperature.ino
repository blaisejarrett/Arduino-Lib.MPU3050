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
}

void loop()
{
    // don't bother reading if we failed to connect
    if (fail)
        return;

    // to store the temperature of the sensor
    double temp;

    // Read the temperature in degrees
    if (gyro.readTempDegrees(&temp) != 0)
    {
        Serial.println("Failed to read gyroscope");
        return;
    }
    
    // print it out
    Serial.print("Temp: ");
    Serial.println(temp);

    delay(50);
}
