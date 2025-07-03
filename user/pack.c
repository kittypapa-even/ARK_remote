#include "pack.h"

#define PACKET_MAX_SIZE 32
#define RX_BUF_SIZE 256
extern uint8_t usart1_rx_buf[1]; // 临时接收 1 字节
extern uint8_t ring_buffer[];
extern volatile uint16_t write_index;
ReceivePacket_t packet;

void process_ring_buffer()
{
    static uint16_t read_index = 0;
    const uint16_t total_size = 1 + PACKET_SIZE + 1; // 包头 + 数据 + 包尾

    while (((write_index + RX_BUF_SIZE) - read_index) % RX_BUF_SIZE >= total_size)
    {
        // 查找包头
        if (ring_buffer[read_index] == 0xA5)
        {
            // 提取出包尾索引位置
            uint16_t end_index = (read_index + total_size - 1) % RX_BUF_SIZE;

            // 检查包尾是否为 0x5A
            if (ring_buffer[end_index] == 0x5A)
            {
                // 拷贝出数据部分（不含包头和包尾）
                uint8_t temp[PACKET_SIZE];
                for (int i = 0; i < PACKET_SIZE; i++)
                {
                    temp[i] = ring_buffer[(read_index + 1 + i) % RX_BUF_SIZE];
                }

                // 反序列化成结构体
                memcpy(&packet, temp, PACKET_SIZE);

                // ✅ 成功解析一个完整数据包：packet.pitch、packet.yaw 可用

                // 移动读指针，跳过整个包
                read_index = (read_index + total_size) % RX_BUF_SIZE;
                continue;
            }
        }

        // 若不是包头或包尾不对，继续找下一个字节
        read_index = (read_index + 1) % RX_BUF_SIZE;
    }
}

void send_packet(UART_HandleTypeDef *huart, const SendPacket_t *payload)
{
    uint8_t buffer[1 + SEND_PACKET_SIZE + 1];

    buffer[0] = 0xA5; // 包头
    memcpy(&buffer[1], payload, SEND_PACKET_SIZE); // 数据体
    buffer[1 + SEND_PACKET_SIZE] = 0x5A; // 包尾

    HAL_UART_Transmit(huart, buffer, sizeof(buffer), HAL_MAX_DELAY);
}


