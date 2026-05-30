/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: May 27, 2026
 *      Author: julian
 */


#include "stm32f407xx_gpio_driver.h"

/*
 * Peripheral clock setup
 */


/********************************************************************************************
 * @fn						- GPIO_PeriClockControl
 *
 * @brief					- this function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port (e.g., GPIOA, GPIOB).
 * @param[in]				- EnorDi: Command to either ENABLE or DISABLE (macros) the clock.
 *
 * @return					- none
 *
 * @Note					- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE){
		if (pGPIOx == GPIOA) {
			GPIOA_PCLCK_EN();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLCK_EN();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLCK_EN();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLCK_EN();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLCK_EN();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLCK_EN();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLCK_EN();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLCK_EN();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLCK_EN();
		}
	} else {
		/*
		if (pGPIOx == GPIOA) {
			GPIOA_PCLCK_DI();
		} else if (pGPIOx == GPIOB) {
			GPIOB_PCLCK_DI();
		} else if (pGPIOx == GPIOC) {
			GPIOC_PCLCK_DI();
		} else if (pGPIOx == GPIOD) {
			GPIOD_PCLCK_DI();
		} else if (pGPIOx == GPIOE) {
			GPIOE_PCLCK_DI();
		} else if (pGPIOx == GPIOF) {
			GPIOF_PCLCK_DI();
		} else if (pGPIOx == GPIOG) {
			GPIOG_PCLCK_DI();
		} else if (pGPIOx == GPIOH) {
			GPIOH_PCLCK_DI();
		} else if (pGPIOx == GPIOI) {
			GPIOI_PCLCK_DI();
		}
			*/
	}
}

/*
 * Init and De-init
 */

/********************************************************************************************
 * @fn						- GPIO_Init
 *
 * @brief					- Initializes the specified GPIO pin(s) based on the configuration
 *                            settings provided in the handler structure.
 *
 * @param[in]				- pGPIOHandle: Pointer to the GPIO handle structure containing
 *                            the base address of the GPIO port and the pin configurations.
 *
 * @return					- none
 *
 * @Note					- The peripheral clock for the respective GPIO port must be
 *                            enabled before calling this function.
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	//1. configure the mode of gpio pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG) {
		//none interrupt mode
		//getting the pin mode 0-3 and then left shifted by calculation of the pin number * 2 because every pin
		//has 2 bits (e.g (1 << 10))
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		//clearing the bits before we set them just to be sure
		//0x3 = 11 negated = 00
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;
	} else {
		//Interrupt mode
		//Here we configure everything for the MCU peripheral side
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1.configure the Falling trigger suggestion register (FTSR)
			EXTI->FTSR |= (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1.configure the RTSR
			EXTI->RTSR |= (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR &= ~(1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		} else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//1. configure both FTSR and RTSR
			EXTI->RTSR |= (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. configure the GPIO port selection in SYSCFG_EXITCR
		//Here we calculate which EXTI register we need 0-4d
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		//here we calculate at which pin/bit we should start placing our value
		//it will be multiplied later with 4 because there are 4 bits to set in this register
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;

		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		//activate the clock for SYSCFG
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);

		//3. enable the EXITI interrupt delivery using IMR
		EXTI->IMR |= (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp = 0;

	//2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	temp = 0;

	//3. configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;

	//4. configure the optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	temp = 0;

	//5. configure the alt functionality
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN) {
		//configure the alt function register
		uint8_t temp1, temp2;

		//Here we check if we need to use element [0] one or element [1] two from the array
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
	}
}

/********************************************************************************************
 * @fn						- GPIO_DeInit
 *
 * @brief					- Resets all registers of the given GPIO port to their default
 *                            reset values.
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port to be reset.
 *
 * @return					- none
 *
 * @Note					- This function typically utilizes the RCC peripheral reset
 *                            registers (e.g., RCC_AHB1RSTR) to reset the entire port at once.
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	//Here we have to search in the Reference manual for the correct RCC peripheral reset register
	//then we turn the peripheral on (1) and then off again (0) so that it does not stay in
	//reset mode all the time
	if (pGPIOx == GPIOA) {
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB) {
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC) {
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD) {
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE) {
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOF) {
		GPIOF_REG_RESET();
	} else if (pGPIOx == GPIOG) {
		GPIOG_REG_RESET();
	} else if (pGPIOx == GPIOH) {
		GPIOH_REG_RESET();
	} else if (pGPIOx == GPIOI) {
		GPIOI_REG_RESET();
	}
}


/*
 * Data read and write
 */

