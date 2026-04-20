################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeMASTER/drivers/dreg/S32K/freemaster_s32k3xx_lpuart.c 

OBJS += \
./FreeMASTER/drivers/dreg/S32K/freemaster_s32k3xx_lpuart.o 

C_DEPS += \
./FreeMASTER/drivers/dreg/S32K/freemaster_s32k3xx_lpuart.d 


# Each subdirectory must supply rules for building sources it contributes
FreeMASTER/drivers/dreg/S32K/%.o: ../FreeMASTER/drivers/dreg/S32K/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@FreeMASTER/drivers/dreg/S32K/freemaster_s32k3xx_lpuart.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


