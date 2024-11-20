################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Programmer/util/FileTransferCheck.c \
../Core/Programmer/util/button.c \
../Core/Programmer/util/buzzer.c \
../Core/Programmer/util/ihex_parser.c \
../Core/Programmer/util/led.c \
../Core/Programmer/util/w25qxx.c 

OBJS += \
./Core/Programmer/util/FileTransferCheck.o \
./Core/Programmer/util/button.o \
./Core/Programmer/util/buzzer.o \
./Core/Programmer/util/ihex_parser.o \
./Core/Programmer/util/led.o \
./Core/Programmer/util/w25qxx.o 

C_DEPS += \
./Core/Programmer/util/FileTransferCheck.d \
./Core/Programmer/util/button.d \
./Core/Programmer/util/buzzer.d \
./Core/Programmer/util/ihex_parser.d \
./Core/Programmer/util/led.d \
./Core/Programmer/util/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Programmer/util/%.o Core/Programmer/util/%.su Core/Programmer/util/%.cyclo: ../Core/Programmer/util/%.c Core/Programmer/util/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DDEBUG_CONFIG_DEBUG_MODE -DDEBUG_USE_IHEX_DEBUG -DDEBUG_USE_BUZZER_SOUND_SMALL -DDEBUG_USE_PRINTF -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Programmer -I../Core/Programmer/swd -I../Core/Programmer/target -I../Core/Programmer/util -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Programmer-2f-util

clean-Core-2f-Programmer-2f-util:
	-$(RM) ./Core/Programmer/util/FileTransferCheck.cyclo ./Core/Programmer/util/FileTransferCheck.d ./Core/Programmer/util/FileTransferCheck.o ./Core/Programmer/util/FileTransferCheck.su ./Core/Programmer/util/button.cyclo ./Core/Programmer/util/button.d ./Core/Programmer/util/button.o ./Core/Programmer/util/button.su ./Core/Programmer/util/buzzer.cyclo ./Core/Programmer/util/buzzer.d ./Core/Programmer/util/buzzer.o ./Core/Programmer/util/buzzer.su ./Core/Programmer/util/ihex_parser.cyclo ./Core/Programmer/util/ihex_parser.d ./Core/Programmer/util/ihex_parser.o ./Core/Programmer/util/ihex_parser.su ./Core/Programmer/util/led.cyclo ./Core/Programmer/util/led.d ./Core/Programmer/util/led.o ./Core/Programmer/util/led.su ./Core/Programmer/util/w25qxx.cyclo ./Core/Programmer/util/w25qxx.d ./Core/Programmer/util/w25qxx.o ./Core/Programmer/util/w25qxx.su

.PHONY: clean-Core-2f-Programmer-2f-util

