/*
 * Stm32L053xx.h
 *
 *  Created on: Jul 11, 2026
 *      Author: admin
 */

#ifndef INC_STM32L053XX_H_
#define INC_STM32L053XX_H_

#include <stdint.h>

#define NVIC_ISER 						0xE000E100
#define NVIC_ICER						0xE000E180
#define NVIC_IPR						0xE000E400

#define FLASH_BASE_ADRESSE 				0X40022000U
#define RAM_BASE_ADRESS 				0X20000000U

#define GPIOA_BASE						0x50000000U
#define GPIOB_BASE						0x50000400U
#define GPIOC_BASE						0x50000800U
#define GPIOD_BASE						0x50000C00U
#define GPIOE_BASE						0x50001000U

#define USART1_BASE						0X40013800U

#define SYS_CONFIG_BASE					0X40010000U

#define EXTI_BASE						0X40010400U

#define RCC_BASE 						0X40021000U

#define EXTI_BASE 						0X40010400U

#define SPI1_BASE						0X40013000U
#define SPI2_BASE						0X40003800U

#define I2C1_BASE 						0X40005400U
#define I2C2_BASE						0X40005800U
#define I2C3_BASE						0X40007800U

#define ENABLE 			1
#define DISABLE 		0


// EXTI register structure

typedef struct{
	uint32_t EXTI_IMR;
	uint32_t EXTI_EMR;
	uint32_t EXTI_RTSR;
	uint32_t EXTI_FTSR;
	uint32_t EXTI_SWIER;
	uint32_t EXTI_PR;
}Exti_RegDef_t;

// GPIO register structure

typedef struct{
	uint32_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDR;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
	uint32_t BRR;
}Gpio_RegDef_t;


// SYSCONFIG register structure

typedef struct{
	uint32_t SYSCFG_CFGR1;
	uint32_t SYSCFG_CFGR2;
	uint32_t SYSCFG_EXTICR[4];
	uint32_t COMP1_CTRL[2];
	uint32_t SYSCFG_CFGR;

}SYSCONFIG_RegDef_t;

typedef struct{
	uint32_t SPI_CR1;
	uint32_t SPI_CR2;
	uint32_t SPI_SR;
	uint32_t SPI_DR;
	uint32_t SPI_CRCPR;
	uint32_t SPI_RXCRCR;
	uint32_t SPI_TXCRCR;
	uint32_t SPI_I2SCFGR;
	uint32_t SPI_I2SPR;
}Spi_RegDef_t;

// i2c rgister def

typedef struct {
	uint32_t I2C_CR1;
	uint32_t I2C_CR2;
	uint32_t I2C_OAR1;
	uint32_t I2C_OAR2;
	uint32_t I2C_TIMINGR;
	uint32_t I2C_TIMEOUTR;
	uint32_t I2C_ISR;
	uint32_t I2C_ICR;
	uint32_t I2C_PECR;
	uint32_t I2C_RXDR;
	uint32_t I2C_TXDR;

}I2C_RegDef_t;

#define I2C1    ((I2C_RegDef_t*)(I2C1_BASE))
#define I2C2    ((I2C_RegDef_t*)(I2C2_BASE))
#define I2C3    ((I2C_RegDef_t*)(I2C3_BASE))


#define SPI1   ((Spi_RegDef_t*)SPI1_BASE)
#define SPI2   ((Spi_RegDef_t*)SPI2_BASE)

#define GPIOA  ((Gpio_RegDef_t *)GPIOA_BASE)
#define GPIOB  ((Gpio_RegDef_t *)GPIOB_BASE)
#define GPIOC  ((Gpio_RegDef_t *)GPIOC_BASE)
#define GPIOD  ((Gpio_RegDef_t *)GPIOD_BASE)
#define GPIOE  ((Gpio_RegDef_t *)GPIOE_BASE)

#define EXTI   ((Exti_RegDef_t*)EXTI_BASE)

#define SYSCONFIG   ((SYSCONFIG_RegDef_t *)SYS_CONFIG_BASE)

#define GPIO_BASE_ADDR(x)      (x == GPIOA) ? 0x0 :\
                                (x == GPIOB) ? 0x1 :\
                                (x == GPIOC) ? 0x2 :\
                                (x == GPIOD) ? 0x3 :\
                                (x == GPIOE) ? 0x4 : 6\

