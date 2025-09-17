/*
 * RCC_Registers.h
 *
 *  Created on: Aug 18, 2025
 *      Author: Admin
 */

#ifndef RCC_REGISTERS_H_
#define RCC_REGISTERS_H_

#define RCC_BASE     (0x40021000)

typedef struct{
    volatile unsigned int CR;
    volatile unsigned int CFGR;
    volatile unsigned int CIR;
    volatile unsigned int APB2RSTR;
    volatile unsigned int APB1RSTR;
    volatile unsigned int AHBENR;
    volatile unsigned int APB2ENR;
    volatile unsigned int APB1ENR;
    volatile unsigned int BDCR;
    volatile unsigned int CSR;
    volatile unsigned int AHBSTR;
    volatile unsigned int CFGR2;
}RCC_Type;

#define RCC     ((RCC_Type *)(RCC_BASE))

#endif /* RCC_REGISTERS_H_ */
