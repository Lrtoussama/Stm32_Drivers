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

I2C_Handle_t vI2C ;

uint8_t I2C_Rx_state ;
uint8_t I2C_Tx_state ;

void delay(){
	for(int i = 0 ; i < 10000000 ; i ++){

	}
}

void I2C_Reg_Init( I2C_RegDef_t * I2C_n){
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
	(&vI2C)->pI2C = I2C_n;
//	(&vI2C)->I2C_Config.Data_Length =5  ;
//	(&vI2C)->I2C_Config.I2C_DeviceAddress = 0x61;
//	(&vI2C)->I2C_Config.I2C_SlaveAddress = 0x68;
//	(&vI2C)->I2C_Config.I2C_W_R = W_R_mode ;
	I2C_Init(&vI2C);
}


void GPIO_A1_init(GPIO_Handle_t * Gpio_A1){
	(Gpio_A1)->pGPIO = GPIOA;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pinNumber = GPIO_PIN_NO_8 ;
	(Gpio_A1)->GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	(Gpio_A1)->GPIO_PinConfig.PinPupdControl = GPIO_PD;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_OUT;
	GPIO_Init(Gpio_A1);
}

int main(){

	GPIO_Handle_t* McoGpio = malloc(sizeof(GPIO_Handle_t));
	GPIO_A1_init(McoGpio);

	((RCC_struc_t *)RCC_BASE) -> RCC_CR |= (0x01 << 0);
	((RCC_struc_t *)RCC_BASE) -> RCC_CFGR |= (0x01 << 24);
	((RCC_struc_t *)RCC_BASE) -> RCC_CFGR |= (0x01 << 0);
//	uint8_t command = 0x51;
//	uint8_t response;
	// enabling interrupt of txie , register cr1 , bit txie 1
	char txt[] = "hello";

	vI2C.I2C_Config.pTxBuffer = (uint8_t*)txt;

	I2C_Reg_Init(I2C1);

	I2C1->I2C_CR1 |= (0x01 << 1);
	I2C1->I2C_CR1 |= (0x01 << 2);

	I2C_IRQConfig( I2C_1_INTERRUPT, ENABLE , 1);


	I2C_Slave_Send(&vI2C , 8, txt, strlen(txt));


	//////////// sending receiving msg through pooling  /////////////////


	while(1);



}

void  ApplicationCallBack(I2C_Handle_t *pI2CHandle , uint8_t App_ev){

	if(App_ev == Tx_terminated){

		I2C_Tx_state = 1 ;

	}else if (App_ev == Rx_terminated){

		I2C_Rx_state = 1 ;
	}

}



void I2C1_IRQHandler(){
	I2C_EV_IRQHandling(&vI2C);
}