typedef struct{
	uint32_t RCC_CR;
	uint32_t RESERVED1;
	uint32_t RESERVED2;
	uint32_t RCC_CFGR;
	uint32_t RESERVED4;
	uint32_t RESERVED5;
	uint32_t RESERVED6;
	uint32_t RCC_IOPRSTR;
	uint32_t RESERVED8;
	uint32_t RESERVED9;
	uint32_t RESERVED10;
	volatile uint32_t RCC_IOPENR;
	uint32_t RESERVED12;
	volatile uint32_t RCC_APB2ENR;
	volatile uint32_t RCC_APB1ENR;
}RCC_struc_t;


// RCC var

#define RCC_CTR 	   ((RCC_struc_t *)RCC_BASE)

// enabling gpio
#define RCC_EN_GPIOA() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 0))
#define RCC_EN_GPIOB() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 1))
#define RCC_EN_GPIOC() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 2))
#define RCC_EN_GPIOD() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 3))
#define RCC_EN_GPIOE() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 4))

// enabling sysconfig
#define RCC_EN_Sysconfig() (((RCC_struc_t*)RCC_BASE) -> RCC_APB2ENR |= (0x01 << 0))
// reset gpio register

#define RCC_RESET_GPIOA()  do {(((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR |= (0x01 << 0)) ; (((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR &= ~(0x01 << 0))} while(0) ;
#define RCC_RESET_GPIOB()  do {(((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR |= (0x01 << 1)) ; (((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR &= ~(0x01 << 1))} while(0) ;
#define RCC_RESET_GPIOC()  do {(((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR |= (0x01 << 2)) ; (((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR &= ~(0x01 << 2))} while(0) ;
#define RCC_RESET_GPIOD()  do {(((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR |= (0x01 << 3)) ; (((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR &= ~(0x01 << 3))} while(0) ;
#define RCC_RESET_GPIOE()  do {(((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR |= (0x01 << 4)) ; (((RCC_struc_t *)RCC_BASE) -> RCC_IOPRSTR &= ~(0x01 << 4))} while(0) ;
//////

#define RCC_DI_GPIOA() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 0))
#define RCC_DI_GPIOB() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 0))
#define RCC_DI_GPIOC() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 0))
#define RCC_DI_GPIOD() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 0))
#define RCC_DI_GPIOE() (((RCC_struc_t *)RCC_BASE) -> RCC_IOPENR |= (0x01 << 0))


//enabling sysconfig rcc
#define RCC_EN_SYSCONFIG() ((RCC_struc_t*)RCC_BASE) -> RCC_APB2ENR |= (0x01 << 0)


//enabling spi

#define RCC_EN_SPI_1() ((RCC_struc_t*)RCC_BASE) -> RCC_APB2ENR |= (0x01 << 12)
#define RCC_EN_SPI_2() ((RCC_struc_t*)RCC_BASE) -> RCC_APB1ENR |= (0x01 << 14)

// enabling i2c

#define I2C1_R						21
#define I2C2_R						22
#define I2C3_R						30

#define RCC_EN_I2C1()		(((RCC_struc_t*)RCC_BASE) -> RCC_APB1ENR |= (0x01 << I2C1_R))
#define RCC_EN_I2C2()		(((RCC_struc_t*)RCC_BASE) -> RCC_APB1ENR |= (0x01 << I2C2_R))
#define RCC_EN_I2C3()		(((RCC_struc_t*)RCC_BASE) -> RCC_APB1ENR |= (0x01 << I2C3_R))

// exti interrupt

#define IRQ_N_EXTI_0_1		 5
#define IRQ_N_EXTI_2_3		 6
#define IRQ_N_EXTI_4_15		 7
#define IRQ_PRIO_PADDING	 6

// i2c interrupt

#define I2C_1_INTERRUPT		23
#define I2C_2_INTERRUPT		24
#define I2C_3_INTERRUPT		21

// spi interrupt

#define SPI_1_INTERRUPT 	25
#define SPI_2_INTERRUPT 	26



#endif /* INC_STM32L053XX_H_ */
