/*
 * stm32L053xx_I2C_derivers.h
 *
 *  Created on: Sep 6, 2026
 *      Author: admin
 */

#ifndef INC_STM32L053XX_I2C_DERIVERS_H_
#define INC_STM32L053XX_I2C_DERIVERS_H_

#include "Stm32L053xx.h"

#include "stm32L053xx_gpio_drivers.h"
typedef struct{
	uint8_t I2C_SCLspeed;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint8_t TxRxState;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_SlaveAddress;
	uint8_t I2C_W_R;
	uint8_t Data_Length;
}I2C_Config_t;

typedef struct{
	I2C_RegDef_t * pI2C;
	I2C_Config_t I2C_Config;

}I2C_Handle_t;

// some definition for Pin Config
//		Comm speed
#define I2C_SCL_SPEED_SM	100000
#define I2C_SCL_SPEED_FM2K	200000
#define I2C_SCL_SPEED_FM4K	400000


//		Ack control
#define I2C_ACK_EN				1
#define I2C_ACK_DI				0


//		I2C FMDutyCycle
#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1

// W/R mode

#define WRITE 					0
#define READ					1

// i2c application state

#define I2C_READY				0
#define I2C_BUSY_IN_RX			1
#define I2C_BUSY_IN_TX			2
#define I2C_BUSY_IN_RX_SLAVE 	3
#define I2C_BUSY_IN_TX_SLAVE    4

// application call back

#define Tx_terminated			0
#define Rx_terminated 			1


void I2C_Init(I2C_Handle_t *pI2CHandle);

void I2C_Master_Comm_Init(I2C_Handle_t *pI2CHandle);

void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t enOrDi);

void I2C_Peri_Control(I2C_RegDef_t *pI2Cx , uint8_t EnOrDi);

void I2C_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority);

void I2C_IRQPriority(uint8_t IRQ_Number , uint8_t Priority);

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);

// i2c send data

void I2C_Master_Transmit(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len, uint8_t SlaveAdresse);

// i2c master receive data

void I2C_Master_Receive(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len, uint8_t SlaveAdresse);

// i2c send data interrupt

uint8_t I2C_Master_TransmitIT(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len , uint8_t SlaveAdresse);

// i2c master receive data interrupt

uint8_t I2C_Master_ReceiveIT(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t len , uint8_t SlaveAdresse, GPIO_Handle_t* McoGpio);

// I2C Master

void I2C_Slave_Send(I2C_Handle_t* pI2CHandle , uint8_t Own_adress, uint8_t *pTxBuffer , uint32_t len );

// app call back

void ApplicationCallBack(I2C_Handle_t *pI2CHandle, uint8_t App_ev);



#endif /* INC_STM32L053XX_I2C_DERIVERS_H_ */
