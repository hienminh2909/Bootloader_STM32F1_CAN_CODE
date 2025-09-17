/*
 * Flash.c
 *
 *  Created on: Aug 16, 2025
 *      Author: Admin
 */
#include<FLASH.h>

#define TIMEOUT 1000000

void Flash_Unlock(void)
{
	if((FLASH->CR & (1<<7)) != 0)
	{
		FLASH->KEYR = FLASH_KEY_1;
		FLASH->KEYR = FLASH_KEY_2;
	}
}

void Flash_Lock(void)
{
    FLASH->CR |= (1<<7);
}

void Flash_Erase_Page(uint32_t page_addr)
{
	while((FLASH->SR & (1<<0)) != 0);
	FLASH->CR |= (1<<1);
	FLASH->AR = page_addr;
	FLASH->CR |= (1<<6);
	while((FLASH->SR & (1<<0)) != 0);
	FLASH->CR &= ~(1u<<1u);
}

void Flash_Write_HalfWord(uint32_t addr, uint16_t data )
{
	while ((FLASH->SR & (1<<0)) != 0);
	FLASH->CR |= (1<<0); // PG
	*(volatile uint16_t*)addr = data;
	while ((FLASH->SR & (1<<0)) != 0);
	FLASH->CR &= ~(1u<<0u); // Clear PG

}

uint16_t Flash_Read_HalfWord(uint32_t addr)
{
	uint16_t data = *(volatile uint16_t*)addr;
	return data;
}
