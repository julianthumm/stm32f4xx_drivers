/*
 * stm32f407xx.h
 *
 *  Created on: May 27, 2026
 *      Author: julian
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo	volatile

/*
 * base addresses of flash and SRAM memories
 */
#define FLASH_BASEADDR				0x08000000U //Flash storage of the board
#define SRAM1_BASEADDR				0x20000000U //112KB
#define SRAM2_BASEADDR				0x20001C00U //112KB in Hex then 0x20000000 + offset (112KB = 1C00)
#define ROM_BASEADDR				0xfFFF0000U //ROM = System memory -> RM Embedded flash memory interface
#define SRAM 						SRAM1_BASEADDR


/*
 * AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASEADDR				0x40000000U
#define APB1PERIPH_BASEADDR			PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR			0x40010000U
#define AHB1PERIPH_BASEADDR			0x40020000U
#define AHB2PERIPH_BASEADDR			0x50000000U

/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */

#define GPIOA_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR				(AHB1PERIPH_BASEADDR + 0x2000)
#define RCC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x3800)

/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */

#define I2C1_BASEADDR				(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR				(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR				(APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR				(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR				(APB1PERIPH_BASEADDR + 0x3C00)

//difference between USART and UART is that USART supports synchronous communication and UART not
#define USART2_BASEADDR				(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR				(APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR				(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR				(APB1PERIPH_BASEADDR + 0x5000)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */

#define EXTI_BASEADDR				(APB2PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASEADDR				(APB2PERIPH_BASEADDR + 0x3000)
#define SYSCFG_BASEADDR				(APB2PERIPH_BASEADDR + 0x3800)
#define USART1_BASEADDR				(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR				(APB2PERIPH_BASEADDR + 0x1400)





/********************************************peripheral register definition structures*****************************************/

//Defining a structure for GPIO register
typedef struct
{
	__vo uint32_t MODER;	/*GPIO port mode register						Address offset: 0x00*/
	__vo uint32_t OTYPER;	/*GPIO port output type register				Address offset: 0x04*/
	__vo uint32_t OSPEEDR;	/*GPIO port output speed register				Address offset: 0x08*/
	__vo uint32_t PUPDR;	/*GPIO port pull-up/pull-down register			Address offset: 0x0C*/
	__vo uint32_t IDR;		/*GPIO port input data register					Address offset: 0x10*/
	__vo uint32_t ODR;		/*GPIO port output data register				Address offset: 0x14*/
	__vo uint32_t BSRR;		/*GPIO port bit set/reset register				Address offset: 0x18*/
	__vo uint32_t LCKR;		/*GPIO port configuration lock register			Address offset: 0x1C*/
	__vo uint32_t AFR[2];	/*GPIO alternate function low/high register		Address offset: 0x20 & 0x24*/
} GPIO_RegDef_t;

