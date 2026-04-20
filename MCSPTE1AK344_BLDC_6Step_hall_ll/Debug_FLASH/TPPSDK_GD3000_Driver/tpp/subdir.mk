################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPPSDK_GD3000_Driver/tpp/tpp.c 

OBJS += \
./TPPSDK_GD3000_Driver/tpp/tpp.o 

C_DEPS += \
./TPPSDK_GD3000_Driver/tpp/tpp.d 


# Each subdirectory must supply rules for building sources it contributes
TPPSDK_GD3000_Driver/tpp/%.o: ../TPPSDK_GD3000_Driver/tpp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@TPPSDK_GD3000_Driver/tpp/tpp.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


