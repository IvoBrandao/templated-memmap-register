################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/syscalls.c \
../Src/sysmem.c 

CPP_SRCS += \
../Src/Systick.cpp \
../Src/Systick_test.cpp \
../Src/main.cpp 

C_DEPS += \
./Src/syscalls.d \
./Src/sysmem.d 

OBJS += \
./Src/Systick.o \
./Src/Systick_test.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

CPP_DEPS += \
./Src/Systick.d \
./Src/Systick_test.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=c++17 -g3 -DDEBUG -DSTM32H743ZITx -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -c -I../Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32H743ZITx -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

