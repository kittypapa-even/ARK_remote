#include "task.hpp"

BSP_ADC* rocker_adc = nullptr;
Encoder* shoulder_encode = nullptr; // 用于摇杆编码器
Encoder* elbow_encode = nullptr;    // 用于摇杆编码器
uint16_t adcBuffer[4]; //右摇杆x：0 y：4  左摇杆x：3  y：2
uint16_t armBuffer[2]; //机械臂编码器数据 shoulder:0 elbow:1
SendPacket_t sendPacket;
extern struct Bkeys bkeys[];

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
    process_ring_buffer();
    sendPacket.Vx= adcBuffer[1]; // 获取右摇杆X轴数据
    sendPacket.Vy= adcBuffer[2]; // 获取右摇杆Y轴数据
    sendPacket.Vw= adcBuffer[0]; // 获取左摇杆X轴数据
    if (bkeys[1].short_flag==1) {
        static uint8_t flag = 1;
        if (flag==0) {
            sendPacket.VELkey=0;
            flag=1;
        }
        else if (flag==1) {
            sendPacket.VELkey=1;
            flag=0;
        }
        bkeys[1].short_flag=0;
    }
    send_packet(&huart1, &sendPacket); // 发送数据包
}







