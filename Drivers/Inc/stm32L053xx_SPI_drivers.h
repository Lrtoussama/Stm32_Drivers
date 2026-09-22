/*
 * stm32L053xx_SPI_drivers.h
 *
 *  Created on: Aug 20, 2026
 *      Author: admin
 */

#ifndef INC_STM32L053XX_SPI_DRIVERS_H_
#define INC_STM32L053XX_SPI_DRIVERS_H_

#include "Stm32L053xx.h"


typedef struct{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_PinConfig_t;

typedef struct{
	Spi_RegDef_t * pSPI;
	SPI_PinConfig_t SPI_PinConfig;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen ;
	uint32_t RxLen ;
	uint8_t TxState;
	uint8_t RxState;
}SPI_Handle_t;

// Device Mode

#define SPI_DEVICE_MODE_MASTER 		1
#define SPI_DEVICE_MODE_SLAVE 		0

// SPI COMM type

#define SPI_BUS_CONFIG_FD 			0
#define SPI_BUS_CONFIG_HD			1
#define SPI_BUS_CONFIG_RX_ONLY		2

// CLK speed

#define SPI_CLK_SPEED_DIV_2			0
#define SPI_CLK_SPEED_DIV_4			1
#define SPI_CLK_SPEED_DIV_8			2
#define SPI_CLK_SPEED_DIV_16		3

// frame format of spi

#define SPI_FRAME_FORMAT_8			0
#define SPI_FRAME_FORMAT_16			1

// CPOL
#define SPI_CPOL_HIGH				1
#define SPI_CPOL_LOW				0

// CPHA
#define SPI_CPHA_HIGH				1
#define SPI_CPHA_LOW				0

// SSM

#define SPI_SSM_DI					0
#define SPI_SSM_EN					1


// spi tx status
#define SPI_IS_EMPTY				1
#define SPI_IS_NOT_EMPTY			0

// Macros For Tx and Rx state
#define SPI_READY					0
#define SPI_BUSY_IN_RX 				1
#define SPI_BUSY_IN_TX 				2

// Regi
// status register
#define SPI_SR_RXNE 				0
#define SPI_SR_TXNE 				1

// interrupt register

#define SPI_CR2_RXECIE				6
#define SPI_CR2_TXECIE				7

// overun flag

#define SPI_SR_OVR					6
#define SPI_CR2_ERRIE				5

// application call back

#define SPI_RX_COMPL 				1
#define SPI_TX_COMPL				2
#define SPI_OVR_EVENT				3





void SPI_Init(SPI_Handle_t *pSPIHandle) ;

void SPI_DeInit(Spi_RegDef_t *pSPIx);

void SPI_PeriClockControl(Spi_RegDef_t *pSPIx, uint8_t enOrDi);

void SPI_Send_Data(Spi_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t len);

uint8_t SPI_Send_DataIT(SPI_Handle_t *SPI_Handle , uint8_t *pTxBuffer , uint32_t len);

void SPI_Receive_Data(Spi_RegDef_t *pSPIx , uint8_t *pRxBuffer , uint32_t len);

uint8_t SPI_Receive_DataIT(SPI_Handle_t *SPI_Handle , uint8_t *pRxBuffer , uint32_t len);

void SPI_Peri_Control(Spi_RegDef_t *pSPIx , uint8_t EnOrDi);

void SSI_Config(Spi_RegDef_t *pSPIx , uint8_t EnOrDi);

void SPI_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority);

void SPI_IRQPriority(uint8_t IRQ_Number , uint8_t Priority);

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

// app call back

void SPI_ApplicationCallback(SPI_Handle_t *pSPIHandle, uint8_t App_ev);

#endif /* INC_STM32L053XX_SPI_DRIVERS_H_ */
