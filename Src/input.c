/*
 * input.c
 *
 *  Created on: Dec 4, 2019
 *      Author: giand
 */
#include "input.h"
#include "UART_Print.h"

PulseProcessor pulse_processor;
GeometryBuilder geometry_builder;

void _Input(Input *self, uint8_t input_idx){
	_GeometryBuilder(&geometry_builder);
	_PulseProcessor(&pulse_processor, 1);
	pulse_processor.next = &geometry_builder;
	self->next = &pulse_processor;
	self->input_idx_ = input_idx;

}
volatile uint32_t count = 0;
Pulse p;
void PulseHandlerTask(void *argument){
	_Input(&input0, 0);
	while(1){
		osMessageQueueGet (pulseQueue, (void *) &p, NULL, osWaitForever);
		consume_pulse(input0.next, &p);
	}


	/*
	while(1){
		osThreadFlagsWait(, , osWaitForever);
		while(1){
			osMessageQueueGet (pulseQueue, (void *) &p, NULL, osWaitForever);
			consume_pulse(input0.next, &p);
		}
	}
	*/
}


void enqueue_pulse(Input *self, uint16_t start_time, uint16_t len){
	Pulse p_in = {self->input_idx_, start_time, len};
	osMessageQueuePut(pulseQueue, (const void *) &p_in, 0, 0);
	count = osMessageQueueGetCount(pulseQueue);
}
