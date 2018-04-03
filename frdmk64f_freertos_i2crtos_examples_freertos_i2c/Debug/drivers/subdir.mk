################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_flash.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_smc.c \
../drivers/fsl_uart.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_flash.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_flash.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -DFRDM_K64F -DFSL_RTOS_FREE_RTOS -DCR_INTEGER_PRINTF -DFREEDOM -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\board" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\source" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\drivers" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\CMSIS" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\utilities" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\startup" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\drivers\freertos" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\freertos\include" -I"C:\i2c\SEBM2_i2c\frdmk64f_freertos_i2crtos_examples_freertos_i2c\freertos\portable" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