//Defining a structure for Reset and clock control registers
typedef struct
{
	__vo uint32_t CR;			/*RCC clock control register 										Address offset: 0x00*/
	__vo uint32_t PLLCFGR;		/*RCC PLL configuration register 									Address offset: 0x04*/
	__vo uint32_t CFGR;			/*RCC clock configuration register 									Address offset: 0x08*/
	__vo uint32_t CIR;			/*RCC clock interrupt register 										Address offset: 0x0C*/
	__vo uint32_t AHB1RSTR;		/*RCC AHB1 peripheral reset register 								Address offset: 0x10*/
	__vo uint32_t AHB2RSTR;		/*RCC AHB2 peripheral reset register 								Address offset: 0x14*/
	__vo uint32_t AHB3RSTR;		/*RCC AHB3 peripheral reset register 								Address offset: 0x18*/
	uint32_t RESERVED0;			/*																	Address offset: 0x1C*/
	__vo uint32_t APB1RSTR;		/*RCC APB1 peripheral reset register 								Address offset: 0x20*/
	__vo uint32_t APB2RSTR;		/*RCC APB2 peripheral reset register 								Address offset: 0x24*/
	uint32_t RESERVED1[2];		/* 																	Address offset: 0x28 & 0x2C*/
	__vo uint32_t AHB1ENR;		/*RCC AHB1 peripheral clock enable register 						Address offset: 0x30*/
	__vo uint32_t AHB2ENR;		/*RCC AHB2 peripheral clock enable register 						Address offset: 0x34*/
	__vo uint32_t AHB3ENR;		/*RCC AHB3 peripheral clock enable register 						Address offset: 0x38*/
	uint32_t RESERVED2;			/*																	Address offset: 0x3C*/
	__vo uint32_t APB1ENR;		/*RCC APB1 peripheral clock enable register 						Address offset: 0x40*/
	__vo uint32_t APB2ENR;		/*RCC APB2 peripheral clock enable register 						Address offset: 0x44*/
	uint32_t RESERVED3[2];		/*																	Address offset: 0x48 & 0x4C*/
	__vo uint32_t AHB1LPENR;	/*RCC AHB1 peripheral clock enable in low power mode register 		Address offset: 0x50*/
	__vo uint32_t AHB2LPENR;	/*RCC AHB2 peripheral clock enable in low power mode register 		Address offset: 0x54*/
	__vo uint32_t AHB3LPENR;	/*RCC AHB3 peripheral clock enable in low power mode register 		Address offset: 0x58*/
	uint32_t RESERVED4;			/*																	Address offset: 0x5C*/
	__vo uint32_t APB1LPENR;	/*RCC APB1 peripheral clock enable in low power mode register 		Address offset: 0x60*/
	__vo uint32_t APB2LPENR;	/*RCC APB2 peripheral clock enabled in low power mode register 		Address offset: 0x64*/
	uint32_t RESERVED5[2];		/*																	Address offset: 0x68 & 0x6C*/
	__vo uint32_t BDCR;			/*RCC Backup domain control register 								Address offset: 0x70*/
	__vo uint32_t CSR;			/*RCC clock control & status register 								Address offset: 0x74*/
	uint32_t RESERVED6[2];		/*																	Address offset: 0x78 & 0x7C*/
	__vo uint32_t SSCGR;		/*RCC spread spectrum clock generation register 					Address offset: 0x80*/
	__vo uint32_t PLLI2SCFGR;	/*RCC PLLI2S configuration register 								Address offset: 0x04*/
} RCC_Reg_Def_t;

/*
 * peripheral definitions (Peripheral base addresses type casted to xxx_RegDef_t)
 */

#define GPIOA	((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB	((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC	((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD	((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE	((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF	((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG	((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH	((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI	((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC ((RCC_Reg_Def_t*)RCC_BASEADDR)

/*
 * Clock enable macros for GPIOx peripherals
 */

//Accessing RCC AHB1 peripheral clock enable register and turning on the clock for GPIOxEN
#define GPIOA_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLCK_EN()	(RCC->AHB1ENR |= (1 << 8))

/*
 * Clock enable macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN() 		(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN() 		(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN() 		(RCC->APB1ENR |= (1 << 23))


/*
 * Clock enable macros for SPIx peripherals
 */

#define SPI1_PCLK_EN()		(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()		(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()		(RCC->APB2ENR |= (1 << 15))

/*
 * Clock enable macros for USARTx peripherals
 */
#define USART1_PCLK_EN()		(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()		(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()		(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()			(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()			(RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()		(RCC->APB2ENR |= (1 << 5))

/*
 * Clock enable macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN()		(RCC->APB2ENR |= (1 << 14))


/*
 * Clock disable macros for GPIOx peripherals
 */
//here we use &= ~() to clear a bit to put it at 0 instead of 1
#define GPIOA_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLCK_DI()	(RCC->AHB1ENR &= ~(1 << 8))

/*
 * Clock disable macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI() 		(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI() 		(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI() 		(RCC->APB1ENR &= ~(1 << 23))


/*
 * Clock disable macros for SPIx peripherals
 */
#define SPI1_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 15))

/*
 * Clock disable macros for USARTx peripherals
 */
#define USART1_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 5))

/*
 * Clock disable macros for SYSCFG peripherals
 */
#define SYSCFG_PCLK_DI()		(RCC->APB2ENR &= ~(1 << 14))

/*
 * Macros to reset GPIOx peripherals
 * We will set 1 for resetting and then turn it back to 0
 * We use a do while loop in macro to call 2 statements in one macro
 */
#define GPIOA_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 0));	(RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 1));	(RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 2));	(RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 3));	(RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 4));	(RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 5));	(RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 6));	(RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 7));	(RCC->AHB1RSTR &= ~(1 << 7)); } while(0)
#define GPIOI_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 8));	(RCC->AHB1RSTR &= ~(1 << 8)); } while(0)

//Some generic macros
#define ENABLE 			1
#define DISABLE 		0
#define SET 			ENABLE
#define RESET 			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET

























#endif /* INC_STM32F407XX_H_ */
