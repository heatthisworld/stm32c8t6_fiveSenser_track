#ifndef __ATTITUDE_H
#define __ATTITUDE_H
#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"
#include "Delay.h"
#define int16 int16_t 
#define uint8 uint8_t 
typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
} icm_param_t;


typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quater_param_t;


typedef struct {
    float pitch;    //¸©Ñö½Ç
    float roll;     //·­¹ö½Ç
    float yaw;       //Æ«º½½Ç
} euler_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

extern euler_param_t eulerAngle;
extern icm_param_t icm_data;

extern int16_t AX, AY, AZ, GX, GY, GZ;			
void gyroOffset_init(void);//È¥ÁãÆ¯£¬³õÊ¼»¯

float fast_sqrt(float x);

void ICM_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);

void ICM_getValues(void);

void ICM_getEulerianAngles(void);

#endif
