/*
 * stm32L053xx_SPI_drivers.h
 *
 *  Created on: Aug 20, 2026
 *      Author: admin
 */


#include "Stm32L053xx.h"
#include "stm32L053xx_SPI_drivers.h"

static void Spi_Tx_interruption_handler(SPI_Handle_t *pSPIHandle);
static void Spi_Rx_interruption_handler(SPI_Handle_t *pSPIHandle);
static void Spi_OVR_interruption_handler(SPI_Handle_t *pSPIHandle);

void SPI_PeriClockControl(Spi_RegDef_t *pSPIx, uint8_t enOrDi){
	if(enOrDi == ENABLE){
		if(pSPIx == SPI1){
			RCC_EN_SPI_1();
		}else if(pSPIx == SPI2){
			RCC_EN_SPI_2();
		}

	}else{

	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle){

	SPI_PeriClockControl(pSPIHandle -> pSPI, ENABLE);
	uint32_t temp = 0 ;
	// configuring device master ou slave
	temp |= (pSPIHandle -> SPI_PinConfig.SPI_DeviceMode << 2 );
	// configure HD ,FD , RX Only

	if(pSPIHandle ->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		temp &= ~(1 << 15 );
	}else if(pSPIHandle ->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		temp |= (1 << 15 );
	}else if(pSPIHandle ->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_RX_ONLY){
		temp &= ~(1 << 15 );
		temp |= (1 << 10 );
	}

	// baude rate
	temp &= ~(0x07 << 3 );
	temp |= (pSPIHandle -> SPI_PinConfig.SPI_SclkSpeed << 3 );

	//data frame format dff
	temp |= (pSPIHandle -> SPI_PinConfig.SPI_DFF << 11 );

	// CPOL
	temp |= (pSPIHandle -> SPI_PinConfig.SPI_CPOL << 1 );

	//CPHA
	temp |= (pSPIHandle -> SPI_PinConfig.SPI_CPHA << 0 );

	//SSM
	temp |= (pSPIHandle -> SPI_PinConfig.SPI_SSM << 9 );

	pSPIHandle -> pSPI -> SPI_CR1 = temp;
}

void SPI_Send_Data(Spi_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t len){
	while(len != 0){
		while(!(pSPIx -> SPI_SR & (SPI_IS_EMPTY << 1)));
		if((pSPIx -> SPI_CR1 & (1 << 11)) == SPI_FRAME_FORMAT_8){
			pSPIx -> SPI_DR = *(pTxBuffer);
			len-- ;
			pTxBuffer++;
		}else if(((pSPIx -> SPI_CR1 >> 11) & 1) == SPI_FRAME_FORMAT_16){
			pSPIx -> SPI_DR = *((uint16_t*)pTxBuffer);
			len-- ;
			len-- ;
			(uint16_t*)pTxBuffer ++;
		}
	}
}

void SPI_Receive_Data(Spi_RegDef_t *pSPIx , uint8_t *pRxBuffer , uint32_t len){
	while(len != 0){
		while(!(pSPIx -> SPI_SR & (SPI_IS_EMPTY << 0)));
		if((pSPIx -> SPI_CR1 & (1 << 11)) == SPI_FRAME_FORMAT_8){
			*(pRxBuffer) = pSPIx -> SPI_DR;
			len-- ;
			pRxBuffer++;
		}else if(((pSPIx -> SPI_CR1 >> 11) & 1) == SPI_FRAME_FORMAT_16){
			*((uint16_t*)pRxBuffer) = pSPIx -> SPI_DR;
			len-- ;
			len-- ;
			(uint16_t*)pRxBuffer ++;
		}
	}
}



void SPI_Peri_Control(Spi_RegDef_t *pSPIx , uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pSPIx ->SPI_CR1 |= (0x01 << 6);
	}else{
		pSPIx ->SPI_CR1 &= ~(0x01 << 6);
	}
}



void SSI_Config(Spi_RegDef_t *pSPIx , uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pSPIx->SPI_CR1 |= (0x01 << 8);
	}else{
		pSPIx->SPI_CR1 &= ~(0x01 << 8);
	}
}


void SPI_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority){

	if(EnOrDi == ENABLE){
		*(volatile uint32_t *)(NVIC_ISER) |= (0x1 << IRQ_Number);
	}else{
		*(volatile uint32_t *)(NVIC_ICER) |= (0x1 << IRQ_Number);
	}
	SPI_IRQPriority(IRQ_Number , Priority);
}

void SPI_IRQPriority(uint8_t IRQ_Number , uint8_t Priority){

	uint8_t temp1 = IRQ_Number / 4 ;
	uint8_t temp2 = IRQ_Number % 4 ;
	*(volatile uint32_t *)(NVIC_IPR + (int32_t) temp1 * 4) |= (0x02 << ((Priority * 8 )+IRQ_PRIO_PADDING ));
}


