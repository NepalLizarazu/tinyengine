################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/startup_stm32f746xx.s 

C_SRCS += \
../Src/CycleCounter.c \
../Src/MFCC.c \
../Src/arm_cfft_f32.c \
../Src/arm_cfft_init_f32.c \
../Src/arm_cfft_radix2_f32.c \
../Src/arm_cfft_radix8_f32.c \
../Src/arm_common_tables.c \
../Src/arm_const_structs.c \
../Src/arm_cos_f32.c \
../Src/arm_rfft_fast_f32.c \
../Src/arm_rfft_fast_init_f32.c \
../Src/audio_play.c \
../Src/audio_rec.c \
../Src/picojpeg.c \
../Src/stm32f7xx_hal_msp.c \
../Src/stm32f7xx_it.c \
../Src/system_stm32f7xx.c 

CPP_SRCS += \
../Src/JPEGDecoder.cpp \
../Src/camera.cpp \
../Src/camera_i2c.cpp \
../Src/camera_spi.cpp \
../Src/lcd.cpp \
../Src/main.cpp 

S_UPPER_SRCS += \
../Src/arm_bitreversal2.S 

S_DEPS += \
./Src/startup_stm32f746xx.d 

C_DEPS += \
./Src/CycleCounter.d \
./Src/MFCC.d \
./Src/arm_cfft_f32.d \
./Src/arm_cfft_init_f32.d \
./Src/arm_cfft_radix2_f32.d \
./Src/arm_cfft_radix8_f32.d \
./Src/arm_common_tables.d \
./Src/arm_const_structs.d \
./Src/arm_cos_f32.d \
./Src/arm_rfft_fast_f32.d \
./Src/arm_rfft_fast_init_f32.d \
./Src/audio_play.d \
./Src/audio_rec.d \
./Src/picojpeg.d \
./Src/stm32f7xx_hal_msp.d \
./Src/stm32f7xx_it.d \
./Src/system_stm32f7xx.d 

OBJS += \
./Src/CycleCounter.o \
./Src/JPEGDecoder.o \
./Src/MFCC.o \
./Src/arm_bitreversal2.o \
./Src/arm_cfft_f32.o \
./Src/arm_cfft_init_f32.o \
./Src/arm_cfft_radix2_f32.o \
./Src/arm_cfft_radix8_f32.o \
./Src/arm_common_tables.o \
./Src/arm_const_structs.o \
./Src/arm_cos_f32.o \
./Src/arm_rfft_fast_f32.o \
./Src/arm_rfft_fast_init_f32.o \
./Src/audio_play.o \
./Src/audio_rec.o \
./Src/camera.o \
./Src/camera_i2c.o \
./Src/camera_spi.o \
./Src/lcd.o \
./Src/main.o \
./Src/picojpeg.o \
./Src/startup_stm32f746xx.o \
./Src/stm32f7xx_hal_msp.o \
./Src/stm32f7xx_it.o \
./Src/system_stm32f7xx.o 

S_UPPER_DEPS += \
./Src/arm_bitreversal2.d 

CPP_DEPS += \
./Src/JPEGDecoder.d \
./Src/camera.d \
./Src/camera_i2c.d \
./Src/camera_spi.d \
./Src/lcd.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/CycleCounter.o: ../Src/CycleCounter.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/CycleCounter.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/JPEGDecoder.o: ../Src/JPEGDecoder.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/JPEGDecoder.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/MFCC.o: ../Src/MFCC.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/MFCC.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_bitreversal2.o: ../Src/arm_bitreversal2.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/arm_bitreversal2.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/arm_cfft_f32.o: ../Src/arm_cfft_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_cfft_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_cfft_init_f32.o: ../Src/arm_cfft_init_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_cfft_init_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_cfft_radix2_f32.o: ../Src/arm_cfft_radix2_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_cfft_radix2_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_cfft_radix8_f32.o: ../Src/arm_cfft_radix8_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_cfft_radix8_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_common_tables.o: ../Src/arm_common_tables.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_common_tables.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_const_structs.o: ../Src/arm_const_structs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_const_structs.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_cos_f32.o: ../Src/arm_cos_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_cos_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_rfft_fast_f32.o: ../Src/arm_rfft_fast_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_rfft_fast_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/arm_rfft_fast_init_f32.o: ../Src/arm_rfft_fast_init_f32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/arm_rfft_fast_init_f32.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/audio_play.o: ../Src/audio_play.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/audio_play.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/audio_rec.o: ../Src/audio_rec.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/audio_rec.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/camera.o: ../Src/camera.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/camera.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/camera_i2c.o: ../Src/camera_i2c.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/camera_i2c.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/camera_spi.o: ../Src/camera_spi.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/camera_spi.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/lcd.o: ../Src/lcd.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/lcd.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++11 -g -DSTM32F746xx -c -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I../Src -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/STM32746G-Discovery -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/picojpeg.o: ../Src/picojpeg.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/picojpeg.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/startup_stm32f746xx.o: ../Src/startup_stm32f746xx.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DSTM32F746xx -c -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/src -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/CMSIS/Device/ST/STM32F7xx/Include -IC:/Users/ASW-MNO-Admin/Atollic/TrueSTUDIO/ARM_videos/STM32F746G-Discovery/Drivers/STM32F7xx_HAL_Driver/Inc -x assembler-with-cpp -MMD -MP -MF"Src/startup_stm32f746xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/stm32f7xx_hal_msp.o: ../Src/stm32f7xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f7xx_hal_msp.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f7xx_it.o: ../Src/stm32f7xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f7xx_it.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f7xx.o: ../Src/system_stm32f7xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=c11 -g -DSTM32F746xx -c -I../Src -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis" -I"C:/Users/HP/Desktop/Master_Thesis/tinyengine/tutorial/TinyEngine_kws_0/Src/TinyEngine/codegen/Include" -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/BSP/STM32746G-Discovery -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../TinyEngine/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f7xx.d" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

