/*
 * input.h
 *
 *  Created on: Nov 21, 2019
 *      Author: giand
 */

#pragma once
#include "stm32wbxx_hal.h"
#include "pulse_processor.h"
#include "circular_buffer.h"
#include "messages.h"

typedef struct Input{
	CircularBuffer pulses_buf_;
	// Index of this input.
	uint32_t input_idx_;

    uint16_t rise_time_;
    uint8_t rise_valid_;
    uint32_t cmp_threshold_;
    uint8_t pulse_polarity_;

	PulseProcessor *next;
} Input;

void do_work_input(Input *self, uint16_t cur_time);
void enqueue_pulse(Input *self, uint16_t start_time, uint16_t len);


