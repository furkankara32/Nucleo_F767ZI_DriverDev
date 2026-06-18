################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Examples/Src/ex0001_gpio_led_blink.c \
../Examples/Src/ex002_gpio_led_set_reset.c \
../Examples/Src/ex003_gpio_button_input.c \
../Examples/Src/ex004_gpio_deinit_test.c \
../Examples/Src/ex005_gpio_button_interrupt.c \
../Examples/Src/ex006_systick_led_blink.c \
../Examples/Src/ex007_usart3_tx.c \
../Examples/Src/ex008_usart3_rx_echo.c \
../Examples/Src/ex009_usart3_it_rx_echo.c \
../Examples/Src/ex010_usart3_packet_10byte.c \
../Examples/Src/ex011_usart3_packet_parser.c \
../Examples/Src/ex012_spi1_loopback.c \
../Examples/Src/ex013_spi1_loopback_with_cs.c \
../Examples/Src/ex014_spi1_send_receive_test.c \
../Examples/Src/ex015_i2c1_init_test.c \
../Examples/Src/ex017_i2c1_scanner_test.c \
../Examples/Src/ex018_i2c1_ssd1306_command_test.c \
../Examples/Src/ex020_usart3_idle_rx_test.c \
../Examples/Src/ex021_spi1_loopback_it_test.c \
../Examples/Src/ex022_spi1_send_it_test.c \
../Examples/Src/ex023_spi1_receive_it_test.c \
../Examples/Src/ex024_i2c1_it_nack_test.c \
../Examples/Src/ex025_i2c1_it_rx_nack_test.c \
../Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.c \
../Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.c \
../Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.c \
../Examples/Src/ex029_i2c1_it_write_read_nack_test.c 

OBJS += \
./Examples/Src/ex0001_gpio_led_blink.o \
./Examples/Src/ex002_gpio_led_set_reset.o \
./Examples/Src/ex003_gpio_button_input.o \
./Examples/Src/ex004_gpio_deinit_test.o \
./Examples/Src/ex005_gpio_button_interrupt.o \
./Examples/Src/ex006_systick_led_blink.o \
./Examples/Src/ex007_usart3_tx.o \
./Examples/Src/ex008_usart3_rx_echo.o \
./Examples/Src/ex009_usart3_it_rx_echo.o \
./Examples/Src/ex010_usart3_packet_10byte.o \
./Examples/Src/ex011_usart3_packet_parser.o \
./Examples/Src/ex012_spi1_loopback.o \
./Examples/Src/ex013_spi1_loopback_with_cs.o \
./Examples/Src/ex014_spi1_send_receive_test.o \
./Examples/Src/ex015_i2c1_init_test.o \
./Examples/Src/ex017_i2c1_scanner_test.o \
./Examples/Src/ex018_i2c1_ssd1306_command_test.o \
./Examples/Src/ex020_usart3_idle_rx_test.o \
./Examples/Src/ex021_spi1_loopback_it_test.o \
./Examples/Src/ex022_spi1_send_it_test.o \
./Examples/Src/ex023_spi1_receive_it_test.o \
./Examples/Src/ex024_i2c1_it_nack_test.o \
./Examples/Src/ex025_i2c1_it_rx_nack_test.o \
./Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.o \
./Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.o \
./Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.o \
./Examples/Src/ex029_i2c1_it_write_read_nack_test.o 

