/*
 * FLASS_MANAGER.c
 *
 *  Created on: Sep 10, 2025
 *      Author: Admin
 */


#include<FLASH.h>
#include <FLASH_MANAGER.h>

FLMG_Config flmg_config;

void FLMG_Init(FLMG_Config* config)
{
	flmg_config.APP_ADDR = config->APP_ADDR;
	flmg_config.FLAG_APP_AVAIL_ADDR = config->FLAG_APP_AVAIL_ADDR;
	flmg_config.FLAG_UPDATE_FIRM_ADDR = config->FLAG_UPDATE_FIRM_ADDR;
	flmg_config.MAX_APP_SIZE  = config->MAX_APP_SIZE;
}

void FLMG_Erase_Page(uint32_t addr_page)
{
	Flash_Unlock();
	Flash_Erase_Page(addr_page);
	Flash_Lock();
}

void FLMG_Write_HalfWord(uint32_t addr, uint16_t data)
{
	Flash_Unlock();
	Flash_Write_HalfWord(addr, data);
	Flash_Lock();
}

uint16_t FLMG_Read_HalfWord(uint32_t addr)
{
	uint16_t data = Flash_Read_HalfWord(addr);
	return data;
}
