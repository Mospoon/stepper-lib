#ifndef INC_UART_MEC_H_
#define INC_UART_MEC_H_

#include "main.h"
#include "stepper.h"
#include "servo.h"
#include <stdint.h>

// UART Commands
typedef enum {
    CMD_START = 0xAA,
    CMD_END = 0x00,
    CMD_STEPPER_MOVE = 0x01,
    CMD_STEPPER_SET_SPEED = 0x02,
    CMD_STEPPER_STOP = 0x03,
    CMD_SERVO_SET_ANGLE = 0x10,
    CMD_SERVO_MOVE_SMOOTH = 0x11,
    CMD_SERVO_SET_SPEED = 0x12,
    CMD_SERVO_STOP = 0x13,
	CMD_CHECK_STEPPER =0x14,
	CMD_CHECK_ANGLE=0x15
} UART_Command_t;

#define UART_MSG_SIZE 32u
extern UART_HandleTypeDef huart1;
extern uint8_t UART_RxBuffer[UART_MSG_SIZE];
extern uint8_t UART_MainBuffer[UART_MSG_SIZE];
extern volatile uint8_t UART_Sent;
extern uint8_t UART_TxBuffer[UART_MSG_SIZE];

extern stepper_t stepper1;
extern stepper_t stepper2;
extern stepper_t stepper3;
extern stepper_t stepper4;

extern servo_t servo1;
extern servo_t servo2;


void UART_receive(void);
HAL_StatusTypeDef UART_Transmit(const uint8_t *data);
float UART_BytesToFloat(uint8_t *bytes);
uint32_t UART_FloatToBytes(float f);
void UART_Prepare(uint8_t cmd, uint8_t *payload, uint8_t payload_len);
void UART_Process(void);

#endif /* INC_UART_MEC_H_ */
