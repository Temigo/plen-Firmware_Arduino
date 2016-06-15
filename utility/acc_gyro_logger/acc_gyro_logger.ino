#include <Arduino.h>

#include "System.h"
#include "AccelerationGyroSensor.h"


namespace
{
	PLEN2::AccelerationGyroSensor acc_gyro;
}


void setup()
{
	volatile PLEN2::System s;
}

void loop()
{
	acc_gyro.sampling();
	acc_gyro.dump();
	delay(500);
}
