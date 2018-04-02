################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.axf

# Tool invocations
frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -print-memory-usage -Xlinker --gc-sections -Xlinker -Map="frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.map" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master_Debug.ld -o "frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.axf"; # arm-none-eabi-objcopy -v -O binary "frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.axf" "frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.bin" ; # checksum -p MK64FN1M0xxx12 -d "frdmk64f_i2cdriver_examples_i2c_i2c_interrupt_b2b_transfer_master.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
