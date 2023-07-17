################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.c 

C_DEPS += \
./Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.d 

OBJS += \
./Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.o: ../Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/mfxstm32l152/mfxstm32l152.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

