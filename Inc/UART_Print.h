#pragma once
#include "stm32wbxx_hal.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

void UART_Send_Start();

void UART_Print_float(float val);

void UART_Print_uint32_t(uint32_t val);

void UART_Print_uint16_t(uint16_t val);

void UART_Print_int32_t(int32_t val);

void UART_Print_int16_t(int16_t val);
