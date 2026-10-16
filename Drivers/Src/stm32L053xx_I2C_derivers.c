/*
 * stm32L053xx_I2C_derivers.c
 *
 *  Created on: Sep 6, 2026
 *      Author: admin
 */


#include "stm32L053xx_I2C_derivers.h"
extern uint8_t I2C_Rx_state ;
extern uint8_t I2C_Tx_state ;
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
	pI2CHandle -> pI2C -> I2C_OAR1 |= (pI2CHandle -> I2C_Config.I2C_DeviceAddress << 0);
	// 7bit mode -> I2C_CR2 , bit 11
	pI2CHandle -> pI2C -> I2C_CR2 |= (0 << 11);
	// SA -> I2C_CR2 , bit 1-7 in normal mode
	pI2CHandle -> pI2C -> I2C_CR2 &= ~(0xFF<< 1);
	pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.I2C_SlaveAddress << 1);
	// Tx_Direction ->  I2C_CR2 , bit 10
	pI2CHandle -> pI2C -> I2C_CR2 &= ~(0x01 << 10);
	pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.I2C_W_R << 10);
	// Data length -> NBYTES , bit 16 - 23
	pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.Data_Length << 16);
	// autoend -> AUTOEND , bit 25
	pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 25);
	// Start Comm -> I2C_CR2 , bit 13
	pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 13);

}

void I2C_Master_Transmit(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len, uint8_t SlaveAdresse){
	pI2CHandle -> I2C_Config.I2C_W_R = WRITE ;
	pI2CHandle -> I2C_Config.I2C_SlaveAddress = SlaveAdresse;
	pI2CHandle -> I2C_Config.Data_Length = len ;
	// wait until bus become free
	while(((pI2CHandle -> pI2C -> I2C_ISR >> 15) & 0x01) == 1);
	I2C_Master_Comm_Init(pI2CHandle);
	while(len > 0){
		//TXE => I2C_ISR , bit 0
		while(((pI2CHandle -> pI2C -> I2C_ISR >> 0) & 0x01) != 1);
		pI2CHandle -> pI2C -> I2C_TXDR = *pTxBuffer;
		len --;
		pTxBuffer ++;

	}

}
void I2C_Master_Receive(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len, uint8_t SlaveAdresse){
	pI2CHandle -> I2C_Config.I2C_W_R = READ ;
	pI2CHandle -> I2C_Config.I2C_SlaveAddress = SlaveAdresse;
	pI2CHandle -> I2C_Config.Data_Length = len ;
	// wait until bus become free
	while(((pI2CHandle -> pI2C -> I2C_ISR >> 15) & 0x01) == 1);
	I2C_Master_Comm_Init(pI2CHandle);
	while(len > 0){
		//RXNE => I2C_ISR , bit 2
		while(((pI2CHandle -> pI2C -> I2C_ISR >> 2) & 0x01) != 1);
		*pTxBuffer = pI2CHandle -> pI2C -> I2C_RXDR;
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
// i2c master send data interrupt // enabling interrupt and init

uint8_t I2C_Master_TransmitIT(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len , uint8_t SlaveAdresse){

	uint8_t current_state = pI2CHandle->I2C_Config.TxRxState ;

	if(current_state != I2C_BUSY_IN_RX && current_state != I2C_BUSY_IN_TX){
		I2C_Peri_Control(pI2CHandle->pI2C , ENABLE);
		pI2CHandle->I2C_Config.I2C_SlaveAddress = SlaveAdresse;
		pI2CHandle->I2C_Config.Data_Length = len;
		pI2CHandle->I2C_Config.pTxBuffer = pTxBuffer;
		pI2CHandle->I2C_Config.TxRxState = I2C_BUSY_IN_TX ;

		////////////// intilize i2c registers ////////////
		pI2CHandle -> pI2C -> I2C_OAR1 |= (pI2CHandle -> I2C_Config.I2C_DeviceAddress << 0);
		// 7bit mode -> I2C_CR2 , bit 11
		pI2CHandle -> pI2C -> I2C_CR2 |= (0 << 11);
		// SA -> I2C_CR2 , bit 1-7 in normal mode
		pI2CHandle -> pI2C -> I2C_CR2 &= ~(0xFF<< 1);
		pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.I2C_SlaveAddress << 1);
		// Tx_Direction ->  I2C_CR2 , bit 10
		pI2CHandle -> pI2C -> I2C_CR2 &= ~(READ << 10);
		// Data length -> NBYTES , bit 16 - 23
		pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.Data_Length << 16);
		// autoend -> AUTOEND , bit 25
		pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 25);
		// enabling interrupt of txie , register cr1 , bit txie 1
		pI2CHandle -> pI2C->I2C_CR1 |= (0x01 << 1);
		// wait until bus become free
		while(((pI2CHandle -> pI2C -> I2C_ISR >> 15) & 0x01) == 1);
		// Start Comm -> I2C_CR2 , bit 13
		pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 13);

	}
	return current_state;

}

