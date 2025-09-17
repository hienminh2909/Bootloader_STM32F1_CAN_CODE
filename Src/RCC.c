/*
 * RCC.c
 *
 *  Created on: Sep 11, 2025
 *      Author: Admin
 */
#include<RCC.h>
#include<GPIO_Registers.h>
void Clock_Gpio_init(CLOCK_GPIO_Type Gpiox)

{
	RCC->APB2ENR |= (1u<<Gpiox);
}

void Clock_Can_init()
{

    RCC->APB1ENR |= (1u<<25u);                       /*Enable  CAN1*/
	RCC->APB2ENR |= (1u<<2u);                        /*Enable  GPIOA*/

	GPIOA->CRH   &= ~(0xFu<<12u);                    /*Clear CNF,MODE cua GPIOA_11*/
	GPIOA->CRH   |=  (0x4u<<12u);                    /* CNF=01: , MODE =00 :Floating Input GPIOA_11*/

	GPIOA->CRH   &= ~(0xFu<<16u);                    /*Clear CNF,MODE cua GPIOA_12*/
	GPIOA->CRH   |=  (0xBu<<16u);                    /* CNF=10:Alternate function output Push-pull , MODE =11 :Output 50 MHz. GPIOA_12*/
}