uint8_t SPI_Send_DataIT(SPI_Handle_t *SPI_Handle , uint8_t *pTxBuffer , uint32_t len){

	uint8_t status = SPI_Handle ->TxState ;

	if(status != SPI_BUSY_IN_TX){

		// save length and data
		SPI_Handle -> pTxBuffer = pTxBuffer ;
		SPI_Handle -> TxLen = len ;
		// spi now is busy
		SPI_Handle -> TxState = SPI_BUSY_IN_TX ;
		// spi interrupt enable
		SPI_Handle -> pSPI->SPI_CR2 |= (0x01 << SPI_CR2_TXECIE);

	}
	return status;
}

uint8_t SPI_Receive_DataIT(SPI_Handle_t *SPI_Handle , uint8_t *pRxBuffer , uint32_t len){
	uint8_t status = SPI_Handle ->RxState ;

	if(status != SPI_BUSY_IN_RX){

		// save length and data
		SPI_Handle -> pRxBuffer = pRxBuffer ;
		SPI_Handle -> RxLen = len ;
		// spi now is busy
		SPI_Handle -> RxState = SPI_BUSY_IN_RX ;
		// spi interrupt enable
		SPI_Handle -> pSPI->SPI_CR2 |= (0x01 << 6);

	}
	return status;
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle){

	uint8_t temp1 , temp2 ;
	// txe status
	temp1 = (pSPIHandle -> pSPI->SPI_SR) & (1 << SPI_SR_TXNE);
	// check if interrupt tx
	temp2 = (pSPIHandle -> pSPI->SPI_CR2) & (1 << SPI_CR2_TXECIE);

	if(temp1 && temp2){
		Spi_Tx_interruption_handler(pSPIHandle);
	}

	// RXe status
	temp1 = (pSPIHandle -> pSPI->SPI_SR) & (1 << SPI_SR_RXNE);
		// check if interrupt Rx
	temp2 = (pSPIHandle -> pSPI->SPI_CR2) & (1 << SPI_CR2_RXECIE);

	if(temp1 && temp2){
		Spi_Rx_interruption_handler(pSPIHandle);
	}

	temp1 = (pSPIHandle -> pSPI->SPI_SR) & (1 << SPI_SR_OVR);

	temp2 = (pSPIHandle -> pSPI->SPI_CR2) & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2){
		Spi_OVR_interruption_handler(pSPIHandle);
	}


}

static void Spi_Tx_interruption_handler(SPI_Handle_t *pSPIHandle){
	if((pSPIHandle->pSPI -> SPI_CR1 & (1 << 11)) == SPI_FRAME_FORMAT_8){
		pSPIHandle->pSPI -> SPI_DR = *(pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen-- ;
		pSPIHandle->pTxBuffer++;
	}else if(((pSPIHandle->pSPI -> SPI_CR1 >> 11) & 1) == SPI_FRAME_FORMAT_16){
		pSPIHandle->pSPI -> SPI_DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen-- ;
		pSPIHandle->TxLen-- ;
		(uint16_t*)pSPIHandle->pTxBuffer ++;
	}

	if(pSPIHandle->TxLen == 0){
		pSPIHandle -> pSPI->SPI_CR2 &= ~(0x01 << SPI_CR2_TXECIE);
		pSPIHandle -> pSPI = ((void*)0);
		pSPIHandle->TxLen = 0;
		pSPIHandle->TxState = SPI_READY ;
		SPI_ApplicationCallback(pSPIHandle,SPI_TX_COMPL);

	}
}

static void Spi_Rx_interruption_handler(SPI_Handle_t *pSPIHandle){
	if((pSPIHandle->pSPI -> SPI_CR1 & (1 << 11)) == SPI_FRAME_FORMAT_8){
		*(pSPIHandle -> pRxBuffer) = pSPIHandle -> pSPI -> SPI_DR;
		pSPIHandle ->RxLen-- ;
		pSPIHandle ->pRxBuffer++;
	}else if(((pSPIHandle ->pSPI -> SPI_CR1 >> 11) & 1) == SPI_FRAME_FORMAT_16){
		*((uint16_t*)pSPIHandle ->pRxBuffer) = pSPIHandle ->pSPI -> SPI_DR;
		pSPIHandle ->RxLen-- ;
		pSPIHandle ->RxLen-- ;
		(uint16_t*)pSPIHandle ->pRxBuffer ++;

	}
	if(pSPIHandle->RxLen == 0){
		pSPIHandle -> pSPI->SPI_CR2 &= ~(0x01 << SPI_CR2_RXECIE);
		pSPIHandle -> pSPI = ((void*)0);
		pSPIHandle->RxLen = 0;
		pSPIHandle->RxState = SPI_READY ;
		SPI_ApplicationCallback(pSPIHandle,SPI_RX_COMPL);
	}
}


static void Spi_OVR_interruption_handler(SPI_Handle_t *pSPIHandle){

	uint32_t tmp1;
	uint32_t tmp2;

	tmp1 = pSPIHandle ->pSPI -> SPI_DR;
	tmp2 = pSPIHandle ->pSPI -> SPI_SR;

	(void)tmp1;
	(void)tmp2;

	SPI_ApplicationCallback(pSPIHandle,SPI_OVR_EVENT);

}

__attribute__((weak)) void SPI_ApplicationCallback(SPI_Handle_t *pSPIHandle, uint8_t App_ev){

}
