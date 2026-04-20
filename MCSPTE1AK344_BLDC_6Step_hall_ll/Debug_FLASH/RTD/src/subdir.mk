################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTD/src/Adc_Sar_Ip.c \
../RTD/src/Adc_Sar_Ip_Irq.c \
../RTD/src/Bctu_Ip.c \
../RTD/src/Bctu_Ip_Irq.c \
../RTD/src/Clock_Ip.c \
../RTD/src/Clock_Ip_Data.c \
../RTD/src/Clock_Ip_Divider.c \
../RTD/src/Clock_Ip_DividerTrigger.c \
../RTD/src/Clock_Ip_ExtOsc.c \
../RTD/src/Clock_Ip_FracDiv.c \
../RTD/src/Clock_Ip_Frequency.c \
../RTD/src/Clock_Ip_Gate.c \
../RTD/src/Clock_Ip_IntOsc.c \
../RTD/src/Clock_Ip_Irq.c \
../RTD/src/Clock_Ip_Monitor.c \
../RTD/src/Clock_Ip_Pll.c \
../RTD/src/Clock_Ip_ProgFreqSwitch.c \
../RTD/src/Clock_Ip_Selector.c \
../RTD/src/Clock_Ip_Specific.c \
../RTD/src/Det.c \
../RTD/src/Det_stub.c \
../RTD/src/Emios_Icu_Ip.c \
../RTD/src/Emios_Icu_Ip_Irq.c \
../RTD/src/Emios_Mcl_Ip.c \
../RTD/src/Emios_Mcl_Ip_Irq.c \
../RTD/src/Emios_Pwm_Ip.c \
../RTD/src/Emios_Pwm_Ip_Irq.c \
../RTD/src/Igf_Port_Ip.c \
../RTD/src/IntCtrl_Ip.c \
../RTD/src/Lcu_Ip.c \
../RTD/src/Lcu_Ip_Hw_Access.c \
../RTD/src/Lcu_Ip_Irq.c \
../RTD/src/Lcu_Ip_Multicore.c \
../RTD/src/Lpspi_Ip.c \
../RTD/src/Lpspi_Ip_Irq.c \
../RTD/src/Lpuart_Uart_Ip.c \
../RTD/src/Lpuart_Uart_Ip_Irq.c \
../RTD/src/OsIf_Interrupts.c \
../RTD/src/OsIf_Timer.c \
../RTD/src/OsIf_Timer_System.c \
../RTD/src/OsIf_Timer_System_Internal_Systick.c \
../RTD/src/Pit_Ip.c \
../RTD/src/SchM_Adc.c \
../RTD/src/SchM_Dio.c \
../RTD/src/SchM_Gpt.c \
../RTD/src/SchM_Icu.c \
../RTD/src/SchM_Mcl.c \
../RTD/src/SchM_Mcu.c \
../RTD/src/SchM_Port.c \
../RTD/src/SchM_Pwm.c \
../RTD/src/SchM_Spi.c \
../RTD/src/SchM_Uart.c \
../RTD/src/Siul2_Dio_Ip.c \
../RTD/src/Siul2_Icu_Ip.c \
../RTD/src/Siul2_Icu_Ip_Irq.c \
../RTD/src/Siul2_Port_Ip.c \
../RTD/src/Trgmux_Ip.c \
../RTD/src/Trgmux_Ip_HwAcc.c \
../RTD/src/Tspc_Port_Ip.c 

OBJS += \
./RTD/src/Adc_Sar_Ip.o \
./RTD/src/Adc_Sar_Ip_Irq.o \
./RTD/src/Bctu_Ip.o \
./RTD/src/Bctu_Ip_Irq.o \
./RTD/src/Clock_Ip.o \
./RTD/src/Clock_Ip_Data.o \
./RTD/src/Clock_Ip_Divider.o \
./RTD/src/Clock_Ip_DividerTrigger.o \
./RTD/src/Clock_Ip_ExtOsc.o \
./RTD/src/Clock_Ip_FracDiv.o \
./RTD/src/Clock_Ip_Frequency.o \
./RTD/src/Clock_Ip_Gate.o \
./RTD/src/Clock_Ip_IntOsc.o \
./RTD/src/Clock_Ip_Irq.o \
./RTD/src/Clock_Ip_Monitor.o \
./RTD/src/Clock_Ip_Pll.o \
./RTD/src/Clock_Ip_ProgFreqSwitch.o \
./RTD/src/Clock_Ip_Selector.o \
./RTD/src/Clock_Ip_Specific.o \
./RTD/src/Det.o \
./RTD/src/Det_stub.o \
./RTD/src/Emios_Icu_Ip.o \
./RTD/src/Emios_Icu_Ip_Irq.o \
./RTD/src/Emios_Mcl_Ip.o \
./RTD/src/Emios_Mcl_Ip_Irq.o \
./RTD/src/Emios_Pwm_Ip.o \
./RTD/src/Emios_Pwm_Ip_Irq.o \
./RTD/src/Igf_Port_Ip.o \
./RTD/src/IntCtrl_Ip.o \
./RTD/src/Lcu_Ip.o \
./RTD/src/Lcu_Ip_Hw_Access.o \
./RTD/src/Lcu_Ip_Irq.o \
./RTD/src/Lcu_Ip_Multicore.o \
./RTD/src/Lpspi_Ip.o \
./RTD/src/Lpspi_Ip_Irq.o \
./RTD/src/Lpuart_Uart_Ip.o \
./RTD/src/Lpuart_Uart_Ip_Irq.o \
./RTD/src/OsIf_Interrupts.o \
./RTD/src/OsIf_Timer.o \
./RTD/src/OsIf_Timer_System.o \
./RTD/src/OsIf_Timer_System_Internal_Systick.o \
./RTD/src/Pit_Ip.o \
./RTD/src/SchM_Adc.o \
./RTD/src/SchM_Dio.o \
./RTD/src/SchM_Gpt.o \
./RTD/src/SchM_Icu.o \
./RTD/src/SchM_Mcl.o \
./RTD/src/SchM_Mcu.o \
./RTD/src/SchM_Port.o \
./RTD/src/SchM_Pwm.o \
./RTD/src/SchM_Spi.o \
./RTD/src/SchM_Uart.o \
./RTD/src/Siul2_Dio_Ip.o \
./RTD/src/Siul2_Icu_Ip.o \
./RTD/src/Siul2_Icu_Ip_Irq.o \
./RTD/src/Siul2_Port_Ip.o \
./RTD/src/Trgmux_Ip.o \
./RTD/src/Trgmux_Ip_HwAcc.o \
./RTD/src/Tspc_Port_Ip.o 

