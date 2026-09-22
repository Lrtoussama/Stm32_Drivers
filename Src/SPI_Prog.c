

#include <stdlib.h>
#include "Stm32L053xx.h"
#include "stm32L053xx_SPI_drivers.h"
#include "stm32L053xx_gpio_drivers.h"
#include <string.h>


int main(){

	// setting the gpios
//	gpioA
//	PA4 spi1_NSS
//	PA5 spi1_clk
//	PA6 spi1_miso
//	PA7 spi1_mosi
	uint8_t Pins[] = {4,5,6,7};
	GPIO_Handle_t vGPIOA[4];
	SPI_Handle_t vSPI1;
	char txt[] = "Hello World";
	for(int i = 0 ; i < 4 ; i++){
		vGPIOA[i].pGPIO = GPIOA;
		vGPIOA[i].GPIO_PinConfig.GPIO_AltFunMode = AF0;
		vGPIOA[i].GPIO_PinConfig.GPIO_pinMode = GPIO_MODE_ALT;
		vGPIOA[i].GPIO_PinConfig.GPIO_pin_speed = GPIO_SPEED_FAST;
		vGPIOA[i].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		vGPIOA[i].GPIO_PinConfig.GPIO_pinNumber = Pins[i];
		GPIO_Init(&vGPIOA[i]);
	}

	vSPI1.pSPI = SPI1;
	vSPI1.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD ;
	vSPI1.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER ;
	vSPI1.SPI_PinConfig.SPI_SclkSpeed = SPI_CLK_SPEED_DIV_2 ;
	vSPI1.SPI_PinConfig.SPI_DFF = SPI_FRAME_FORMAT_8 ;
	vSPI1.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW ;
	vSPI1.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW ;
	vSPI1.SPI_PinConfig.SPI_SSM = SPI_SSM_EN ;
	SPI_Init(&vSPI1);

	SSI_Config(vSPI1.pSPI , ENABLE);

	SPI_Peri_Control(vSPI1.pSPI , ENABLE);

	SPI_Send_Data(vSPI1.pSPI, (uint8_t*)txt , strlen(txt));

	while(1);
}
