#include "Track.h"
#define MAX_STEER_ANGLE 40.0f
#define MAX_SPEED 1000
#define MIN_SPEED 300
#define integralLimit 50.0f

float Kp = 12.0f;
float Ki = 0.01f;
float Kd = 2.0f;
int16_t baseSpeed = 350; // 基础车速
float steerStep = 0.8f;  // 作举调整步长
float lastError = 0.0f;
float integral = 0.1f;
float targetYaw = 0.0f;

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
    float currentYaw = eulerAngle.yaw;
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
        targetYaw += 45.0f;
        break;
    case 2:
        targetYaw += 15.0f;
        break;
    case 4:
        targetYaw += 0;

        break;
    case 8:
        targetYaw += -15.0f;
        break;
    case 16:
        targetYaw += -45.0f;
        break;
    default: // 多个传感器意义同时跨黑线
        targetYaw += 0.0f;
        break;
    }
    Delay;

    float errorYaw = targetYaw - currentYaw;
    // 确保误差在 -180 ~ +180 之间
    if (error > 180.0f)
        error -= 360.0f;
    if (error < -180.0f)
        error += 360.0f;

    // if (error != 0.0f)
    // {
    //     OLED_ShowString(1, 1, "err:");
    //     OLED_ShowSignedNum(1, 5, error * 10, 3);
    // }

    noLineCounter = 0; // 重置无黑线计数

    // PID 计算
    integral += error;
    integral = constrain(integral, -integralLimit, integralLimit); // 添加积分限制
    float derivative = error - lastError;
    float pidOutput = Kp * error + Ki * integral + Kd * derivative;
    lastError = error; // 更新上一次误差

    // 调整旋转角
    steerAngle = 50.0f + pidOutput * steerStep;
    if (steerAngle < 10.0f)
        steerAngle = 10.0f;
    if (steerAngle > 90.0f)
        steerAngle = 90.0f;

    Servo_SetAngle(steerAngle);

    // // 调整车速
    int16_t leftSpeed = baseSpeed + pidOutput;
    int16_t rightSpeed = baseSpeed - pidOutput;
    // int16_t leftSpeed = baseSpeed;
    // int16_t rightSpeed = baseSpeed;

    if (leftSpeed < MIN_SPEED)
        leftSpeed = MIN_SPEED;
    if (leftSpeed > MAX_SPEED)
        leftSpeed = MAX_SPEED;
    if (rightSpeed < MIN_SPEED)
        rightSpeed = MIN_SPEED;
    if (rightSpeed > MAX_SPEED)
        rightSpeed = MAX_SPEED;

    Motor_LeftSetSpeed(leftSpeed);
    Motor_RightSetSpeed(rightSpeed);
}