C_DEPS += \
./RTD/src/Adc_Sar_Ip.d \
./RTD/src/Adc_Sar_Ip_Irq.d \
./RTD/src/Bctu_Ip.d \
./RTD/src/Bctu_Ip_Irq.d \
./RTD/src/Clock_Ip.d \
./RTD/src/Clock_Ip_Data.d \
./RTD/src/Clock_Ip_Divider.d \
./RTD/src/Clock_Ip_DividerTrigger.d \
./RTD/src/Clock_Ip_ExtOsc.d \
./RTD/src/Clock_Ip_FracDiv.d \
./RTD/src/Clock_Ip_Frequency.d \
./RTD/src/Clock_Ip_Gate.d \
./RTD/src/Clock_Ip_IntOsc.d \
./RTD/src/Clock_Ip_Irq.d \
./RTD/src/Clock_Ip_Monitor.d \
./RTD/src/Clock_Ip_Pll.d \
./RTD/src/Clock_Ip_ProgFreqSwitch.d \
./RTD/src/Clock_Ip_Selector.d \
./RTD/src/Clock_Ip_Specific.d \
./RTD/src/Det.d \
./RTD/src/Det_stub.d \
./RTD/src/Emios_Icu_Ip.d \
./RTD/src/Emios_Icu_Ip_Irq.d \
./RTD/src/Emios_Mcl_Ip.d \
./RTD/src/Emios_Mcl_Ip_Irq.d \
./RTD/src/Emios_Pwm_Ip.d \
./RTD/src/Emios_Pwm_Ip_Irq.d \
./RTD/src/Igf_Port_Ip.d \
./RTD/src/IntCtrl_Ip.d \
./RTD/src/Lcu_Ip.d \
./RTD/src/Lcu_Ip_Hw_Access.d \
./RTD/src/Lcu_Ip_Irq.d \
./RTD/src/Lcu_Ip_Multicore.d \
./RTD/src/Lpspi_Ip.d \
./RTD/src/Lpspi_Ip_Irq.d \
./RTD/src/Lpuart_Uart_Ip.d \
./RTD/src/Lpuart_Uart_Ip_Irq.d \
./RTD/src/OsIf_Interrupts.d \
./RTD/src/OsIf_Timer.d \
./RTD/src/OsIf_Timer_System.d \
./RTD/src/OsIf_Timer_System_Internal_Systick.d \
./RTD/src/Pit_Ip.d \
./RTD/src/SchM_Adc.d \
./RTD/src/SchM_Dio.d \
./RTD/src/SchM_Gpt.d \
./RTD/src/SchM_Icu.d \
./RTD/src/SchM_Mcl.d \
./RTD/src/SchM_Mcu.d \
./RTD/src/SchM_Port.d \
./RTD/src/SchM_Pwm.d \
./RTD/src/SchM_Spi.d \
./RTD/src/SchM_Uart.d \
./RTD/src/Siul2_Dio_Ip.d \
./RTD/src/Siul2_Icu_Ip.d \
./RTD/src/Siul2_Icu_Ip_Irq.d \
./RTD/src/Siul2_Port_Ip.d \
./RTD/src/Trgmux_Ip.d \
./RTD/src/Trgmux_Ip_HwAcc.d \
./RTD/src/Tspc_Port_Ip.d 


# Each subdirectory must supply rules for building sources it contributes
RTD/src/%.o: ../RTD/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@RTD/src/Adc_Sar_Ip.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