C_DEPS += \
./Examples/Src/ex0001_gpio_led_blink.d \
./Examples/Src/ex002_gpio_led_set_reset.d \
./Examples/Src/ex003_gpio_button_input.d \
./Examples/Src/ex004_gpio_deinit_test.d \
./Examples/Src/ex005_gpio_button_interrupt.d \
./Examples/Src/ex006_systick_led_blink.d \
./Examples/Src/ex007_usart3_tx.d \
./Examples/Src/ex008_usart3_rx_echo.d \
./Examples/Src/ex009_usart3_it_rx_echo.d \
./Examples/Src/ex010_usart3_packet_10byte.d \
./Examples/Src/ex011_usart3_packet_parser.d \
./Examples/Src/ex012_spi1_loopback.d \
./Examples/Src/ex013_spi1_loopback_with_cs.d \
./Examples/Src/ex014_spi1_send_receive_test.d \
./Examples/Src/ex015_i2c1_init_test.d \
./Examples/Src/ex017_i2c1_scanner_test.d \
./Examples/Src/ex018_i2c1_ssd1306_command_test.d \
./Examples/Src/ex020_usart3_idle_rx_test.d \
./Examples/Src/ex021_spi1_loopback_it_test.d \
./Examples/Src/ex022_spi1_send_it_test.d \
./Examples/Src/ex023_spi1_receive_it_test.d \
./Examples/Src/ex024_i2c1_it_nack_test.d \
./Examples/Src/ex025_i2c1_it_rx_nack_test.d \
./Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.d \
./Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.d \
./Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.d \
./Examples/Src/ex029_i2c1_it_write_read_nack_test.d 


# Each subdirectory must supply rules for building sources it contributes
Examples/Src/%.o Examples/Src/%.su Examples/Src/%.cyclo: ../Examples/Src/%.c Examples/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32F767xx -c -I../Drivers/MyDrivers/Inc -I"D:/Nucleo_F767ZI_DriverDev/Nucleo_F767ZI_DriverDev/Examples/Inc" -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/Nucleo_F767ZI_DriverDev/Nucleo_F767ZI_DriverDev/Drivers/MyDrivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Examples-2f-Src