/********************************************************************************************
 * @fn						- GPIO_ReadFromInputPin
 *
 * @brief					- Reads the digital logic state of a specific input pin on a given
 * 							  GPIO port.
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port (e.g., GPIOA, GPIOB).
 * @param[in]				- PinNumber: The specific pin number to read (0 to 15).
 *
 * @return					- The state of the pin, which is either 0 (LOW) or 1 (HIGH).
 *
 * @Note					- The pin must be configured in Input mode for this read to reflect
 *                            the external physical state accurately.
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	//Here we push the provided pin number, from the user, to the right (first bit position)
	//With & 0x00000001 we want to get the current bit status if 0 or 1
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

	return value;
}

/********************************************************************************************
 * @fn						- GPIO_ReadFromInputPort
 *
 * @brief					- Reads the digital logic states of all pins on a given GPIO port.
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port (e.g., GPIOA, GPIOB).
 *
 * @return					- A 16-bit value representing the status of all pins (Pins 0-15) on the port.
 *
 * @Note					- The returned value directly reflects the contents of the Input Data Register (IDR).
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}

/********************************************************************************************
 * @fn						- GPIO_WriteToOutputPin
 *
 * @brief					- Writes a digital logic state (HIGH or LOW) to a specific output
 * 							  pin on a given GPIO port.
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port (e.g., GPIOA, GPIOB).
 * @param[in]				- PinNumber: The specific pin number to write to (0 to 15).
 * @param[in]				- Value: The logic state to be written, using ENABLE/DISABLE or
 * 							  HIGH/LOW macros.
 *
 * @return					- none
 *
 * @Note					- The pin must be configured in Output mode for this write operation
 *                            to change the physical pin state.
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if (Value == GPIO_PIN_SET) {
		//write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx->ODR |= (1 << PinNumber);
	} else {
		//write 0
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/********************************************************************************************
 * @fn						- GPIO_WriteToOutputPort
 *
 * @brief					- Writes a 16-bit value to the entire output data register of a
 * 							  given GPIO port.
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port (e.g., GPIOA, GPIOB).
 * @param[in]				- Value: The 16-bit value containing the states for all pins (Pins 0-15).
 *
 * @return					- none
 *
 * @Note					- This operation overwrites the states of all pins on the port simultaneously.
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

/********************************************************************************************
 * @fn						- GPIO_ToggleOutputPin
 *
 * @brief					- Toggles the digital logic state of a specific output pin on a given
 * 							  GPIO port (HIGH/LOW).
 *
 * @param[in]				- pGPIOx: Base address of the GPIO peripheral port (e.g., GPIOA, GPIOB).
 * @param[in]				- PinNumber: The specific pin number to toggle (0 to 15).
 *
 * @return					- none
 *
 * @Note					- The pin should be configured in Output mode for this operation to
 * 							  have an effect.
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	//xor operator only accepts one true value
	//Current status (ODR):    1   0   1   0  (LED 3 is ON, LED 2 is OFF...)
	//XOR-mask (^):            0   1   0   0  (Command: "Only change the second position!")
	//-----------------------------------------
	//New value:           	   1   1   1   0
	//
	//Other example
	//
	//Current status (ODR):    1   1   1   0  (LED 3 is ON, LED 2 is OFF...)
	//XOR-mask (^):            0   1   0   0  (Command: "Only change the second position!")
	//-----------------------------------------
	//New value:               1   0   1   0
	pGPIOx->ODR ^= (1 << PinNumber);
}


/*
 * IRQ configuration and ISR handling
 */

/********************************************************************************************
 * @fn						- GPIO_IRQConfig
 *
 * @brief					- Configures the NVIC settings for a given IRQ number, including
 *                            enabling/disabling the interrupt and setting its priority.
 *
 * @param[in]				- IRQNumber: The specific Interrupt Request number to configure.
 * @param[in]				- IRQPriority: The priority level to assign to the interrupt.
 * @param[in]				- EnorDi: Command to either ENABLE or DISABLE (macros) the interrupt
 *							  in the NVIC.
 *
 * @return					- none
 *
 * @Note					- This function directly configures the processor's NVIC (Nested
 *                            Vectored Interrupt Controller) registers.
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	//here we define everything from the processor side for interrupts (NVIC)
	if (EnorDi == ENABLE)
	{
		if (IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= (1<<IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64)
		{
			//program ISER1 register
			*NVIC_ISER1 |= (1<<IRQNumber % 32);
		} else if (IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ISER2 register
			*NVIC_ISER2 |= (1<<IRQNumber % 64);
		}
	} else
	{
		if (IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= (1<<IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64)
		{
			//program ICER1 register
			*NVIC_ICER1 |= (1<<IRQNumber % 32);
		} else if (IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ICER2 register
			*NVIC_ICER2 |= (1<<IRQNumber % 64);
		}
	}
}

/********************************************************************************************
 * @fn						- GPIO_IRQPriorityConfig
 *
 * @brief					- Configures the priority of a specific interrupt request (IRQ)
 *                            in the ARM Cortex-M Nested Vectored Interrupt Controller (NVIC).
 *
 * @param[in]				- IRQNumber: The specific Interrupt Request number (0 to 239).
 * @param[in]				- IRQPriority: The priority value to be assigned (0 to 15 or 255,
 *                            depending on implemented bits).
 *
 * @return					- none
 *
 * @Note					- This function directly manipulates the NVIC_IPRx registers.
 *                            It accounts for the non-implemented lower bits in the priority
 *                            byte (microcontroller-specific shift via NO_PR_BITS_IMPLEMENTED).
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}


/********************************************************************************************
 * @fn						- GPIO_IRQHandling
 *
 * @brief					- Handles the interrupt processing for a specific GPIO pin by
 *                            clearing the corresponding pending bit in the EXTI register.
 *
 * @param[in]				- PinNumber: The specific pin number (0 to 15) that triggered the interrupt.
 *
 * @return					- none
 *
 * @Note					- This function must be called inside the respective ISR (Interrupt
 *                            Service Routine) in the startup code to acknowledge and clear the interrupt.
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//clear the exti pr register corresponding to the pin number
	if (EXTI->PR & (1<<PinNumber))
	{
		//clear
		EXTI->PR |= (1<<PinNumber);

	}
}
