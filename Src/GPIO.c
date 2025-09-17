/*
 * GPIO.c
 *
 *  Created on: Aug 18, 2025
 *      Author: Admin
 */

#include "GPIO.h"

void GPIO_Init(GPIO_Typedef *GPIOx,uint8_t pin, Config_Mode_Type config_mode)
{
	if(pin<=7)
	{
		GPIOx->CRL &= ~(0XFu<<(pin*4));
		GPIOx->CRL |= (config_mode <<(pin*4));
	}
	else
	{
		pin-=8;
		GPIOx->CRH &= ~(0xFu<<(pin*4));
		GPIOx->CRH |= (config_mode <<(pin*4));
	}
}

void GPIO_Write(GPIO_Typedef *GPIOx,uint8_t pin, uint8_t state)
{
	if(state)
	{
		GPIOx->BSRR =(1<<pin);
	}
	else
	{
		GPIOx->BRR= (1<<pin);
	}
}
uint8_t GPIO_Read (GPIO_Typedef *GPIOx,uint8_t pin)
{
	return ((GPIOx->IDR>>pin) &0x01);// de tra ve 1 or 0
}
void GPIO_Toggle (GPIO_Typedef *GPIOx,uint8_t pin)
{
	GPIOx->ODR ^=(1u<<pin);
}

