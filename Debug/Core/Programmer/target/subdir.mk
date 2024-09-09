################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Programmer/target/Target.c \
../Core/Programmer/target/stm32c0_flash.c \
../Core/Programmer/target/stm32h7_flash.c 

OBJS += \
./Core/Programmer/target/Target.o \
./Core/Programmer/target/stm32c0_flash.o \
./Core/Programmer/target/stm32h7_flash.o 

C_DEPS += \
./Core/Programmer/target/Target.d \
./Core/Programmer/target/stm32c0_flash.d \
./Core/Programmer/target/stm32h7_flash.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Programmer/target/%.o Core/Programmer/target/%.su Core/Programmer/target/%.cyclo: ../Core/Programmer/target/%.c Core/Programmer/target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Programmer -I../Core/Programmer/swd -I../Core/Programmer/target -I../Core/Programmer/util -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Programmer-2f-target

clean-Core-2f-Programmer-2f-target:
	-$(RM) ./Core/Programmer/target/Target.cyclo ./Core/Programmer/target/Target.d ./Core/Programmer/target/Target.o ./Core/Programmer/target/Target.su ./Core/Programmer/target/stm32c0_flash.cyclo ./Core/Programmer/target/stm32c0_flash.d ./Core/Programmer/target/stm32c0_flash.o ./Core/Programmer/target/stm32c0_flash.su ./Core/Programmer/target/stm32h7_flash.cyclo ./Core/Programmer/target/stm32h7_flash.d ./Core/Programmer/target/stm32h7_flash.o ./Core/Programmer/target/stm32h7_flash.su

.PHONY: clean-Core-2f-Programmer-2f-target

