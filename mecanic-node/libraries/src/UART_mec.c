#include "UART_mec.h"
#include "string.h"
#include "main.h"
#include "stepper.h"
#include "servo.h"

// UART buffers
uint8_t UART_RxBuffer[UART_MSG_SIZE];
uint8_t UART_MainBuffer[UART_MSG_SIZE];
uint8_t UART_TxBuffer[UART_MSG_SIZE];
volatile uint8_t UART_Sent = 0;

// Stepper & Servo instances


void UART_receive(void)
{
    memset(UART_RxBuffer, 0, UART_MSG_SIZE);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, UART_RxBuffer, UART_MSG_SIZE);
}

HAL_StatusTypeDef UART_Transmit(const uint8_t *data)
{
    if (data == NULL) return HAL_ERROR;
    if (huart1.gState == HAL_UART_STATE_BUSY_TX) return HAL_BUSY;

    memcpy(UART_TxBuffer, data, UART_MSG_SIZE);
    UART_Sent = 0;
    return HAL_UART_Transmit_IT(&huart1, UART_TxBuffer, UART_MSG_SIZE);
}

float UART_BytesToFloat(uint8_t *bytes)
{
    union { uint8_t b[4]; float f; } conv;
    memcpy(conv.b, bytes, 4);
    return conv.f;
}

uint32_t UART_FloatToBytes(float f)
{
    union { float val; uint32_t u32; } conv;
    conv.val = f;
    return conv.u32;
}

void UART_Prepare(uint8_t cmd, uint8_t *payload, uint8_t payload_len)
{
    memset(UART_TxBuffer, 0, UART_MSG_SIZE);
    UART_TxBuffer[0] = CMD_START;
    UART_TxBuffer[1] = cmd;
    UART_TxBuffer[2] = payload_len;
    if (payload_len > 0 && payload != NULL)
        memcpy(&UART_TxBuffer[3], payload, payload_len);

    uint8_t cs = 0;
    for (uint8_t i = 1; i < 3 + payload_len; i++)
        cs ^= UART_TxBuffer[i];

    UART_TxBuffer[3 + payload_len] = cs;
    UART_TxBuffer[4 + payload_len] = CMD_END;
    UART_Transmit(UART_TxBuffer);
}


