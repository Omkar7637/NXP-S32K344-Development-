################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/actuate_s32k.c \
../src/main.c \
../src/meas_s32k.c \
../src/state_machine.c 

OBJS += \
./src/actuate_s32k.o \
./src/main.o \
./src/meas_s32k.o \
./src/state_machine.o 

C_DEPS += \
./src/actuate_s32k.d \
./src/main.d \
./src/meas_s32k.d \
./src/state_machine.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/actuate_s32k.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


