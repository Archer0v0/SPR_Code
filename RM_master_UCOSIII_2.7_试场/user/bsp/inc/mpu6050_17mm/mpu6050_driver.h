#ifndef __MPU6050_DRIVER_H__
#define __MPU6050_DRIVER_H__

#include <stm32f4xx.h>
#include <mpu6050_i2c.h>

typedef struct __MPU6050_17mm_RAW_Data__
{
    short Accel_X;  //?????X???????
    short Accel_Y;  //?????Y???????
    short Accel_Z;  //?????Z???????
    short Temp;     //??????????
    short Gyro_X;   //?????X???????
    short Gyro_Y;   //?????Y???????
    short Gyro_Z;   //?????Z???????
}MPU6050_17mm_RAW_DATA;

typedef struct __MPU6050_17mm_REAL_Data__
{
    float Accel_X;  //??????X????,
    float Accel_Y;  //??????Y????,
    float Accel_Z;  //??????Z????,
    float Temp;     //????????,??????
    float Gyro_X;   //??????X?????,
    float Gyro_Y;   //??????Y?????,
    float Gyro_Z;   //??????Z?????
}MPU6050_17mm_REAL_DATA;

extern MPU6050_17mm_RAW_DATA    MPU6050_17mm_Raw_Data; 
extern MPU6050_17mm_REAL_DATA   MPU6050_17mm_Real_Data;

int MPU6050_Initialization(void);
int MPU6050_17mm_ReadData(void);
void MPU6050_Gyro_calibration(void);

#endif
