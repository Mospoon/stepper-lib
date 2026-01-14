#include "stepper.h"

// this function is to mover the stepper with precise number of steps with a fixed speed
void stepper_move(stepper_t *st,uint32_t steps,uint32_t speed,dir_stepper_t dir){// the speed is the number of steps per second

	HAL_GPIO_WritePin(st->GPIO_Direction, st->GPIO_Pin_Direction, dir);
	if(!speed){
			stepper_stop(st);
		}

	st->step_target = 2* steps;// 1 step means toggling 2 times


	uint32_t compare = 1000000 / (speed * 2);// my timer is 1MHz frequency

	 __HAL_TIM_SET_COMPARE(st->htim, st->channel, compare);// we set the pulse width
	 HAL_TIM_OC_Start_IT(st->htim, st->channel);// we start the pulses

}

// this function is for continuous rotation stepper
void stepper_set_speed(stepper_t *st, uint32_t speed,dir_stepper_t dir){
	HAL_GPIO_WritePin(st->GPIO_Direction, st->GPIO_Pin_Direction, dir);
	if(!speed){
		stepper_stop(st);

	}

		uint32_t compare = 1000000 / (speed * 2);// my timer is 1MHz frequency

		 __HAL_TIM_SET_COMPARE(st->htim, st->channel, compare);// we set the pulse width
		 HAL_TIM_OC_Start(st->htim, st->channel);// we start the pulses

}
// this function is to stop the stepper
void stepper_stop(stepper_t *st){
	 HAL_TIM_OC_Stop_IT(st->htim, st->channel);
}



