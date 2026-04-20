################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPPSDK_GD3000_Driver/gd3000_init.c 

OBJS += \
./TPPSDK_GD3000_Driver/gd3000_init.o 

C_DEPS += \
./TPPSDK_GD3000_Driver/gd3000_init.d 


# Each subdirectory must supply rules for building sources it contributes
TPPSDK_GD3000_Driver/%.o: ../TPPSDK_GD3000_Driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@TPPSDK_GD3000_Driver/gd3000_init.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


