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
	while(1){
		osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
		_Input(&input0, 0);
		while(1){
			osMessageQueueGet (pulseQueue, (void *) &p, NULL, osWaitForever);
			consume_pulse(input0.next, &p);
		}
	}

}

VIVEVars vive_vars;
void MasterTask(void *argument){
	osThreadFlagsSet(pulseHandlerTaskHandle, 0x00000001U);
	while(1){
		osMessageQueueGet(viveQueue, (void *) &vive_vars, NULL, osWaitForever);
		UART_Print_float(vive_vars.pos[0]);
	}
}


void enqueue_pulse(Input *self, uint16_t start_time, uint16_t len){
	Pulse p_in = {self->input_idx_, start_time, len};
	osMessageQueuePut(pulseQueue, (const void *) &p_in, 0, 0);
	count = osMessageQueueGetCount(pulseQueue);
}
