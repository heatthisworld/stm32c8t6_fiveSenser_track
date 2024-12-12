#include "Track.h"

float Kp = 50.0f;
float Ki = 0.0f;
float Kd = 1.0f;
int16_t baseSpeed = 300; // 基础车速
float steerStep = 1.0f;  // 作举调整步长
float lastError = 0.0f;
float integral = 0.0f;

void Track_Init(void)
{
    Motor_Init();
    Servo_Init();
    mySensor_Init();
}

void Track_Run(void)
{
    uint8_t sensorValue = mySensor_Read();
    float error = 0.0f;
    static int noLineCounter = 0;
    float steerAngle;

    // 根据传感器值计算误差
    switch (sensorValue)
    {
    case 0: // 无黑线身
        noLineCounter++;
        if (noLineCounter < 10)
        {
            error = lastError; // 继续保持原路径
        }
        else
        {
            Motor_LeftSetSpeed(0);
            Motor_RightSetSpeed(0);
            return;
        }
        break;
    case 1:
        error = 2.0f;
        break;
    case 2:
        error = 1.0f;
        break;
    case 4:
        error = 0.0f;
        break;
    case 8:
        error = -1.0f;
        break;
    case 16:
        error = -2.0f;
        break;
    default: // 多个传感器意义同时跨黑线
        error = 0.0f;
        break;
    }

    noLineCounter = 0; // 重置无黑线计数

    // PID 计算
    integral += error;
    float derivative = error - lastError;
    float pidOutput = Kp * error + Ki * integral + Kd * derivative;

    // 调整旋转角
    steerAngle = 50.0f + pidOutput * steerStep;
    if (steerAngle < 10.0f)
        steerAngle = 10.0f;
    if (steerAngle > 90.0f)
        steerAngle = 90.0f;

    Servo_SetAngle(steerAngle);

    // 调整车速
    int16_t leftSpeed = baseSpeed + pidOutput;
    int16_t rightSpeed = baseSpeed - pidOutput;

    if (leftSpeed < 300)
        leftSpeed = 300;
    if (leftSpeed > 1000)
        leftSpeed = 1000;
    if (rightSpeed < 300)
        rightSpeed = 300;
    if (rightSpeed > 1000)
        rightSpeed = 1000;

    Motor_LeftSetSpeed(leftSpeed);
    Motor_RightSetSpeed(rightSpeed);

    lastError = error; // 更新上一次误差
}
