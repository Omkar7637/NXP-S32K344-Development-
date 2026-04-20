################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeMASTER/common/freemaster_appcmd.c \
../FreeMASTER/common/freemaster_can.c \
../FreeMASTER/common/freemaster_pdbdm.c \
../FreeMASTER/common/freemaster_pipes.c \
../FreeMASTER/common/freemaster_protocol.c \
../FreeMASTER/common/freemaster_rec.c \
../FreeMASTER/common/freemaster_scope.c \
../FreeMASTER/common/freemaster_serial.c \
../FreeMASTER/common/freemaster_sha.c \
../FreeMASTER/common/freemaster_tsa.c \
../FreeMASTER/common/freemaster_ures.c \
../FreeMASTER/common/freemaster_utils.c 

OBJS += \
./FreeMASTER/common/freemaster_appcmd.o \
./FreeMASTER/common/freemaster_can.o \
./FreeMASTER/common/freemaster_pdbdm.o \
./FreeMASTER/common/freemaster_pipes.o \
./FreeMASTER/common/freemaster_protocol.o \
./FreeMASTER/common/freemaster_rec.o \
./FreeMASTER/common/freemaster_scope.o \
./FreeMASTER/common/freemaster_serial.o \
./FreeMASTER/common/freemaster_sha.o \
./FreeMASTER/common/freemaster_tsa.o \
./FreeMASTER/common/freemaster_ures.o \
./FreeMASTER/common/freemaster_utils.o 

C_DEPS += \
./FreeMASTER/common/freemaster_appcmd.d \
./FreeMASTER/common/freemaster_can.d \
./FreeMASTER/common/freemaster_pdbdm.d \
./FreeMASTER/common/freemaster_pipes.d \
./FreeMASTER/common/freemaster_protocol.d \
./FreeMASTER/common/freemaster_rec.d \
./FreeMASTER/common/freemaster_scope.d \
./FreeMASTER/common/freemaster_serial.d \
./FreeMASTER/common/freemaster_sha.d \
./FreeMASTER/common/freemaster_tsa.d \
./FreeMASTER/common/freemaster_ures.d \
./FreeMASTER/common/freemaster_utils.d 


# Each subdirectory must supply rules for building sources it contributes
FreeMASTER/common/%.o: ../FreeMASTER/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@FreeMASTER/common/freemaster_appcmd.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


