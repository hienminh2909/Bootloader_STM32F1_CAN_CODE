/*
 * GPIO.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Admin
 */

#ifndef GPIO_H_
#define GPIO_H_

#include<GPIO_Registers.h>

typedef enum {
	INPUT =0b00,
	OUTPUT_10MHZ=0b01,
	OUTPUT_2MHZ=0b10,
	OUTPUT_50MHZ=0b11,

}GPIO_Mode;

typedef enum{
	INPUT_ANALOG=0b0000,
	INPUT_FLOATING=0b0100,
	INPUT_PD_PU=0b1000,

	OUTPUT_PP_10MHZ=0b0001,
	OUTPUT_PP_2MHZ=0b0010,
	OUTPUT_PP_50MHZ=0b0011,

	OUTPUT_OD_10MHZ=0b0101,
	OUTPUT_OD_2MHZ=0b0110,
	OUTPUT_OD_50MHZ=0b0111,

	AFO_PP_10MHZ=0b1001,
	AFO_PP_2MHZ=0b1010,
	AFO_PP_50MHZ=0b1011,

	AF0_OD_10MHZ=0b1101,
	AF0_OD_2MHZ=0b1110,
	AF0_OD_50MHZ=0b1111
}Config_Mode_Type;

void GPIO_Init(GPIO_Typedef *GPIOx, uint8_t pin, Config_Mode_Type cofig_mode);
void GPIO_Write(GPIO_Typedef *GPIOx, uint8_t pin, uint8_t state);
uint8_t GPIO_Read(GPIO_Typedef *GPIOx, uint8_t pin);
void GPIO_Toggle(GPIO_Typedef *GPIOx, uint8_t pin);

#endif /* GPIO_H_ */
