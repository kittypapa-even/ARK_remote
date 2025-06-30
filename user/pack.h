#ifndef _PACK_H_
#define _PACK_H_

#include "main.h"
#include <string.h>

typedef struct __attribute__((packed)) {
    uint8_t length;
} ReceivePacket_t;
#define PACKET_SIZE sizeof(ReceivePacket_t)

typedef struct __attribute__((packed)) {
    uint8_t length;
    uint8_t data;
} SendPacket_t;
#define SEND_PACKET_SIZE sizeof(SendPacket_t)

void send_packet(UART_HandleTypeDef *huart, const ReceivePacket_t *payload);
void process_ring_buffer();

#endif