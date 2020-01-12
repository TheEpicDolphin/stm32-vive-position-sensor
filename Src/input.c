/*
 * input.c
 *
 *  Created on: Dec 4, 2019
 *      Author: giand
 */
#include "input.h"
#include "UART_Print.h"

PulseProcessor pulse_processor;

void _Input(Input *self, uint8_t input_idx){
	/*
	BaseStationGeometryDef bs_0 = {{0.682646, 1.712605, 0.298152},
			  	  	  	  	  	  {0.356806, -0.017381, 0.934017, 0.001791, 0.999838, 0.017922, -0.934177, -0.004722, 0.356779}};

	BaseStationGeometryDef bs_1 = {{0.780941, 2.300994, -0.204002},
	  	  	  	  	  	  	  	  {-0.184830, -0.411017, 0.892694, 0.104180, 0.895032, 0.433664, -0.977233, 0.173155, -0.122609}};

	SensorLocalGeometry s_loc_geo = {0, {0.0, 0.0, 0.0}};
	GeometryBuilder geometry_builder = {{bs_0, bs_1}, {s_loc_geo}};

	_PulseProcessor(&pulse_processor, 1);
	pulse_processor.next = &geometry_builder;
	input.next = &pulse_processor;
	input.input_idx_ = input_idx;
	*/
}

void PulseHandlerTask(void *argument){
	_Input(&input0, 0);
	//uint64_t timer = 0;
	while(1){

		Pulse p;
		osMessageQueueGet (pulseQueue, (void *) &p, NULL, osWaitForever);
		UART_Print_uint16_t(p.pulse_len);


		//Pulse *p = (Pulse *) pvPortMalloc(sizeof(Pulse));
		//osMessageQueueGet (pulseQueue, (void *) p, NULL, osWaitForever);
		//UART_Print_uint16_t(p->pulse_len);
		//free((void *) p);

		//consume_pulse(input0.next, p);
		//do_work_pulse_processor(input0.next, __HAL_TIM_GET_COUNTER(&htim16));

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
	//UART_Print_uint16_t(p.pulse_len);
}
