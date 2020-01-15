/*
 * UART_Print.c
 *
 *  Created on: Dec 9, 2019
 *      Author: giand
 */


#include "UART_Print.h"
#include "arm_math.h"

#define BUF_SIZE 4



void UART_Send_Start(){
	char *message_buf = "start\n";
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, strlen(message_buf), 0xFFFF);
}

void UART_Print_3DCoords(float *pos){
	uint8_t message_buf[12];
	memcpy(message_buf, pos, 3 * sizeof(float));
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, 12, 1000);
}

void UART_Print_float(float val){
	uint8_t message_buf[BUF_SIZE];
	memcpy(message_buf, &val, sizeof(float));
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, BUF_SIZE, 1000);
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)message_buf, BUF_SIZE);
}

void UART_Print_uint32_t(uint32_t val){
	uint8_t message_buf[BUF_SIZE];
	memcpy(message_buf, &val, sizeof(uint32_t));
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, BUF_SIZE, 1000);
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)message_buf, BUF_SIZE);
}

void UART_Print_uint16_t(uint16_t val){
	uint8_t message_buf[BUF_SIZE];
	memcpy(message_buf, &val, sizeof(uint16_t));
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, BUF_SIZE, 1000);
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)message_buf, BUF_SIZE);
}

void UART_Print_int32_t(int32_t val){
	uint8_t message_buf[BUF_SIZE];
	memcpy(message_buf, &val, sizeof(int32_t));
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, BUF_SIZE, 1000);
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)message_buf, BUF_SIZE);
}

void UART_Print_int16_t(int16_t val){
	uint8_t message_buf[BUF_SIZE];
	memcpy(message_buf, &val, sizeof(int16_t));
	HAL_UART_Transmit(&huart1, (uint8_t*)message_buf, BUF_SIZE, 1000);
	//HAL_UART_Transmit_IT(&huart1, (uint8_t*)message_buf, BUF_SIZE);
}
