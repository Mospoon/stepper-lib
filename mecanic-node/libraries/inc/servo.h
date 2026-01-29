#ifndef SERVO_H
#define SERVO_H

#include "main.h"




typedef struct {
	TIM_HandleTypeDef *htim;
	uint16_t channel;
	float current_angle;

} servo_t;




void servo_init(servo_t *s);
void servo_set_angle(servo_t *s,float angle);
void servo_move_smooth(servo_t *s,float angle ,float speed);
void servo_set_speed(servo_t *s,int16_t speed);
void servo_stop(servo_t *s);

#endif
