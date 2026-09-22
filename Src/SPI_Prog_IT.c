/*
 * SPI_Prog_IT.c
 *
 *  Created on: Sep 1, 2026
 *      Author: admin
 */




#include <stdlib.h>
#include "Stm32L053xx.h"
#include "stm32L053xx_SPI_drivers.h"
#include "stm32L053xx_gpio_drivers.h"
#include <string.h>

SPI_Handle_t * vSPI1;

void GPIO_A1_init(GPIO_Handle_t * Gpio_A1){
	(Gpio_A1)->pGPIO = GPIOA;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pinNumber = GPIO_PIN_NO_1 ;
	(Gpio_A1)->GPIO_PinConfig.PinPupdControl = GPIO_PD;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST;
	(Gpio_A1)->GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_IN;
	GPIO_Init(Gpio_A1);
}

void SPI1_Reg_Init(){
	// configure the gpios
//	gpioA
//	PA4 spi1_NSS
//	PA5 spi1_clk
//	PA6 spi1_miso
//	PA7 spi1_mosi

	uint8_t Pins[] = {4,5,6,7};
	GPIO_Handle_t vGPIOA[4];
	for(int i = 0 ; i < 4 ; i++){
		vGPIOA[i].pGPIO = GPIOA;
		vGPIOA[i].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		vGPIOA[i].GPIO_PinConfig.PinPupdControl = GPIO_PD;
		vGPIOA[i].GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST;
		vGPIOA[i].GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_ALT;
		vGPIOA[i].GPIO_PinConfig.GPIO_AltFunMode = AF0 ;
		vGPIOA[i].GPIO_PinConfig.GPIO_pinNumber = Pins[i];
		GPIO_Init(&vGPIOA[i]);
	}

	///// setting up SPI
	vSPI1->pSPI = SPI1;
	vSPI1->SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	vSPI1->SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER ;
	vSPI1->SPI_PinConfig.SPI_SclkSpeed = SPI_CLK_SPEED_DIV_2 ;
	vSPI1->SPI_PinConfig.SPI_CPHA = SPI_CPOL_LOW ;
	vSPI1->SPI_PinConfig.SPI_CPOL = SPI_CPHA_LOW ;
	vSPI1->SPI_PinConfig.SPI_DFF = SPI_FRAME_FORMAT_8 ;
	vSPI1->SPI_PinConfig.SPI_SSM = SPI_SSM_EN ;
	vSPI1->TxState = SPI_READY;


	SPI_Init(vSPI1);
	SSI_Config(vSPI1->pSPI , ENABLE);
}

int main(){

	char txt[] = "Hello World";

	vSPI1 = malloc(sizeof(SPI_Handle_t));

	// GPIO for SPI init

	SPI1_Reg_Init();

	SPI_Peri_Control(vSPI1->pSPI , ENABLE);

	SPI_IRQConfig(SPI_1_INTERRUPT , ENABLE , 3);

	SPI_Send_DataIT(vSPI1 , (uint8_t*)txt , strlen(txt));



	while(1){



	}






}

void SPI1_IRQHandler(void){
	SPI_IRQHandling(vSPI1);

}
