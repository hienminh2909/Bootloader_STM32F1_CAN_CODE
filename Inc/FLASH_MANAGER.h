/*
 * FLASS_MANAGER.h
 *
 *  Created on: Sep 10, 2025
 *      Author: Admin
 */

#ifndef FLASH_MANAGER_H_
#define FLASH_MANAGER_H_


#include<stdint.h>

typedef struct{
	uint32_t APP_ADDR;
	uint32_t FLAG_UPDATE_FIRM_ADDR;
	uint32_t FLAG_APP_AVAIL_ADDR;
	uint16_t MAX_APP_SIZE;
}FLMG_Config;

extern FLMG_Config flmg_config;
void FLMG_Init(FLMG_Config* config);
void FLMG_Erase_Page(uint32_t addr_page);
void FLMG_Write_HalfWord(uint32_t addr, uint16_t data);
uint16_t FLMG_Read_HalfWord(uint32_t addr);

#endif /* FLASH_MANAGER_H_ */
