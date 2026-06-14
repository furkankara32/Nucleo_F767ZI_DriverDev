################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.c \
../Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.c \
../Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.c \
../Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.c \
../Drivers/MyDrivers/Src/stm32f767xx_spi_driver.c \
../Drivers/MyDrivers/Src/stm32f767xx_systick_driver.c \
../Drivers/MyDrivers/Src/stm32f767xx_usart_driver.c 

OBJS += \
./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.o \
./Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.o \
./Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.o \
./Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.o \
./Drivers/MyDrivers/Src/stm32f767xx_spi_driver.o \
./Drivers/MyDrivers/Src/stm32f767xx_systick_driver.o \
./Drivers/MyDrivers/Src/stm32f767xx_usart_driver.o 

C_DEPS += \
./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.d \
./Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.d \
./Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.d \
./Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.d \
./Drivers/MyDrivers/Src/stm32f767xx_spi_driver.d \
./Drivers/MyDrivers/Src/stm32f767xx_systick_driver.d \
./Drivers/MyDrivers/Src/stm32f767xx_usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MyDrivers/Src/%.o Drivers/MyDrivers/Src/%.su Drivers/MyDrivers/Src/%.cyclo: ../Drivers/MyDrivers/Src/%.c Drivers/MyDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32F767xx -c -I../Drivers/MyDrivers/Inc -I"D:/Nucleo_F767ZI_DriverDev/Nucleo_F767ZI_DriverDev/Examples/Inc" -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/Nucleo_F767ZI_DriverDev/Nucleo_F767ZI_DriverDev/Drivers/MyDrivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MyDrivers-2f-Src

clean-Drivers-2f-MyDrivers-2f-Src:
	-$(RM) ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_gpio_driver.su ./Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_i2c_driver.su ./Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_nvic_driver.su ./Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_rcc_driver.su ./Drivers/MyDrivers/Src/stm32f767xx_spi_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_spi_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_spi_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_spi_driver.su ./Drivers/MyDrivers/Src/stm32f767xx_systick_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_systick_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_systick_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_systick_driver.su ./Drivers/MyDrivers/Src/stm32f767xx_usart_driver.cyclo ./Drivers/MyDrivers/Src/stm32f767xx_usart_driver.d ./Drivers/MyDrivers/Src/stm32f767xx_usart_driver.o ./Drivers/MyDrivers/Src/stm32f767xx_usart_driver.su

.PHONY: clean-Drivers-2f-MyDrivers-2f-Src

