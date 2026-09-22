/*
 * stm32L053xx_I2C_derivers.c
 *
 *  Created on: Sep 6, 2026
 *      Author: admin
 */


#include "stm32L053xx_I2C_derivers.h"

void I2C_GenerateStartCondition(I2C_Handle_t *pI2CHandle);

// ENabling Periph by rcc
//
//uint32_t Get_RCC_clock(){
//	uint16_t AhbPresc[] = {2,4,8,16,32,64,128,256,512};
//	uint8_t Apb_1_Presc[] = {2,4,8,16};
//	uint8_t TempPresc , clk_src;
//	uint32_t sys_clk;
//	clk_src = (RCC_CTR -> RCC_CFGR >> 2) & (0x03) ;
//	if(clk_src == 0){
//		//Msi
//	}else if(clk_src == 1){
//		// HSi
//		sys_clk = 16000000;
//	}else if(clk_src == 2){
//		// HSE
//		sys_clk = 8000000;
//	}else{
//		// PLL
//	}
//	// for ahb
//	TempPresc = (RCC_CTR -> RCC_CFGR >> 4) & (0x0F);
//	if(TempPresc > 0){
//		sys_clk = sys_clk / AhbPresc[TempPresc - 8];
//	}
//	// for apb
//	TempPresc = (RCC_CTR -> RCC_CFGR >> 8) & (0x07);
//	if(TempPresc > 0){
//		sys_clk = sys_clk / Apb_1_Presc[TempPresc - 4];
//	}
//	return sys_clk;
//}

void I2C_Init(I2C_Handle_t *pI2CHandle){

	I2C_PeriClockControl(pI2CHandle->pI2C, ENABLE);
	// chose the i2cclk // by default related to apb

	//scll -> 0 , sclh -> 8 , SDADEL ->16 , SCLDEL-> 20 ,PRESC -> 28
	//pres : bit 28-31 , value 0x03
	pI2CHandle->pI2C->I2C_TIMINGR |= (0x03 << 28);
	//scldel : bit 20 - 23 , value 0x4
	pI2CHandle->pI2C->I2C_TIMINGR |= (0x4 << 20);
	//sdadel : bit 16 - 19 , value 0x02
	pI2CHandle->pI2C->I2C_TIMINGR |= (0x2 << 16);
	//sclh : bit 8 - 15 , value 0x0F
	pI2CHandle->pI2C->I2C_TIMINGR |= (0x0F << 8);
	//scll : bit 0-7 , value 0x13
	pI2CHandle->pI2C->I2C_TIMINGR |= (0x13 << 0);

}

void I2C_Master_Comm_Init(I2C_Handle_t *pI2CHandle){
	I2C_Peri_Control(pI2CHandle->pI2C , ENABLE);
	// device own adresse
	// OA -> I2C_OAR1 , bit 0-9
	pI2CHandle -> pI2C -> I2C_OAR1 |= (pI2CHandle -> I2C_PinConfig.I2C_DeviceAddress << 0);
	// 7bit mode -> I2C_CR2 , bit 11
	pI2CHandle -> pI2C -> I2C_CR2 |= (0 << 11);
	// SA -> I2C_CR2 , bit 1-7 in normal mode
	pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_PinConfig.I2C_SlaveAddress << 1);
	// Tx_Direction ->  I2C_CR2 , bit 10
	pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_PinConfig.I2C_W_R << 10);
	// Data length -> NBYTES , bit 16 - 23
	pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_PinConfig.Data_Length << 16);
	// Start Comm -> I2C_CR2 , bit 13
	pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 13);

}

void I2C_Master_Transmit(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len){
	while(len > 0){
		//TXE => I2C_ISR , bit 0
		while(((pI2CHandle -> pI2C -> I2C_ISR >> 0) & 0x01) != 1);
		pI2CHandle -> pI2C -> I2C_TXDR = *pTxBuffer;
		len --;
		pTxBuffer ++;

	}
}
void I2C_Master_Receive(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len){
	while(len > 0){
		//RXNE => I2C_ISR , bit 2
		while(((pI2CHandle -> pI2C -> I2C_ISR >> 2) & 0x01) != 1);
		*pTxBuffer = pI2CHandle -> pI2C -> I2C_TXDR ;
		len --;
		pTxBuffer ++;

	}
}
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t enOrDi){
	if(enOrDi == ENABLE){
		if(pI2Cx == I2C1){
			RCC_EN_I2C1();
		}else if(pI2Cx == I2C2){
			RCC_EN_I2C2();
		}else if(pI2Cx == I2C1){
			RCC_EN_I2C1();
		}else{

		}
	}else{
		//TODO
	}
}


// enabling i2c by reg
void I2C_Peri_Control(I2C_RegDef_t *pI2Cx , uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pI2Cx->I2C_CR1 |= (0x01 << 0);
	}else if(EnOrDi == DISABLE){
		pI2Cx->I2C_CR1 &= ~(0x01 << 0);

	}
}

// interrupt config


void I2C_GenerateStartCondition(I2C_Handle_t *pI2CHandle){
	// I2C_CR2 , bit 13 => start
	pI2CHandle -> pI2C->I2C_CR2 |= (0x01 << 13);
}


void I2C_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority){
	if(EnOrDi == ENABLE){
		*(uint32_t*)NVIC_ISER |= (0x01 << IRQ_Number);
	}else{

	}
	I2C_IRQPriority(IRQ_Number ,Priority);
}

void I2C_IRQPriority(uint8_t IRQ_Number , uint8_t Priority){
	uint8_t tmp1 = IRQ_Number / 4;
	uint8_t tmp2 = IRQ_Number % 4;
	*((uint8_t*)NVIC_IPR + tmp1) |= (Priority << (tmp2 * 8 + IRQ_PRIO_PADDING));
}

