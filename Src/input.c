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
//volatile uint32_t count = 0;
void PulseHandlerTask(void *argument){
	_Input(&input0, 0);
	//uint64_t timer = 0;
	Pulse p;
	while(1){

		osMessageQueueGet (pulseQueue, (void *) &p, NULL, osWaitForever);
		//UART_Print_uint16_t(p.pulse_len);
		consume_pulse(input0.next, &p);
		do_work_pulse_processor(input0.next, __HAL_TIM_GET_COUNTER(&htim16));

		/*
		if(timer % 65536 == 0){
			float x = input0.next->next->pos_.pos[0];
			float y = input0.next->next->pos_.pos[1];
			//UART_Print_float(x);
		}
		timer += 1;
		*/
	}
}


void enqueue_pulse(Input *self, uint16_t start_time, uint16_t len){
	Pulse p = {self->input_idx_, start_time, len};
	osMessageQueuePut(pulseQueue, (const void *) &p, 0, 0);

	//osMessageQueueGet (pulseQueue, (void *) &p, NULL, 0);
	//Pulse p = {0, 420, 69};
	//osMessageQueuePut(pulseQueue, (const void *) &p, 0, 0);

	//UART_Print_uint16_t(np.pulse_len);
	//UART_Print_uint16_t(p.pulse_len);
}
