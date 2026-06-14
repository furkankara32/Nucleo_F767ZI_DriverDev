################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.c 

OBJS += \
./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.o 

C_DEPS += \
./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MyDrivers/Src/%.o Drivers/MyDrivers/Src/%.su Drivers/MyDrivers/Src/%.cyclo: ../Drivers/MyDrivers/Src/%.c Drivers/MyDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MyDrivers-2f-Src

clean-Drivers-2f-MyDrivers-2f-Src:
	-$(RM) ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.su

.PHONY: clean-Drivers-2f-MyDrivers-2f-Src