void UART_Process(void)
{
    uint8_t cmd = UART_MainBuffer[1];

    switch (cmd)
    {

    case CMD_STEPPER_MOVE:
    {
        uint8_t  stepper_mask = UART_MainBuffer[3];
        uint16_t steps      = (UART_MainBuffer[4] << 8) | UART_MainBuffer[5];
        float    speed      = UART_BytesToFloat(&UART_MainBuffer[6]);
        float    accel      = UART_BytesToFloat(&UART_MainBuffer[10]);
        uint8_t  dir        = UART_MainBuffer[14];

        if (stepper_mask & 0x01)
        {
            stepper_move_accel(&stepper1, steps, (uint32_t)speed,
                               (uint32_t)accel, (dir_stepper_t)dir);
        }

        if (stepper_mask & 0x02)
        {
            stepper_move_accel(&stepper2, steps, (uint32_t)speed,
                               (uint32_t)accel, (dir_stepper_t)dir);
        }

        if (stepper_mask & 0x04)
        {
            stepper_move_accel(&stepper3, steps, (uint32_t)speed,
                               (uint32_t)accel, (dir_stepper_t)dir);
        }

        if (stepper_mask & 0x08)
        {
            stepper_move_accel(&stepper4, steps, (uint32_t)speed,
                               (uint32_t)accel, (dir_stepper_t)dir);
        }

        UART_Prepare(CMD_STEPPER_MOVE, 0, 0);
        break;
    }

    case CMD_STEPPER_STOP:
    {
        uint8_t stepper_mask = UART_MainBuffer[3];


        if (stepper_mask & 0x01)
                {
        	stepper_stop(&stepper1);
                }

                if (stepper_mask & 0x02)
                {
                	stepper_stop(&stepper2);
                }

                if (stepper_mask & 0x04)
                {
                	stepper_stop(&stepper3);
                }

                if (stepper_mask & 0x08)
                {
                	stepper_stop(&stepper4);
                }
        UART_Prepare(CMD_STEPPER_STOP, 0, 0);
        break;
    }




        case CMD_SERVO_SET_ANGLE:
        {
            uint8_t servo_id = UART_MainBuffer[3];
            float angle      = UART_BytesToFloat(&UART_MainBuffer[4]);
            servo_t *s       = (servo_id==1) ? &servo1 : &servo2;
            servo_set_angle(s, angle);
            UART_Prepare(CMD_SERVO_SET_ANGLE, 0, 0);
            break;
        }
        case CMD_SERVO_MOVE_SMOOTH:
{
    uint8_t servo_mask = UART_MainBuffer[3];
    float angle        = UART_BytesToFloat(&UART_MainBuffer[4]);
    float speed        = UART_BytesToFloat(&UART_MainBuffer[8]);

    if (servo_mask & 0x01)   
    {
        servo_move_smooth(&servo1, angle, speed);
    }

    if (servo_mask & 0x02)   
    {
        servo_move_smooth(&servo2, angle, speed);
    }

    if (servo_mask & 0x04)   
    {
        servo_move_smooth(&servo3, angle, speed);
    }

    if (servo_mask & 0x08)   
    {
        servo_move_smooth(&servo4, angle, speed);
    }

    if (servo_mask & 0x10)   
    {
        servo_move_smooth(&servo5, angle, speed);
    }

    if (servo_mask & 0x20)   
    {
        servo_move_smooth(&servo6, angle, speed);
    }

    UART_Prepare(CMD_SERVO_MOVE_SMOOTH, 0, 0);
    break;
}

        case CMD_SERVO_SET_SPEED:
        {
            uint8_t servo_id = UART_MainBuffer[3];
            uint16_t speed   = (UART_MainBuffer[4]<<8) | UART_MainBuffer[5];
            servo_t *s       = (servo_id==1) ? &servo1 : &servo2;
            servo_set_speed(s, speed);
            UART_Prepare(CMD_SERVO_SET_SPEED, 0, 0);
            break;
        }
       case CMD_SERVO_STOP:
{
    uint8_t servo_mask = UART_MainBuffer[3];

    if (servo_mask & 0x01)   
    {
        servo_stop(&servo1);
    }

    if (servo_mask & 0x02)   
    {
        servo_stop(&servo2);
    }

    if (servo_mask & 0x04)   
    {
        servo_stop(&servo3);
    }

    if (servo_mask & 0x08)   
    {
        servo_stop(&servo4);
    }

    if (servo_mask & 0x10)   
    {
        servo_stop(&servo5);
    }

    if (servo_mask & 0x20)   
    {
        servo_stop(&servo6);
    }

    UART_Prepare(CMD_SERVO_STOP, 0, 0);
    break;
}


        case CMD_CHECK_STEPPER:
        {
            uint8_t stepper_id = UART_MainBuffer[3];
            uint8_t status ;

            switch (stepper_id)
            {
                case 1: status = stepper1.flag_stepper; break;
                case 2: status = stepper2.flag_stepper; break;
                case 3: status = stepper3.flag_stepper; break;
                case 4: status = stepper4.flag_stepper; break;
                default: break;
            }

            UART_Prepare(CMD_CHECK_STEPPER, &status, 1);
            break;
        }





        default:
            break;
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        memcpy(UART_MainBuffer, UART_RxBuffer, Size);
        memset(UART_RxBuffer, 0, UART_MSG_SIZE);
        UART_Process();
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, UART_RxBuffer, UART_MSG_SIZE);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        UART_Sent = 1;
    }
}
