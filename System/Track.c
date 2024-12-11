#include "track.h"
#include "Motor.h"
#include "Servo.h"
#include "mySensor.h"
#include "attitude_solution.h" // 引入偏航角计算相关头文件
// PID 参数
static float Kp = 10.0f; // 比例系数
static float Ki = 0.0f;  // 积分系数
static float Kd = 1.0f;  // 微分系数

// PID 控制变量
static float previousError = 0.0f;
static float integral = 0.0f;

// 电机速度范围
static const int16_t minSpeed = 300;  // 电机最小速度
static const int16_t maxSpeed = 1000; // 电机最大速度

// 舵机角度范围
static const float servoCenter = 50.0f; // 舵机中间角度
static const float servoRange = 40.0f;  // 舵机最大偏转范围

// 传感器权值数组
static const int8_t sensorWeights[5] = {-2, -1, 0, 1, 2};

// 初始化函数
void LineTracking_Init(void)
{
    Motor_LeftSetSpeed(minSpeed);
    Motor_RightSetSpeed(minSpeed);
}

// 读取传感器值并计算误差
float CalculateSensorError(uint8_t sensorData)
{
    int8_t weightedSum = 0;
    int8_t activeSensors = 0;

    for (int i = 0; i < 5; i++)
    {
        if (!(sensorData & (1 << i))) // 检测第 i 位是否为低电平（在黑线上）
        {
            weightedSum += sensorWeights[i];
            activeSensors++;
        }
    }

    // 如果没有检测到黑线，返回一个特殊值（如 -999）表示失控状态
    if (activeSensors == 0)
        return -999;

    return (float)weightedSum / activeSensors;
}

// 黑线跟踪主控制函数
void LineTracking_Update(void)
{
    // 读取传感器数据
    uint8_t sensorData = mySensor_Read();

    // 计算传感器误差
    float sensorError = CalculateSensorError(sensorData);

    float error = 0.0f;

    // 如果传感器没有检测到黑线，使用偏航角作为补偿
    if (sensorError == -999)
    {
        error = -eulerAngle.yaw; // 偏航角为负值表示左偏，为正值表示右偏
    }
    else
    {
        error = sensorError; // 正常情况下使用传感器误差
    }

    // PID 计算
    integral += error;                        // 积分累加
    float derivative = error - previousError; // 微分
    float correction = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;

    // 计算舵机角度
    float servoAngle = servoCenter + correction;

    // 限幅处理（舵机范围：50 ± 40）
    if (servoAngle > servoCenter + servoRange)
        servoAngle = servoCenter + servoRange;
    if (servoAngle < servoCenter - servoRange)
        servoAngle = servoCenter - servoRange;

    // 设置舵机角度
    Servo_SetAngle(servoAngle);

    // 电机速度调整（保持恒定速度）
    int16_t motorSpeed = (maxSpeed + minSpeed) / 2;

    // 设置电机速度
    Motor_LeftSetSpeed(motorSpeed);
    Motor_RightSetSpeed(motorSpeed);
}
