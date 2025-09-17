/*
 * UDS.h
 *
 *  Created on: Sep 9, 2025
 *      Author: Admin
 */

#ifndef UDS_H_
#define UDS_H_

#include<stdint.h>
#include<CANTP.h>
#include<string.h>
#include<FLASH_MANAGER.h>
// SEND KEY


typedef struct{
	uint16_t BLOCK_SIZE;
	uint16_t STORE_SEED;
	uint16_t TX_CAN_ID;
}UDS_Cofig_Tpye;

extern UDS_Cofig_Tpye UDS_Config;
void UDS_Init(UDS_Cofig_Tpye* Config);
void UDS_Process(uint8_t *data, uint32_t len);
void UDS_SendNeg(uint8_t req_sid, uint8_t nrc);
void UDS_SendPos(uint8_t sid, uint8_t *data, uint8_t len);
uint16_t UDS_Generate_Seed(uint16_t key);
uint16_t UDS_Calculate_Key(uint16_t seed);

void UDS_CRC32_Init(void);
uint32_t UDS_CRC32_Calculate(uint8_t *data, uint32_t length);
uint32_t UDS_CRC32_Flash(uint32_t start_addr, uint32_t size);

#endif /* UDS_H_ */
