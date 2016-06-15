/*!
	@file      AccelerationGyroSensor.h
	@brief     Management class of acceleration and gyro sensor.
	@author    Kazuyuki TAKASE
	@copyright The MIT License - http://opensource.org/licenses/mit-license.php
*/

#pragma once

#ifndef PLEN2_ACCELERATION_GYRO_SENSOR_H
#define PLEN2_ACCELERATION_GYRO_SENSOR_H

namespace PLEN2
{
	class AccelerationGyroSensor;
}

/*!
	@brief Management class of acceleration and gyro sensor
*/
class PLEN2::AccelerationGyroSensor
{
private:
	enum SENSOR_VALUE_MAP {
		ACC_X,
		ACC_Y,
		ACC_Z,
		GYRO_ROLL,
		GYRO_PITCH,
		GYRO_YAW,
		SUM //!< Summation of sensors
	};

	int m_values[SUM];

public:
	/*!
		@brief Do sampling sensor values

		Usage assumption is to call the method at stated periods from loop().

		@attention
		The method has a deadlock of interruption and communication wait
		because it communicates with the sensor through a bus,
		so using it in constructor and interruption vector is deprecated.
		<br><br>
		The order of power supplied or firmware startup timing is base-board, head-board.
		If the method calls from early timing, program freezes because synchronism of communication is missed.
		(Generally, after setup() insert a 3000[msec] delay for success.)
	*/
	void sampling();

	/*!
		@brief Get acceleration on X axis

		@return acceleration on X axis

		@attention
		Return the value cached at the runtime of the sampling method.
	*/
	const int& getAccX();

	/*!
		@brief Get acceleration on Y axis

		@return acceleration on Y axis

		@attention
		Return the value cached at the runtime of the sampling method.
	*/
	const int& getAccY();

	/*!
		@brief Get acceleration on Z axis

		@return acceleration on Z axis

		@attention
		Return the value cached at the runtime of the sampling method.
	*/
	const int& getAccZ();

	/*!
		@brief Get angular velocity of roll axis (rotation axis on X axis)

		@return Angular velocity of roll axis

		@attention
		Return the value cached at the runtime of the sampling method.
	*/
	const int& getGyroRoll();

	/*!
		@brief Get pitch (rotation axis on Y axis) axis's angular velocity

		@return Pitch axis's angular velocity

		@attention
		Return the value cached at the runtime of the sampling method.
	*/
	const int& getGyroPitch();

	/*!
		@brief Get yaw (rotation axis on Z axis) axis's angular velocity

		@return Yaw axis's angular velocity

		@attention
		Return the value cached at the runtime of the sampling method.
	*/
	const int& getGyroYaw();

	/*!
		@brief Dump all sensor values after sampling

		Outputs result in JSON format as below.
		@code
		{
			"Acc X": <integer>,
			"Acc Y": <integer>,
			"Acc Z": <integer>,
			"Gyro Roll": <integer>,
			"Gyro Pitch": <integer>,
			"Gyro Yaw": <integer>
		}
		@endcode
	*/
	void dump();
};

#endif // PLEN2_ACCELERATION_GYRO_SENSOR_H
