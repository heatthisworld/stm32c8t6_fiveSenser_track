#include "Track.h"

int OUTPUT1 = 0, OUTPUT2 = 0; //  OUTPUT1 和OUTPUT2 是控制小车左右轮速度的PWM输出值

int PID_sd = 0; // 小车差速PID控制器的PWM输出值

int error = 0; // 引用lk.c定义的error，才能在本文件内使用error该变量  //作为舵机PID控制器和差速PID控制器的传参输入值

double a = 0; // 最后的速度乘上a，实现调速

int Position_PID(int Encoder, int Target) // 差速PID控制器//这里Encoder=error，Target=0;
{
    float Position_KP = 650, Position_KI = 0, Position_KD = 7300; // 这里未使用到Ki
    static float Bias = 0, Pwm = 0, Integral_bias = 0, Last_Bias = 0;
    Bias = Encoder - Target;                                                                   // 20       //0                        //计算偏差
    Integral_bias += Bias;                                                                     // 求出偏差的积分
    Pwm = Position_KP * Bias + Position_KI * Integral_bias + Position_KD * (Bias - Last_Bias); // 0-20        //位置式PID控制器
    Last_Bias = Bias;                                                                          // 20      0                             //保存上一次偏差
    return Pwm;
    // 返回PWM值
}

void Track()
{
}