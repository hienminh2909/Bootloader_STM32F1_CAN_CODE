/*
 * FLASH.h
 *
 *  Created on: Aug 16, 2025
 *      Author: Admin
 */

#ifndef FLASH_H_
#define FLASH_H_

#include<Flash_Registers.h>
#include<stdint.h>

void Flash_Unlock(void);
void Flash_Lock(void);
void Flash_Erase_Page(uint32_t page_addr);
void Flash_Write_HalfWord(uint32_t addr, uint16_t data );
uint16_t Flash_Read_HalfWord(uint32_t addr);

#endif /* FLASH_H_ */
