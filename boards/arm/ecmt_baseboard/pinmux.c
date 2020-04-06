/*
 * Copyright (c) 2020 Ecomott, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <device.h>
#include <init.h>
#include <drivers/gpio.h>
#include <drivers/pinmux.h>
#include <sys/sys_io.h>

#include <pinmux/stm32/pinmux_stm32.h>

#define GPIO_LED_CONTROLLER	DT_ALIAS_LED1_GPIOS_CONTROLLER
#define GPIO_LED_POWER_PIN	DT_ALIAS_LED1_GPIOS_PIN

static const struct pin_config pinconf[] = {
#ifdef CONFIG_UART_1
	{STM32_PIN_PA9, STM32L1X_PINMUX_FUNC_PA9_USART1_TX},
	{STM32_PIN_PA10, STM32L1X_PINMUX_FUNC_PA10_USART1_RX},
#endif /* CONFIG_UART_1 */
#ifdef CONFIG_UART_2
	{STM32_PIN_PA2, STM32L1X_PINMUX_FUNC_PA2_USART2_TX},
	{STM32_PIN_PA3, STM32L1X_PINMUX_FUNC_PA3_USART2_RX},
#endif /* CONFIG_UART_2 */
#ifdef CONFIG_USB_DC_STM32
	{STM32_PIN_PA11, STM32L1X_PINMUX_FUNC_PA11_USB_DM},
	{STM32_PIN_PA12, STM32L1X_PINMUX_FUNC_PA12_USB_DP},
#endif /* CONFIG_USB_DC_STM32 */
#ifdef CONFIG_I2C_1
	{STM32_PIN_PB8, STM32L1X_PINMUX_FUNC_PB8_I2C1_SCL},
	{STM32_PIN_PB9, STM32L1X_PINMUX_FUNC_PB9_I2C1_SDA},
#endif /* CONFIG_I2C_1 */
#ifdef CONFIG_I2C_2
	{STM32_PIN_PB10, STM32L1X_PINMUX_FUNC_PB10_I2C2_SCL},
	{STM32_PIN_PB11, STM32L1X_PINMUX_FUNC_PB11_I2C2_SDA},
#endif /* CONFIG_I2C_2*/
#ifdef CONFIG_ADC_1
	{STM32_PIN_PA4, STM32L1X_PINMUX_FUNC_PA4_ADC1_IN4},
	{STM32_PIN_PA5, STM32L1X_PINMUX_FUNC_PA5_ADC1_IN5},
#endif /* CONFIG_ADC_1 */
};

static int pinmux_stm32_init(struct device *port)
{
	ARG_UNUSED(port);

	struct device *gpioa = device_get_binding(GPIO_LED_CONTROLLER);

	stm32_setup_pins(pinconf, ARRAY_SIZE(pinconf));

	gpio_pin_configure(gpioa, GPIO_LED_POWER_PIN, GPIO_OUTPUT);
	gpio_pin_set(gpioa, GPIO_LED_POWER_PIN, 1);

	return 0;
}

SYS_INIT(pinmux_stm32_init, PRE_KERNEL_1,
	 CONFIG_PINMUX_STM32_DEVICE_INITIALIZATION_PRIORITY);
