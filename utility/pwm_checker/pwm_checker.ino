#include <Arduino.h>
#include <EEPROM.h>

#include "System.h"
#include "JointController.h"
#include "AccelerationGyroSensor.h"



namespace
{
	PLEN2::JointController joint_ctrl;
  PLEN2::AccelerationGyroSensor acc_gyro;
  
  double delta_t = 0.5;
  unsigned long t = 0;

  int roll_angle = 0;
  int pitch_angle = 0;
  int yaw_angle = 0;

  int roll;
  int pitch;
  int yaw;
}


void setup()
{
	volatile PLEN2::System s;

	joint_ctrl.loadSettings();
  
}

void loop()
{
	using namespace PLEN2;

    acc_gyro.sampling();

    roll = acc_gyro.getGyroRoll();
    pitch = acc_gyro.getGyroPitch();
    yaw = acc_gyro.getGyroYaw();

    // Time interval
    delta_t = (double) (millis() - t) / 1000;
    t = millis();
    
    /*
     * Compensate the drift of gyrometer with accelerometer values
     * Use a Complementary filter (Kalman filter would also be possible but more complex)
     */
    joint_ctrl.setAngleDiff(0, - (int) (0.98*pitch*delta_t + 0.02*acc_gyro.getAccY()));
    pitch_angle = pitch_angle + (int) (0.98*pitch*delta_t + 0.02*acc_gyro.getAccY());

    /*
    joint_ctrl.setAngleDiff(2, - (int) (0.98*roll*delta_t + 0.02*acc_gyro.getAccX()));
    roll_angle = roll_angle + (int) (0.98*roll*delta_t + 0.02*acc_gyro.getAccX());

    joint_ctrl.setAngleDiff(0, - (int) (0.98*yaw*delta_t + 0.02*acc_gyro.getAccZ()));
    yaw_angle = yaw_angle + (int) (0.98*yaw*delta_t + 0.02*acc_gyro.getAccZ());        
    */
    
		System::USBSerial().print(F("Pitch = "));
		System::USBSerial().print(p);
    System::USBSerial().print(F(" Delta_t = "));
    System::USBSerial().print(delta_t);   
    System::USBSerial().print(F(" Angle diff = "));
    System::USBSerial().print(p*delta_t);
    System::USBSerial().println(',');
    
   delay(500);
}
