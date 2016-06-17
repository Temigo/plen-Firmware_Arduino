/*
	Copyright (c) 2015,
	- Kazuyuki TAKASE - https://github.com/Guvalif
	- PLEN Project Company Inc. - https://plen.jp

	This software is released under the MIT License.
	(See also : http://opensource.org/licenses/mit-license.php)
*/

#define DEBUG false

#include "Arduino.h"

#include "Pin.h"
#include "System.h"
#include "AccelerationGyroSensor.h"

#if DEBUG
	#include "Profiler.h"
#endif

/*
 * FIXME : set the full scale sensitivity manually
 */
#define MU6050_ACC_SENSITIVITY 16384
#define MU6050_GYRO_SENSITIVITY 131

namespace
{
	template<typename T>
	void endian_cast(T& value)
	{
		char  temp;
		char* filler = reinterpret_cast<char*>(&value);

		for (int index = 0; index < (sizeof(T) / 2); index++)
		{
			temp = filler[sizeof(T) - 1 - index];
			filler[sizeof(T) - 1 - index] = filler[index];
			filler[index] = temp;
		}
	}

	template<>
	void endian_cast(int& value)
	{
		value = ((value & 0x00FF) << 8) | ((value >> 8) & 0x00FF);
	}
}

void PLEN2::AccelerationGyroSensor::sampling()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::sampling()"));
	#endif

	/*!
		@note
		Firstly, occupy the right of sending data (data flow is "base-board -> head-board") by substituting HIGH for Pin::RS485_TXD().
		If sending any data to head-board, the sensor responds values formatting 2byte, big-endian.

		Just after sending any data, must give up the right of sending data (data flow is "head-board -> base-board")
		by substituting LOW for Pin::RS485_TXD(), for receiving the values.
	*/  
        digitalWrite(Pin::RS485_TXD(), HIGH);

        System::BLESerial().write('<');

        digitalWrite(Pin::RS485_TXD(), LOW);

        char  read_count = 0;
        char* filler = reinterpret_cast<char*>(m_values);

        while (read_count < SUM * sizeof(int)) {
          filler[read_count++] = System::BLESerial().read();
        }

        // @attention For skipping to read '\n'.
        System::BLESerial().read();

        for (int index = 0; index < SUM; index++)
        {
          endian_cast(m_values[index]);
        }

        // From raw values to real values of accelerometer/gyroscope
        /*! 
         * @note
         * The accelerometer and gyroscope measurements are explained in the MPU-6050 datasheet 
         * in the GYRO_CONFIG and ACCEL_CONFIG register descriptions (sections 4.4 and 4.5 on pages 14 and 15). 
         * 
         * The scale of each depends on the sensitivity settings chosen, which can be one of +/- 2, 4, 8, or 16g 
         * for the accelerometer and one of +/- 250, 500, 1000, or 2000 deg/sec for the gyroscope.
         * The accelerometer produces data in units of acceleration (distance over time2), and the gyroscope 
         * produces data in units of rotational velocity (rotation distance over time).
         * 
         * The output scale for any setting is [-32768, +32767] for each of the six axes. 
         * The default setting in the I2Cdevlib class is +/- 2g for the accel and +/- 250 deg/sec for the gyro. 
         * If the device is perfectly level and not moving, then:
         *     X/Y accel axes should read 0
         *     Z accel axis should read 1g, which is +16384 at a sensitivity of 2g
         *     X/Y/Z gyro axes should read 0
         *     
         *  In reality, the accel axes won't read exactly 0 since it is difficult to be perfectly level 
         *  and there is some noise/error, and the gyros will also not read exactly 0 for the same reason (noise/error).
         */
         
        m_values[ACC_X] = (int) m_values[ACC_X] / MU6050_ACC_SENSITIVITY;
        m_values[ACC_Y] = (int) m_values[ACC_Y] / MU6050_ACC_SENSITIVITY;
        m_values[ACC_Z] = (int) m_values[ACC_Z] / MU6050_ACC_SENSITIVITY;
        m_values[GYRO_ROLL] = (int) m_values[GYRO_ROLL] / MU6050_GYRO_SENSITIVITY;
        m_values[GYRO_PITCH] = (int) m_values[GYRO_PITCH] / MU6050_GYRO_SENSITIVITY;
        m_values[GYRO_YAW] = (int) m_values[GYRO_YAW] / MU6050_GYRO_SENSITIVITY;

		delay(1); // @attention A countermeasure of optimization.
	}



const int& PLEN2::AccelerationGyroSensor::getAccX()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getAccX()"));
	#endif

	return m_values[ACC_X];
}

const int& PLEN2::AccelerationGyroSensor::getAccY()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getAccY()"));
	#endif

	return m_values[ACC_Y];
}

const int& PLEN2::AccelerationGyroSensor::getAccZ()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getAccZ()"));
	#endif

	return m_values[ACC_Z];
}

const int& PLEN2::AccelerationGyroSensor::getGyroRoll()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getGyroRoll()"));
	#endif

	return m_values[GYRO_ROLL];
}

const int& PLEN2::AccelerationGyroSensor::getGyroPitch()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getGyroPitch()"));
	#endif

	return m_values[GYRO_PITCH];
}

const int& PLEN2::AccelerationGyroSensor::getGyroYaw()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getGyroYaw()"));
	#endif

	return m_values[GYRO_YAW];
}

void PLEN2::AccelerationGyroSensor::dump()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::dump()"));
	#endif

	sampling();

	System::outputSerial().println(F("{"));

	System::outputSerial().print(F("\t\"Acc X\": "));
	System::outputSerial().print(getAccX());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Acc Y\": "));
	System::outputSerial().print(getAccY());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Acc Z\": "));
	System::outputSerial().print(getAccZ());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Gyro Roll\": "));
	System::outputSerial().print(getGyroRoll());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Gyro Pitch\": "));
	System::outputSerial().print(getGyroPitch());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Gyro Yaw\": "));
	System::outputSerial().println(getGyroYaw());

	System::outputSerial().println(F("}"));
}
