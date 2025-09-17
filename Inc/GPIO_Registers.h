/*
 * GPIO_Registers.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Admin
 */

#ifndef GPIO_REGISTERS_H_
#define GPIO_REGISTERS_H_

#include<stdint.h>
// BASE ADDRESSES

#define PORTA_BASE 0x40010800
#define PORTB_BASE 0x40010C00
#define PORTC_BASE 0x40011000
#define PORTD_BASE 0x40011400

typedef struct {
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;

}GPIO_Typedef;

#define GPIOA ((GPIO_Typedef*)PORTA_BASE)
#define GPIOB ((GPIO_Typedef*)PORTB_BASE)
#define GPIOC ((GPIO_Typedef*)PORTC_BASE)
#define GPIOD ((GPIO_Typedef*)PORTD_BASE)



#endif /* GPIO_REGISTERS_H_ */
