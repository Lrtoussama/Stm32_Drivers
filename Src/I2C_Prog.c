/*
 * I2C_Prog.c
 *
 *  Created on: Sep 13, 2026
 *      Author: admin
 */
#include <stdint.h>
#include "Stm32L053xx.h"
#include "stm32L053xx_I2C_derivers.h"
#include "stm32L053xx_gpio_drivers.h"
#include <string.h>
#include <stdlib.h>

void delay(){
	for(int i = 0 ; i < 10000000 ; i ++){

	}
}

void I2C_Reg_Init(I2C_Handle_t *vI2C , uint8_t W_R_mode){
	// configure the gpios
//	gpioB
//	PB_8 SCL
//	PB_9 SDA

	uint8_t Pins[] = {8,9};
	GPIO_Handle_t vGPIOB[2];
	for(int i = 0 ; i < sizeof(vGPIOB)/sizeof(GPIO_Handle_t) ; i++){
		vGPIOB[i].pGPIO = GPIOB;
		vGPIOB[i].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
		vGPIOB[i].GPIO_PinConfig.PinPupdControl = GPIO_PU;
		vGPIOB[i].GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST;
		vGPIOB[i].GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_ALT;
		vGPIOB[i].GPIO_PinConfig.GPIO_AltFunMode = AF4 ;
		vGPIOB[i].GPIO_PinConfig.GPIO_pinNumber = Pins[i];
		GPIO_Init(&vGPIOB[i]);
	}
	///// setting up I2C
	vI2C->pI2C = I2C1;
	vI2C->I2C_PinConfig.Data_Length = 2 ;
	vI2C->I2C_PinConfig.I2C_DeviceAddress = 0x61;
	vI2C->I2C_PinConfig.I2C_SlaveAddress = 0x08;
	vI2C->I2C_PinConfig.I2C_W_R = W_R_mode ;
	I2C_Init(vI2C);
}


void GPIO_A1_init(GPIO_Handle_t * Gpio_A1){
	(Gpio_A1)->pGPIO = GPIOA;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pinNumber = GPIO_PIN_NO_8 ;
	(Gpio_A1)->GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	(Gpio_A1)->GPIO_PinConfig.PinPupdControl = GPIO_PD;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_ALT;
	(Gpio_A1)->GPIO_PinConfig.GPIO_AltFunMode = AF0 ;
	GPIO_Init(Gpio_A1);
}

int main(){

	((RCC_struc_t *)RCC_BASE) -> RCC_CR |= (0x01 << 0);
	((RCC_struc_t *)RCC_BASE) -> RCC_CFGR |= (0x01 << 24);
	((RCC_struc_t *)RCC_BASE) -> RCC_CFGR |= (0x01 << 0);
	char msg[] = "hello";
	I2C_Handle_t *vI2C = malloc(sizeof(I2C_Handle_t));
	I2C_Reg_Init(vI2C, WRITE);
	I2C_Master_Comm_Init(vI2C);
	I2C_Master_Transmit(vI2C , (uint8_t*)msg , strlen(msg));
	I2C_Reg_Init(vI2C, READ);
	I2C_Master_Comm_Init(vI2C);
	I2C_Master_Receive(vI2C , (uint8_t*)msg , strlen(msg));


	while(1);



}
