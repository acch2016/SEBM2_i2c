################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mk64f12.c 

OBJS += \
./startup/startup_mk64f12.o 

C_DEPS += \
./startup/startup_mk64f12.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -DFRDM_K64F -DCR_INTEGER_PRINTF -DFREEDOM -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master\board" -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master\source" -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master" -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master\drivers" -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master\CMSIS" -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master\utilities" -I"C:\i2c\frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master\startup" -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


