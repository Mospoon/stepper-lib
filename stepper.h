

#include "main.h"


typedef struct {
    TIM_HandleTypeDef *htim;
    uint16_t channel;

    GPIO_TypeDef *GPIO_Direction;
    uint16_t GPIO_Pin_Direction;

    uint32_t current_speed;
    uint16_t counter;
    uint16_t step_target;
} stepper_t;

/* Direction */
typedef enum {
	POS = 0,
	NEG = 1
} dir_stepper_t;


void stepper_move(stepper_t *st,uint32_t steps,uint32_t speed,dir_stepper_t dir);
void stepper_move_accel(stepper_t *st,uint32_t steps,uint32_t speed,dir_stepper_t dir);
void stepper_set_speed(stepper_t *st, uint32_t speed,dir_stepper_t dir);
void stepper_stop(stepper_t *st);