// i2c master receive data interrupt // enabling interrupt and init

uint8_t I2C_Master_ReceiveIT(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t len , uint8_t SlaveAdresse, GPIO_Handle_t* McoGpio){

	uint8_t current_state = pI2CHandle->I2C_Config.TxRxState ;
	GPIO_WriteToOutputPin(McoGpio->pGPIO , 8 , 1);

	if(current_state != I2C_BUSY_IN_RX && current_state != I2C_BUSY_IN_TX){
		I2C_Peri_Control(pI2CHandle->pI2C , ENABLE);
		pI2CHandle->I2C_Config.I2C_SlaveAddress = SlaveAdresse;
		pI2CHandle->I2C_Config.Data_Length = len;
		pI2CHandle->I2C_Config.pRxBuffer = pRxBuffer;
		pI2CHandle->I2C_Config.TxRxState = I2C_BUSY_IN_RX ;
		////////////// intilize i2c registers ////////////
		// device own adresse
		// OA -> I2C_OAR1 , bit 0-9
		pI2CHandle -> pI2C -> I2C_OAR1 |= (pI2CHandle -> I2C_Config.I2C_DeviceAddress << 0);
		// 7bit mode -> I2C_CR2 , bit 11
		pI2CHandle -> pI2C -> I2C_CR2 |= (0 << 11);
		// SA -> I2C_CR2 , bit 1-7 in normal mode
		pI2CHandle -> pI2C -> I2C_CR2 &= ~(0xFF<< 1);
		pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.I2C_SlaveAddress << 1);
		// Tx_Direction ->  I2C_CR2 , bit 10
		pI2CHandle -> pI2C -> I2C_CR2 |= (READ << 10);
		// Data length -> NBYTES , bit 16 - 23
		pI2CHandle -> pI2C -> I2C_CR2 |= (pI2CHandle -> I2C_Config.Data_Length << 16);
		// autoend -> AUTOEND , bit 25
		pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 25);
		// enabling interrupt of rxie , register cr1 , bit rxie 2
		pI2CHandle -> pI2C->I2C_CR1 |= (0x01 << 2);
		// wait until bus become free
		while(((pI2CHandle -> pI2C -> I2C_ISR >> 15) & 0x01) == 1);
		// Start Comm -> I2C_CR2 , bit 13
		pI2CHandle -> pI2C -> I2C_CR2 |= (1 << 13);

	}
	return current_state;

}

void I2C_Slave_Send(I2C_Handle_t* pI2CHandle , uint8_t Own_adress, uint8_t *pTxBuffer , uint32_t len ){

	pI2CHandle -> I2C_Config.Data_Length = len;
	pI2CHandle -> I2C_Config.TxRxState = pTxBuffer ;
	pI2CHandle->I2C_Config.TxRxState = I2C_BUSY_IN_TX_SLAVE;
	// OA -> I2C_OAR1 , bit 0-9
	pI2CHandle -> pI2C -> I2C_OAR1 |= (Own_adress << 1);
	// enable own address , in // OA -> I2C_OAR1 , bit 15
	pI2CHandle -> pI2C -> I2C_OAR1 |= (ENABLE << 15);
	// NOSTRETCH enable slave to do stretching , register cr1 , bit NOSTRETCH 17
	pI2CHandle -> pI2C->I2C_CR1 &= ~(0x01 <<17);
	// RELOAD to tell to NBYTES there more data to be transmitted , reg i2c_cr2 -> bit 24
	pI2CHandle -> pI2C->I2C_CR2 |= (0x01 <<24);
	// Nbyte cr1 -> 16
	pI2CHandle -> pI2C->I2C_CR2 |= (0x01 <<16);
	// SBC is used to strech betwen 8th and 9th bit to end N/Ack , reg i2c_cr1 -> bit 16
	pI2CHandle -> pI2C -> I2C_CR1 |= (0x01 << 16);
	///////////////////////////gcen
	pI2CHandle -> pI2C -> I2C_CR1 |= (0x01 << 19);
	// enabling interrupt of ADDRIE , register cr1 , bit ADDRIE 3
	pI2CHandle -> pI2C->I2C_CR1 |= (0x01 <<3);
	//enabling stop flag, register cr1 , bit stop 5
	pI2CHandle -> pI2C->I2C_CR1 |= (0x01 << 5);


	I2C_Peri_Control(pI2CHandle->pI2C , ENABLE);


}



