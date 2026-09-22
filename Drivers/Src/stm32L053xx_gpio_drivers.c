/*
 * stm32L053xx_drivers.c
 *
 *  Created on: Jul 12, 2026
 *      Author: admin
 */

#include <stm32L053xx_gpio_drivers.h>

void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	// GPIO Enable clock

	GPIO_PeriClockControl(pGPIOHandle->pGPIO, ENABLE);

	// configure the mode
	if(pGPIOHandle->GPIO_PinConfig.GPIO_pinMode <= GPIO_MODE_ANALOG){
		pGPIOHandle->pGPIO->MODER &= ~(0x03 << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);
		pGPIOHandle->pGPIO->MODER |= (pGPIOHandle->GPIO_PinConfig.GPIO_pinMode << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);

	}else{
		// interrupt
		pGPIOHandle->pGPIO->MODER &= ~(0x03 << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);
		pGPIOHandle->pGPIO->MODER |= (GPIO_MODE_IN << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);

		if(pGPIOHandle->GPIO_PinConfig.GPIO_pinMode == GPIO_MODE_IT_FT){
			// falling edge trigger
			EXTI -> EXTI_FTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
			EXTI -> EXTI_RTSR &= ~(1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_pinMode == GPIO_MODE_IT_RT){
			EXTI -> EXTI_FTSR &= ~(1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
			EXTI -> EXTI_RTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
			// rising edge trigger
		}else{
			EXTI -> EXTI_FTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
			EXTI -> EXTI_RTSR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
			// falling and riding edge trigger
		}
		RCC_EN_SYSCONFIG();
		uint32_t EXTI_SYS_x = pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber / 4;
		uint32_t EXTI_SYS_Pin_x = pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber % 4;

		SYSCONFIG -> SYSCFG_EXTICR[EXTI_SYS_x] |= (GPIO_BASE_ADDR(pGPIOHandle->pGPIO) << EXTI_SYS_Pin_x * 4);
		EXTI -> EXTI_IMR |= (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
		//EXTI-> EXTI_PR |=  (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);

	}

	// configure speed
	pGPIOHandle->pGPIO->OSPEEDR |= (pGPIOHandle->GPIO_PinConfig.GPIO_pin_speed << 2 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);

	// configure pupd settings

	pGPIOHandle->pGPIO->PUPDR |= (pGPIOHandle->GPIO_PinConfig.PinPupdControl<< 2 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);

	// configure optype

	pGPIOHandle->pGPIO->OTYPER |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType <<  pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);

	// configure alt functionnality

	if(pGPIOHandle->GPIO_PinConfig.GPIO_pinMode == GPIO_MODE_ALT){
		if(pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber < GPIO_PIN_NO_8){
			pGPIOHandle->pGPIO->AFRL |= (pGPIOHandle->GPIO_PinConfig.GPIO_AltFunMode << 4 * pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber);
		}else{
			pGPIOHandle->pGPIO->AFRH |= (pGPIOHandle->GPIO_PinConfig.GPIO_AltFunMode << 4 * (pGPIOHandle->GPIO_PinConfig.GPIO_pinNumber % 8));
		}
	}




}
void GPIO_DeInit(Gpio_RegDef_t *pGPIOx);


void GPIO_PeriClockControl(Gpio_RegDef_t *pGPIOx, uint8_t enOrDi){

	if(enOrDi == ENABLE){
		if(pGPIOx == GPIOA){
			RCC_EN_GPIOA();
		}else if(pGPIOx == GPIOB){
			RCC_EN_GPIOB();
		}else if(pGPIOx == GPIOC){
			RCC_EN_GPIOC();
		}else if(pGPIOx == GPIOD){
			RCC_EN_GPIOD();
		}else if(pGPIOx == GPIOE){
			RCC_EN_GPIOE();
		}
	}else{
		if(pGPIOx == GPIOA){
			RCC_DI_GPIOA();
		}else if(pGPIOx == GPIOB){
			RCC_DI_GPIOB();
		}else if(pGPIOx == GPIOC){
			RCC_DI_GPIOC();
		}else if(pGPIOx == GPIOD){
			RCC_DI_GPIOD();
		}else if(pGPIOx == GPIOE){
			RCC_DI_GPIOE();
		}
	}

}





uint8_t GPIO_ReadFromInputPin(Gpio_RegDef_t *pGPIOx , uint8_t PinNumber){
	return (uint8_t)(pGPIOx->IDR >> PinNumber & 0x00000001);
}





uint16_t GPIO_ReadFromInputPort(Gpio_RegDef_t *pGPIOx){

	return (uint16_t)(pGPIOx->IDR);
}
void GPIO_WriteToOutputPin(Gpio_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value){

	if( Value == 1){
		pGPIOx->ODR |= (1 << PinNumber);
	}else{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}
void GPIO_WriteToOutputPort(Gpio_RegDef_t *pGPIOx , uint16_t Value){
	pGPIOx->ODR |= (Value);
}
void GPIO_ToggIeOutputPin(Gpio_RegDef_t *pGPIOx ,uint8_t PinNumber){

	pGPIOx->ODR ^= (1 << PinNumber);

}
void GPIO_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority){

	if(EnOrDi == ENABLE){
		*(volatile uint32_t *)(NVIC_ISER) |= (0x1 << IRQ_Number);
	}else{
		*(volatile uint32_t *)(NVIC_ICER) |= (0x1 << IRQ_Number);
	}
	GPIO_IRQPriority(IRQ_Number , Priority);

}

void GPIO_IRQPriority(uint8_t IRQ_Number , uint8_t Priority){

	uint8_t temp1 = IRQ_Number / 4 ;
	uint8_t temp2 = IRQ_Number % 4 ;
	*(volatile uint32_t *)(NVIC_IPR + (int32_t) temp1 * 4) |= (0x02 << ((temp2 * 8 )+IRQ_PRIO_PADDING ));
}


void GPIO_IRQHandling(GPIO_Handle_t *pGPIOHandle){

	if(EXTI-> EXTI_PR & (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber)){
		EXTI-> EXTI_PR |=  (1 << pGPIOHandle -> GPIO_PinConfig.GPIO_pinNumber);
	}
}