clean-Examples-2f-Src:
	-$(RM) ./Examples/Src/ex0001_gpio_led_blink.cyclo ./Examples/Src/ex0001_gpio_led_blink.d ./Examples/Src/ex0001_gpio_led_blink.o ./Examples/Src/ex0001_gpio_led_blink.su ./Examples/Src/ex002_gpio_led_set_reset.cyclo ./Examples/Src/ex002_gpio_led_set_reset.d ./Examples/Src/ex002_gpio_led_set_reset.o ./Examples/Src/ex002_gpio_led_set_reset.su ./Examples/Src/ex003_gpio_button_input.cyclo ./Examples/Src/ex003_gpio_button_input.d ./Examples/Src/ex003_gpio_button_input.o ./Examples/Src/ex003_gpio_button_input.su ./Examples/Src/ex004_gpio_deinit_test.cyclo ./Examples/Src/ex004_gpio_deinit_test.d ./Examples/Src/ex004_gpio_deinit_test.o ./Examples/Src/ex004_gpio_deinit_test.su ./Examples/Src/ex005_gpio_button_interrupt.cyclo ./Examples/Src/ex005_gpio_button_interrupt.d ./Examples/Src/ex005_gpio_button_interrupt.o ./Examples/Src/ex005_gpio_button_interrupt.su ./Examples/Src/ex006_systick_led_blink.cyclo ./Examples/Src/ex006_systick_led_blink.d ./Examples/Src/ex006_systick_led_blink.o ./Examples/Src/ex006_systick_led_blink.su ./Examples/Src/ex007_usart3_tx.cyclo ./Examples/Src/ex007_usart3_tx.d ./Examples/Src/ex007_usart3_tx.o ./Examples/Src/ex007_usart3_tx.su ./Examples/Src/ex008_usart3_rx_echo.cyclo ./Examples/Src/ex008_usart3_rx_echo.d ./Examples/Src/ex008_usart3_rx_echo.o ./Examples/Src/ex008_usart3_rx_echo.su ./Examples/Src/ex009_usart3_it_rx_echo.cyclo ./Examples/Src/ex009_usart3_it_rx_echo.d ./Examples/Src/ex009_usart3_it_rx_echo.o ./Examples/Src/ex009_usart3_it_rx_echo.su ./Examples/Src/ex010_usart3_packet_10byte.cyclo ./Examples/Src/ex010_usart3_packet_10byte.d ./Examples/Src/ex010_usart3_packet_10byte.o ./Examples/Src/ex010_usart3_packet_10byte.su ./Examples/Src/ex011_usart3_packet_parser.cyclo ./Examples/Src/ex011_usart3_packet_parser.d ./Examples/Src/ex011_usart3_packet_parser.o ./Examples/Src/ex011_usart3_packet_parser.su ./Examples/Src/ex012_spi1_loopback.cyclo ./Examples/Src/ex012_spi1_loopback.d ./Examples/Src/ex012_spi1_loopback.o ./Examples/Src/ex012_spi1_loopback.su ./Examples/Src/ex013_spi1_loopback_with_cs.cyclo ./Examples/Src/ex013_spi1_loopback_with_cs.d ./Examples/Src/ex013_spi1_loopback_with_cs.o ./Examples/Src/ex013_spi1_loopback_with_cs.su ./Examples/Src/ex014_spi1_send_receive_test.cyclo ./Examples/Src/ex014_spi1_send_receive_test.d ./Examples/Src/ex014_spi1_send_receive_test.o ./Examples/Src/ex014_spi1_send_receive_test.su ./Examples/Src/ex015_i2c1_init_test.cyclo ./Examples/Src/ex015_i2c1_init_test.d ./Examples/Src/ex015_i2c1_init_test.o ./Examples/Src/ex015_i2c1_init_test.su ./Examples/Src/ex017_i2c1_scanner_test.cyclo ./Examples/Src/ex017_i2c1_scanner_test.d ./Examples/Src/ex017_i2c1_scanner_test.o ./Examples/Src/ex017_i2c1_scanner_test.su ./Examples/Src/ex018_i2c1_ssd1306_command_test.cyclo ./Examples/Src/ex018_i2c1_ssd1306_command_test.d ./Examples/Src/ex018_i2c1_ssd1306_command_test.o ./Examples/Src/ex018_i2c1_ssd1306_command_test.su ./Examples/Src/ex020_usart3_idle_rx_test.cyclo ./Examples/Src/ex020_usart3_idle_rx_test.d ./Examples/Src/ex020_usart3_idle_rx_test.o ./Examples/Src/ex020_usart3_idle_rx_test.su ./Examples/Src/ex021_spi1_loopback_it_test.cyclo ./Examples/Src/ex021_spi1_loopback_it_test.d ./Examples/Src/ex021_spi1_loopback_it_test.o ./Examples/Src/ex021_spi1_loopback_it_test.su ./Examples/Src/ex022_spi1_send_it_test.cyclo ./Examples/Src/ex022_spi1_send_it_test.d ./Examples/Src/ex022_spi1_send_it_test.o ./Examples/Src/ex022_spi1_send_it_test.su ./Examples/Src/ex023_spi1_receive_it_test.cyclo ./Examples/Src/ex023_spi1_receive_it_test.d ./Examples/Src/ex023_spi1_receive_it_test.o ./Examples/Src/ex023_spi1_receive_it_test.su ./Examples/Src/ex024_i2c1_it_nack_test.cyclo ./Examples/Src/ex024_i2c1_it_nack_test.d ./Examples/Src/ex024_i2c1_it_nack_test.o ./Examples/Src/ex024_i2c1_it_nack_test.su ./Examples/Src/ex025_i2c1_it_rx_nack_test.cyclo ./Examples/Src/ex025_i2c1_it_rx_nack_test.d ./Examples/Src/ex025_i2c1_it_rx_nack_test.o ./Examples/Src/ex025_i2c1_it_rx_nack_test.su ./Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.cyclo ./Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.d ./Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.o ./Examples/Src/ex026_i2c1_it_tx_to_i2c2_slave_test.su ./Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.cyclo ./Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.d ./Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.o ./Examples/Src/ex027_i2c1_it_rx_from_i2c2_slave_test.su ./Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.cyclo ./Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.d ./Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.o ./Examples/Src/ex028_i2c1_it_write_read_i2c2_slave_test.su ./Examples/Src/ex029_i2c1_it_write_read_nack_test.cyclo ./Examples/Src/ex029_i2c1_it_write_read_nack_test.d ./Examples/Src/ex029_i2c1_it_write_read_nack_test.o ./Examples/Src/ex029_i2c1_it_write_read_nack_test.su

.PHONY: clean-Examples-2f-Src