// the called function in case of the existence of an event

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle ){

	static uint8_t Interrupt_src = 0 ;

	uint8_t Rx_Event = (pI2CHandle -> pI2C->I2C_ISR >> 2 ) & (0x01);
	uint8_t Tx_Event = (pI2CHandle -> pI2C->I2C_ISR >> 1 ) & (0x01);
	// verifying if addr bit (address match to the OA)
	uint8_t ADDR_Event = (pI2CHandle -> pI2C->I2C_ISR >> 3 ) & (0x01);
	uint8_t Stop_Event = (pI2CHandle -> pI2C->I2C_ISR >> 5 ) & (0x01);
	uint8_t TRC = (pI2CHandle -> pI2C->I2C_ISR >> 7 ) & (0x01);


	I2C_Rx_state = 0 ;
	I2C_Tx_state = 0 ;


	if(pI2CHandle->I2C_Config.Data_Length < 0){
		// ensuring that the last byte ack
		Interrupt_src = 0 ;
		pI2CHandle -> pI2C->I2C_CR2 &= ~(0x01 <<24);
		pI2CHandle -> pI2C->I2C_CR1 &= ~(0x01 <<3);
		pI2CHandle -> pI2C -> I2C_OAR1 &= ~(ENABLE << 15);
		pI2CHandle -> pI2C->I2C_CR2 &= ~(0x01 <<16);
		pI2CHandle -> pI2C -> I2C_CR1 &= ~(0x01 << 19);
	}

	if(TRC &&  Interrupt_src){
		//write 1 to nbyte for streching release
		pI2CHandle -> pI2C->I2C_CR2 |= (0x01 <<16);
	}

	if(Tx_Event && (pI2CHandle->I2C_Config.TxRxState == I2C_BUSY_IN_TX_SLAVE)){
		pI2CHandle -> pI2C -> I2C_TXDR = *(pI2CHandle->I2C_Config.pTxBuffer);
		pI2CHandle->I2C_Config.Data_Length--;
		pI2CHandle->I2C_Config.pTxBuffer ++ ;
		////////////
		if(pI2CHandle->I2C_Config.Data_Length == 0)
		{
			if(Interrupt_src){
				// disable add event by claering ADDRCF : i2c_icr -> bit 3
				pI2CHandle -> pI2C->I2C_ICR |= (0x01 << 3);
				pI2CHandle->I2C_Config.TxRxState = I2C_READY ;
			}
			pI2CHandle -> pI2C->I2C_CR1 &= ~(0x01 <<3);
			pI2CHandle->I2C_Config.TxRxState = I2C_READY ;
			ApplicationCallBack(pI2CHandle , Tx_terminated);
		}
	}

	if(Tx_Event && (pI2CHandle->I2C_Config.TxRxState == I2C_BUSY_IN_TX)){
		pI2CHandle -> pI2C -> I2C_TXDR = *(pI2CHandle->I2C_Config.pTxBuffer);
		pI2CHandle->I2C_Config.Data_Length--;
		pI2CHandle->I2C_Config.pTxBuffer ++ ;


		////////////
		if(pI2CHandle->I2C_Config.Data_Length == 0)
		{
			pI2CHandle->I2C_Config.TxRxState = I2C_READY ;
			ApplicationCallBack(pI2CHandle , Tx_terminated);

		}
	}

	if(Rx_Event){
		*pI2CHandle->I2C_Config.pRxBuffer = pI2CHandle -> pI2C -> I2C_RXDR;
		pI2CHandle->I2C_Config.Data_Length --;
		pI2CHandle->I2C_Config.pRxBuffer ++;
		if(pI2CHandle->I2C_Config.Data_Length == 0){
			pI2CHandle->I2C_Config.TxRxState = I2C_READY ;
			ApplicationCallBack(pI2CHandle , Rx_terminated);
		}

	}
	if(ADDR_Event){
		pI2CHandle -> pI2C->I2C_ICR |= (0x01 << 3);
		// concene slave mode
		pI2CHandle -> pI2C->I2C_CR1 |= (0x01 << 6);
		Interrupt_src = 1 ;
	}
	if(Stop_Event){
		I2C1->I2C_CR2 &= ~(0x01 <<24);
		I2C1->I2C_CR1 &= ~(0x01 <<3);
		I2C1 -> I2C_OAR1 &= ~(ENABLE << 15);
		I2C1->I2C_CR2 &= ~(0x01 <<16);
		I2C1 -> I2C_CR1 &= ~(0x01 << 19);
	}


}

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
	*((uint32_t*)NVIC_IPR + tmp1) |= (Priority << (tmp2 * 8 + IRQ_PRIO_PADDING));
}

void __attribute__ ((weak)) ApplicationCallBack(I2C_Handle_t *pI2CHandle , uint8_t App_ev){

}
