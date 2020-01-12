/*
 * input.c
 *
 *  Created on: Dec 4, 2019
 *      Author: giand
 */
#include "input.h"
#include "UART_Print.h"

void do_work_input(Input *self, uint16_t cur_time){

	Pulse *p;
	//First place to optimize
	while(!empty(&self->pulses_buf_)){
		p = front(&self->pulses_buf_);
		consume_pulse(self->next, p);
		pop_front(&self->pulses_buf_);
	}

}

void enqueue_pulse(Input *self, uint16_t start_time, uint16_t len){
	//Debugging ------

	//UART_Print_uint64_t(start_time);
	//UART_Print_uint64_t(len);
	//----------------


    Pulse *p = malloc(sizeof(Pulse));
    p->input_idx = self->input_idx_;
    p->start_time = start_time;
    p->pulse_len = len;
    append_back(&self->pulses_buf_, p);

}
