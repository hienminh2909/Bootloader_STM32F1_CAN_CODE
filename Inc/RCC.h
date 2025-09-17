/*
 * RCC.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Admin
 */

#ifndef RCC_H_
#define RCC_H_

#include<RCC_Registers.h>

typedef enum{
	RCC_GPIOA = 2,
	RCC_GPIOB = 3,
	RCC_GPIOC = 4,
	RCC_GPIOD = 5,
	RCC_GPIOE = 6,
}CLOCK_GPIO_Type;

void Clock_Gpio_init(CLOCK_GPIO_Type Gpiox);

void Clock_Can_init();

#endif /* RCC_H_ */
