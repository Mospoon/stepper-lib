################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/src/UART_mec.c \
../libraries/src/servo.c \
../libraries/src/stepper.c 

OBJS += \
./libraries/src/UART_mec.o \
./libraries/src/servo.o \
./libraries/src/stepper.o 

C_DEPS += \
./libraries/src/UART_mec.d \
./libraries/src/servo.d \
./libraries/src/stepper.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/src/%.o libraries/src/%.su libraries/src/%.cyclo: ../libraries/src/%.c libraries/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hp/STM32CubeIDE/mecanic-node/libraries/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-libraries-2f-src

clean-libraries-2f-src:
	-$(RM) ./libraries/src/UART_mec.cyclo ./libraries/src/UART_mec.d ./libraries/src/UART_mec.o ./libraries/src/UART_mec.su ./libraries/src/servo.cyclo ./libraries/src/servo.d ./libraries/src/servo.o ./libraries/src/servo.su ./libraries/src/stepper.cyclo ./libraries/src/stepper.d ./libraries/src/stepper.o ./libraries/src/stepper.su

.PHONY: clean-libraries-2f-src

