/*
 * stm32L053xx_I2C_derivers.h
 *
 *  Created on: Sep 6, 2026
 *      Author: admin
 */

#ifndef INC_STM32L053XX_I2C_DERIVERS_H_
#define INC_STM32L053XX_I2C_DERIVERS_H_

#include "Stm32L053xx.h"

typedef struct{
	uint8_t I2C_SCLspeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_SlaveAddress;
	uint8_t I2C_W_R;
	uint8_t Data_Length;
}I2C_PinConfig_t;

typedef struct{
	I2C_RegDef_t * pI2C;
	I2C_PinConfig_t I2C_PinConfig;

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

void I2C_Init(I2C_Handle_t *pI2CHandle) ;

void I2C_Master_Comm_Init(I2C_Handle_t *pI2CHandle);

void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t enOrDi);

void I2C_Peri_Control(I2C_RegDef_t *pI2Cx , uint8_t EnOrDi);

void I2C_IRQConfig(uint8_t IRQ_Number , uint8_t EnOrDi , uint8_t Priority);

void I2C_IRQPriority(uint8_t IRQ_Number , uint8_t Priority);

void I2C_IRQHandling(I2C_Handle_t *pI2CHandle);

// i2c send data

void I2C_Master_Transmit(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len);

// i2c master receive data

void I2C_Master_Receive(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len);
// app call back

void I2C_ApplicationCallback(I2C_Handle_t *pI2CHandle, uint8_t App_ev);



#endif /* INC_STM32L053XX_I2C_DERIVERS_H_ */
