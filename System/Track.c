#include "Track.h"

float Kp = 12.0f;
float Ki = 0.01f;
float Kd = 2.0f;
int16_t baseSpeed = 350; // 基础车速
float steerStep = 0.8f;  // 作举调整步长
float lastError = 0.0f;
float integral = 0.1f;
const float integralLimit = 50.0f; // 积分限制

void Track_Init(void)
{
    Motor_Init();
    Servo_Init();
    mySensor_Init();
}

int16_t constrain(int16_t value, int16_t min, int16_t max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
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
            Motor_LeftSetSpeed(300);
            Motor_RightSetSpeed(300);
            return;
        }
        break;
    case 1:
        error = 6.0f;
        break;
    case 2:
        error = 2.3f;
        break;
    case 4:
        error = 0;

        break;
    case 8:
        error = -2.3f;
        break;
    case 16:
        error = -6.0f;
        break;
    default: // 多个传感器意义同时跨黑线
        error = 0.0f;
        break;
    }
    // if (error != 0.0f)
    // {
    //     OLED_ShowString(1, 1, "err:");
    //     OLED_ShowSignedNum(1, 5, error * 10, 3);
    // }
    noLineCounter = 0; // 重置无黑线计数

    // PID 计算
    integral += error;
    integral = constrain(integral, -integralLimit, integralLimit); // 添加积分限制
    // OLED_ShowString(2, 1, "int:");
    // OLED_ShowSignedNum(2, 5, integral, 4);
    float derivative = error - lastError;
    float pidOutput = Kp * error + Ki * integral + Kd * derivative;
    // OLED_ShowString(3, 1, "out:");
    // OLED_ShowSignedNum(3, 5, pidOutput * 10, 4);
    // 调整旋转角
    steerAngle = 50.0f + pidOutput * steerStep;
    if (steerAngle < 10.0f)
        steerAngle = 10.0f;
    if (steerAngle > 90.0f)
        steerAngle = 90.0f;

    Servo_SetAngle(steerAngle);
    // OLED_ShowString(4, 1, "steer:");
    // OLED_ShowNum(4, 7, steerAngle, 2);

    // // 调整车速
    int16_t leftSpeed = baseSpeed + pidOutput;
    int16_t rightSpeed = baseSpeed - pidOutput;
    // int16_t leftSpeed = baseSpeed;
    // int16_t rightSpeed = baseSpeed;

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
