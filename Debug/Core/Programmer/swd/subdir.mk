################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Programmer/swd/dap.c \
../Core/Programmer/swd/delay.c \
../Core/Programmer/swd/errors.c \
../Core/Programmer/swd/utils.c 

OBJS += \
./Core/Programmer/swd/dap.o \
./Core/Programmer/swd/delay.o \
./Core/Programmer/swd/errors.o \
./Core/Programmer/swd/utils.o 

C_DEPS += \
./Core/Programmer/swd/dap.d \
./Core/Programmer/swd/delay.d \
./Core/Programmer/swd/errors.d \
./Core/Programmer/swd/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Programmer/swd/%.o Core/Programmer/swd/%.su Core/Programmer/swd/%.cyclo: ../Core/Programmer/swd/%.c Core/Programmer/swd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Programmer -I../Core/Programmer/swd -I../Core/Programmer/target -I../Core/Programmer/util -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Programmer-2f-swd

clean-Core-2f-Programmer-2f-swd:
	-$(RM) ./Core/Programmer/swd/dap.cyclo ./Core/Programmer/swd/dap.d ./Core/Programmer/swd/dap.o ./Core/Programmer/swd/dap.su ./Core/Programmer/swd/delay.cyclo ./Core/Programmer/swd/delay.d ./Core/Programmer/swd/delay.o ./Core/Programmer/swd/delay.su ./Core/Programmer/swd/errors.cyclo ./Core/Programmer/swd/errors.d ./Core/Programmer/swd/errors.o ./Core/Programmer/swd/errors.su ./Core/Programmer/swd/utils.cyclo ./Core/Programmer/swd/utils.d ./Core/Programmer/swd/utils.o ./Core/Programmer/swd/utils.su

.PHONY: clean-Core-2f-Programmer-2f-swd

