

#include "Stm32L053xx.h"
#include "stdint.h"
#include <stdlib.h>
#include <stm32L053xx_gpio_drivers.h>


void toggeling_loop_func();

GPIO_Handle_t * input ;
GPIO_Handle_t * output ;

void delay(){
	for(uint32_t i = 0 ; i<500000 ; i++);
}


int main(){
	//
	input = malloc(sizeof(GPIO_Handle_t));
	output = malloc(sizeof(GPIO_Handle_t));
	output -> pGPIO = GPIOA ;
	GPIO_PeriClockControl(output -> pGPIO, ENABLE);
	output -> GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_OUT ;
	output -> GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_HIGH ;
	output -> GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP ;
	output -> GPIO_PinConfig.GPIO_pinNumber = GPIO_PIN_NO_5 ;
	GPIO_Init(output);

	//toggeling_loop_func();


	input -> pGPIO = GPIOC ;
	GPIO_PeriClockControl(input -> pGPIO, ENABLE);
	input -> GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_IT_RT;
	input -> GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST ;
	input -> GPIO_PinConfig.PinPupdControl = GPIO_PD ;
	input -> GPIO_PinConfig.GPIO_pinNumber = GPIO_PIN_NO_13 ;

	GPIO_Init(input);
	//GPIO_IRQHandling(input);
	GPIO_IRQConfig(EXTI_4_15, ENABLE , 0x02);
	while(1){
	}



}
void toggeling_loop_func(){
	for (int i = 0 ; i < 5 ; i++){
		delay();
		GPIO_ToggIeOutputPin(output->pGPIO , output->GPIO_PinConfig.GPIO_pinNumber);
	}
}


void EXTI4_15_IRQHandler(){
	GPIO_ToggIeOutputPin(output->pGPIO , output->GPIO_PinConfig.GPIO_pinNumber);
	GPIO_IRQHandling(input);


}
