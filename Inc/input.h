/*
 * input.h
 *
 *  Created on: Nov 21, 2019
 *      Author: giand
 */

#pragma once
#include "stm32wbxx_hal.h"
#include "pulse_processor.h"
#include "geometry.h"
#include "circular_buffer.h"
#include "messages.h"
#include "cmsis_os2.h"
#include "tim.h"

typedef struct Input{
	CircularBuffer pulses_buf_;
	// Index of this input.
	uint8_t input_idx_;

    uint16_t rise_time_;
    uint8_t rise_valid_;
    uint32_t cmp_threshold_;
    uint8_t pulse_polarity_;

	PulseProcessor *next;
} Input;

Input input0;
osThreadId_t pulseHandlerTaskHandle;
osMessageQueueId_t pulseQueue;

void _Input(Input *self, uint8_t input_idx);
void PulseHandlerTask(void *argument);
void enqueue_pulse(Input *self, uint16_t start_time, uint16_t len);



