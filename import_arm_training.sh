#!/bin/bash
mkdir ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp -r ./TinyEngine/third_party/CMSIS/CMSIS/NN/Include/*.h ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp -r ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/dsp ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_common_tables.h ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_math_memory.h ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_math_types.h ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp ./TinyEngine/third_party/CMSIS/CMSIS/DSP/Include/arm_math.h ./tutorial/TinyEngine_kws_0/Src/TinyEngine/include/arm_cmsis
cp -r ./TinyEngine/third_party/CMSIS/CMSIS/Core/Include ./tutorial/TinyEngine_kws_0/Drivers/CMSIS