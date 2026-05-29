/*
 * 002led_button.c
 *
 *  Created on: May 29, 2026
 *      Author: julian
 */


#include "stm32f407xx.h"

#define HIGH 1
#define BTN_PRESSED HIGH

//software delay
void delay(void)
{
	for (uint32_t i=0;i<300000;i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn;

	//Configuration for the LED

	GpioLed.pGPIOx = GPIOD;
	//The first LED on the STM32F4disc board is on PD12
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; //Push Pull Output type
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);

	//Configuration for the button

	GpioBtn.pGPIOx = GPIOA;
	//The first LED on the STM32F4disc board is on PD12
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GpioBtn);

	while(1)
	{
		//checking if button is pressed. Button is at Port A and Pin 0
		if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == BTN_PRESSED)
		{
			//wait for debouncing
			delay();
			GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
		}
	}

	return 0;
}
