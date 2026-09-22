/*
 * stm32L053xx_drivers.h
 *
 *  Created on: Jul 12, 2026
 *      Author: admin
 */

#ifndef STM32L053XX_DRIVERS_H_
#define STM32L053XX_DRIVERS_H_

#include "Stm32L053xx.h"


typedef struct{
	uint8_t GPIO_pinNumber;
	uint8_t GPIO_pinMode;
	uint8_t GPIO_pin_speed;
	uint8_t PinPupdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_AltFunMode;
}GPIO_PinConfig_t;

typedef struct{
	Gpio_RegDef_t * pGPIO;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;



// GPIO modes MACROS
#define PIN_NUMBERS 			15
#define GPIO_MODE_IN 			0
#define GPIO_MODE_OUT 			1
#define GPIO_MODE_ALT  			2
#define GPIO_MODE_ANALOG 		3
#define GPIO_MODE_IT_FT			4
#define GPIO_MODE_IT_RT			5
#define GPIO_MODE_IT_RFT		6


// GPIO pin Possible output type

#define GPIO_OP_TYPE_PP 		0
#define GPIO_OP_TYPE_OD 		1

// GPIO pin Output speed

#define GPIO_SPEED_LOW      0
#define GPIO_SPEED_MEDIUM   1
#define GPIO_SPEED_FAST     2
#define GPIO_SPEED_HIGH     3

// pull up and pull down

#define GPIO_NO_PUPD    0
#define GPIO_PU         1
#define GPIO_PD         2

// pin numbers

#define GPIO_PIN_NO_0     0
#define GPIO_PIN_NO_1     1
#define GPIO_PIN_NO_2     2
#define GPIO_PIN_NO_3     3
#define GPIO_PIN_NO_4     4
#define GPIO_PIN_NO_5     5
#define GPIO_PIN_NO_6     6
#define GPIO_PIN_NO_7     7
#define GPIO_PIN_NO_8     8
#define GPIO_PIN_NO_9     9
#define GPIO_PIN_NO_10    10
#define GPIO_PIN_NO_11    11
#define GPIO_PIN_NO_12    12
#define GPIO_PIN_NO_13    13
#define GPIO_PIN_NO_14    14
#define GPIO_PIN_NO_15    15

//////////Alt function ///////////////

#define AF0					0
#define AF1					1
#define AF2					2
#define AF3					3
#define AF4					4
#define AF5					5
#define AF6					6
#define AF7					7

////////// Interrupt /////////////////

#define EXTI_1_0			  5
#define EXTI_3_2			  6
#define EXTI_4_15			  7

////////////////////////////////////////
//////////GPIO API ///////////
//////////////////////////////////////

void GPIO_Init(GPIO_Handle_t *pGPIOHandle) ;
void GPIO_DeInit(Gpio_RegDef_t *pGPIOx);
void GPIO_PeriClockControl(Gpio_RegDef_t *pGPIOx, uint8_t enOrDi);
uint8_t GPIO_ReadFromInputPin(Gpio_RegDef_t *pGPIOx , uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(Gpio_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(Gpio_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value);
void GPIO_WriteToOutputPort(Gpio_RegDef_t *pGPIOx , uint16_t Value);
void GPIO_ToggIeOutputPin(Gpio_RegDef_t *pGPIOx ,uint8_t PinNumber);
void GPIO_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority);
void GPIO_IRQPriority(uint8_t IRQ_Number , uint8_t Priority);
void GPIO_IRQHandling(GPIO_Handle_t *pGPIOHandle);


#endif /* STM32L053XX_DRIVERS_H_ */
