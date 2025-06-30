#include "task.hpp"

BSP_ADC* rocker_adc = nullptr;
Encoder* shoulder_encode = nullptr; // 用于摇杆编码器
Encoder* elbow_encode = nullptr;    // 用于摇杆编码器
uint16_t adcBuffer[4]; //右摇杆x：0 y：4  左摇杆x：3  y：2
uint16_t armBuffer[2]; //机械臂编码器数据 shoulder:0 elbow:1

void myinit()
{
    rocker_adc = new BSP_ADC(&hadc1);
    shoulder_encode = new Encoder(&htim1, 65535); // 65535是16位计数器的最大值
    elbow_encode = new Encoder(&htim2, 65535);    // 65535是16位计数器的最大值
    shoulder_encode->start();
    elbow_encode->start(); // 启动编码器
}

void myloop()
{
    rocker_adc->init(adcBuffer, 4); // 启动 DMA 采集
    armBuffer[0]=shoulder_encode->getPos(); // 获取机械臂编码器数据
    armBuffer[1]=elbow_encode->getPos(); // 获取机械臂编码器数据
}







