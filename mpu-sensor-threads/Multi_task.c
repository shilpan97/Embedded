#include "mpu9250.h"
#include "led.h"
#include "sensor_fusion.h"
#include <rtl.h>

OS_TID task_1, task_2, task_3;
OS_SEM semaphore1;
float pitch_;
float yaw_;
float roll_;

__task void read_values(void);
__task void compute_values(void);
__task void send_values (void);


__task void read_values(void){
	OS_RESULT ret; 
	
	while(1){
		//Pass control to other 3 tasks for 3 OS ticks 
		os_dly_wait(3);
		// Wait for the free semaphore (no time limit)
		ret = os_sem_wait (semaphore1, 0xFFFF);
		if(ret !=OS_R_TMO){
		//If there was no time-out then the semaphore was acquired
    //Read all values from sensor
		MPU9250_read_gyro();
		MPU9250_read_acc();
		MPU9250_read_mag();
		//Return token back to semaphore
		os_sem_send (semaphore1);
		}
	}
	
}

__task void compute_values(void){
	OS_RESULT ret; 
	while(1){
		os_dly_wait(3);
		ret = os_sem_wait (semaphore1, 0xFFFF);

    if(ret !=OS_R_TMO){
     //Update sensor value array
    sensor_fusion_update(MPU9250_gyro_data[0],MPU9250_gyro_data[1],MPU9250_gyro_data[2],MPU9250_accel_data[0],
			MPU9250_accel_data[1],MPU9250_accel_data[2],MPU9250_mag_data[0],
			MPU9250_mag_data[1], MPU9250_mag_data[2]);
    //calculate pitch, yaw and roll from recent values
    pitch_ = sensor_fusion_getPitch();
    yaw_  = sensor_fusion_getYaw();
    roll_ = sensor_fusion_getRoll();
    os_sem_send (semaphore1);
		}
	}
}

__task void send_values (void){
	OS_RESULT ret; 
  while(1){
    os_dly_wait(3);
  	ret = os_sem_wait (semaphore1, 0xFFFF);
    if(ret !=OS_R_TMO){
    printf("%f, %f, %f \n", pitch_, yaw_, roll_);
    os_sem_send (semaphore1);
}
		//os_dly_wait(3);


	}
}

__task void init(void){
  //Create program semaphore
  os_sem_init (semaphore1, 1);
  //Create all three tasks
	task_1 = os_tsk_create( read_values, 1);
	task_2 = os_tsk_create( compute_values, 1);
	task_3 = os_tsk_create( send_values, 1);
  //Delete this init thread. Is not required anymore
	os_tsk_delete_self ();

}

int main(){
  //Initialize sensor
  printf("test");
	MPU9250_init(1,1);
  //Initialize LED. Used for debugging and quick check
	LED_setup();
	sensor_fusion_init();
	sensor_fusion_begin(30);
	LED_display(MPU9250_whoami());
  //Initialize init task
	os_sys_init (init);
	
	return 0;
}
