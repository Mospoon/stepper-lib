#include "stepper.h"
const float STEPS_PER_MM = 400.0f;
// this function is to mover the stepper with precise number of steps with a fixed speed
void stepper_move(stepper_t *st,uint32_t steps,uint32_t speed,dir_stepper_t dir){// the speed is the number of steps per second

	HAL_GPIO_WritePin(st->GPIO_Direction, st->GPIO_Pin_Direction, dir);
	if(!speed){
			stepper_stop(st);
		}

	st->step_target = 2* steps;// 1 step means toggling 2 times


	 st->compare = 1000000 / (speed * 2);// my timer is 1MHz frequency

	 __HAL_TIM_SET_COUNTER(st->htim, 0);

	 __HAL_TIM_SET_COMPARE(st->htim, st->channel, st->compare);// we set the pulse width
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
	st->flag_stepper=0;
	 HAL_TIM_OC_Stop_IT(st->htim, st->channel);
}
void stepper_move_accel(stepper_t *st,uint16_t distance,uint32_t speed,float accel,dir_stepper_t dir)
{
	st->flag_stepper = 1;
    HAL_GPIO_WritePin(st->GPIO_Direction, st->GPIO_Pin_Direction, dir);

    if (speed == 0 || accel == 0.0f)
    {
        stepper_stop(st);
        return;
    }

    st->step_target = (uint32_t)(distance ) * 2;
    st->counter     = 0;

    st->current_speed = 1.0f;
    st->target_speed  = speed ;
    st->acceleration  = accel ;

    st->compare = 1000000 / (st->current_speed * 2);

    __HAL_TIM_SET_COUNTER(st->htim, 0);
    __HAL_TIM_SET_COMPARE(st->htim, st->channel, st->compare);
    HAL_TIM_OC_Start_IT(st->htim, st->channel);
}

// this function update the pulse so everytime interrupt happens it update the counter and check if it reached the targed of the oc stops
void stepper_update(stepper_t *st)
{
    st->counter++;

    uint32_t steps_remaining = st->step_target - st->counter;

    float steps_to_stop = st->current_speed / st->acceleration;

    if (steps_remaining > steps_to_stop)
    {
        if (st->current_speed < st->target_speed)
        {
            st->current_speed += st->acceleration;
            if (st->current_speed > st->target_speed)
                st->current_speed = st->target_speed;
        }
    }
    else
    {
        st->current_speed -= st->acceleration;
        if (st->current_speed < 1.0f)
            st->current_speed = 1.0f;
    }

    st->compare =
        (uint32_t)(1000000.0f / (st->current_speed * 2.0f));

    uint32_t next_compare =
        __HAL_TIM_GET_COMPARE(st->htim, st->channel) + st->compare;

    __HAL_TIM_SET_COMPARE(st->htim, st->channel, next_compare);

    if (st->counter >= st->step_target){
    	st->flag_stepper=0;
        HAL_TIM_OC_Stop_IT(st->htim, st->channel);}

}



