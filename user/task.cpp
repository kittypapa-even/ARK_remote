#include "task.hpp"

BSP_ADC* rocker_adc = nullptr;
Encoder* shoulder_encode = nullptr; // 用于摇杆编码器
Encoder* elbow_encode = nullptr;    // 用于摇杆编码器
uint16_t adcBuffer[4]; //右摇杆x：0 y：4  左摇杆x：3  y：2
int armBuffer[2]; //机械臂编码器数据 shoulder:0 elbow:1
SendPacket_t sendPacket;
extern struct Bkeys bkeys[];
uint8_t PAWkey1;
uint8_t PAWkey2;

uint8_t key;

void myinit()
{
    OLED_Init();
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
    armBuffer[1]=-elbow_encode->getPos(); // 获取机械臂编码器数据
    process_ring_buffer();
    sendPacket.Vx= adcBuffer[1]; // 获取右摇杆X轴数据
    sendPacket.Vy= adcBuffer[2]; // 获取右摇杆Y轴数据
    sendPacket.Vw= adcBuffer[0]; // 获取左摇杆X轴数据
    sendPacket.shoulder=armBuffer[0];// 获取肩电机数据
    sendPacket.elbow=armBuffer[1]; // 获取肘电机数据
    key_loop();
    send_packet(&huart1, &sendPacket); // 发送数据包
}

void key_loop()
{
    PAWkey1=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12); // 读取按键状态
    PAWkey2=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1); // 读取按键状态
    if (PAWkey1) {
        sendPacket.PAWkey1=0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET); // 切换LED状态
    }
    else {
        sendPacket.PAWkey1=1;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_SET); // 切换LED状态
    }

    if (PAWkey2) {
        sendPacket.PAWkey2=1;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET); // 切换LED状态
    }
    else {
        sendPacket.PAWkey2=0;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET); // 切换LED状态
    }

    if (bkeys[4].short_flag==1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
        static uint8_t flag4 = 1;
        if (flag4==0) {
            sendPacket.ARMkey=0;
            flag4=1;
        }
        else if (flag4==1) {
            sendPacket.ARMkey=1;
            flag4=0;
        }
        bkeys[4].short_flag=0;
    }

    if (bkeys[2].short_flag==1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
        static uint8_t flag2 = 1;
        if (flag2==0) {
            sendPacket.RESET=0;
            flag2=1;
        }
        else if (flag2==1) {
            sendPacket.RESET=1;
            flag2=0;
        }
        bkeys[2].short_flag=0;
    }

    if (bkeys[5].short_flag==1) {
        static uint8_t flag5 = 1;
        if (flag5==0) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
            sendPacket.VELkey=0;
            flag5=1;
        }
        else if (flag5==1) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
            sendPacket.VELkey=1;
            flag5=0;
        }
        bkeys[5].short_flag=0;
    }
}





