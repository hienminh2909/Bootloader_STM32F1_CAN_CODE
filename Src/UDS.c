/*
 * UDS.c
 *
 *  Created on: Sep 9, 2025
 *      Author: Admin
 */

#include<UDS.h>



uint16_t storedSeed = 0;   // ECU tạo seed

static uint8_t  securityUnlocked = 0;
static uint8_t  flag_write_flash=0;
static uint8_t  flag_flash_erase = 0;

uint32_t addr_current = 0;

static uint32_t size_firmware= 0;
extern uint32_t cantp_received;

UDS_Cofig_Tpye UDS_Config;
void UDS_Init(UDS_Cofig_Tpye* Config)
{
	UDS_Config.BLOCK_SIZE = Config->BLOCK_SIZE;
	UDS_Config.STORE_SEED = Config->STORE_SEED;
	UDS_Config.TX_CAN_ID  = Config->TX_CAN_ID;
}

void UDS_Process(uint8_t *data, uint32_t len)
{
    if(len==0) return;
    uint8_t sid = data[0];

    switch (sid)
    {
    case 0x10:
    {
        if(len < 2)
        {
            UDS_SendNeg(sid,0x13);
            return;
        }
        flag_flash_erase = 0;
        uint8_t sub = data[1];
        uint8_t resp[1] = {sub};
        UDS_SendPos(sid,resp,1);  //{0x02, 0x50, 0x02,0,0,0,0,0};
        break;
    }
    case 0x27:
	{
    	uint8_t subFunc = data[1];
    	if(subFunc == 0x01)
    	{
    		storedSeed = UDS_Generate_Seed(0x1234);
    		uint8_t sub = data[1];
    		uint8_t resp[3] = {sub,(uint8_t)((storedSeed >> 8) & 0xFF), (uint8_t)(storedSeed & 0xFF)};
    		UDS_SendPos(sid, resp, 3);
    	}
    	else if(subFunc == 0x02)
    	{
    		uint16_t key = (data[2] << 8) | data[3];
    		uint16_t expectedKey = UDS_Calculate_Key(storedSeed);
            if (key == expectedKey) {
                securityUnlocked = 1;
                uint8_t sub = data[1];
                uint8_t resp[1] = {sub};
                UDS_SendPos(sid, resp, 1);
            }
            else {
            	securityUnlocked = 0;
                UDS_SendNeg(sid, 0x13);
            }
    	}

    	flag_flash_erase = 0;
    	break;

	}
    case 0x34:
    {
        uint8_t alfid = data[2];
        uint32_t size_len = alfid;

        if(securityUnlocked == 0)
        {
        	UDS_SendNeg(sid,0x13);
        	return;
        }

        flag_write_flash =1;
        //Check size length
        if((size_len == 0) || (size_len + 3 > len))
        {
            UDS_SendNeg(sid,0x13);
            return;
        }

        // Read size firmware
        uint32_t size;
        uint8_t idx=3;
        for(uint32_t i=0; i<size_len;i++)
        {
            size = (size << 8) | data[idx++];
        }

        //Check size firmware
        if(size==0 || size > flmg_config.MAX_APP_SIZE)
        {
        	UDS_SendNeg(sid,0x13);
            return;
        }
        size_firmware = size;

        // Erase Flash
        if(securityUnlocked == 1)
        {
            for (uint32_t page = flmg_config.APP_ADDR; page <= flmg_config.APP_ADDR + size; page += 1024)
            {
                FLMG_Eraser_Page(page);
            }
            flag_flash_erase = 1;
        }
        else
        {
        	UDS_SendNeg(sid,0x13);
        	return;
        }
        addr_current = flmg_config.APP_ADDR;

        uint8_t resp[2] = {(uint8_t)(UDS_Config.BLOCK_SIZE >>8 & 0xFF), (uint8_t) UDS_Config.BLOCK_SIZE & 0xFF};
        UDS_SendPos(sid, resp, 2);      //{0x02, 0x74, BLOCK_SIZE,0,0,0,0,0};
        break;

    }

    case 0x36:
    {
    	if(securityUnlocked == 0 || flag_write_flash ==0)
    	{
    	    UDS_SendNeg(sid, 0x13);
    	    return;
    	}

    	if(flag_write_flash==1 && securityUnlocked==1 && flag_flash_erase == 1)
    	{
    	    for (uint32_t i = 0; i <(cantp_received-2); i += 2)
    	    {
    		    uint16_t Data = data[i+2] | (data[i+3] << 8);
    		    FLMG_Write_HalfWord(addr_current + i, Data);
    		}
    	    //GPIO_Toggle(GPIOC, 13);
    	    addr_current += UDS_Config.BLOCK_SIZE;
    	    uint8_t resp[1] = {data[1]};
    	    UDS_SendPos(sid, resp, 1);
        }
    	else
    	{
    		 UDS_SendNeg(sid, 0x13);
    		 return;
    	}


    	break;
    }

    case 0x37:
    {
    	uint32_t crc_from_pc = (data[1]<<24) | (data[2]<<16) | (data[3]<<8) | data[4];
        // Tính CRC32 trong Flash
    	uint32_t crc_flash = UDS_CRC32_Flash(flmg_config.APP_ADDR, size_firmware);

        if (crc_from_pc == crc_flash)
        {
            FLMG_Eraser_Page(flmg_config.FLAG_APP_AVAIL_ADDR);
    	    FLMG_Write_HalfWord(flmg_config.FLAG_APP_AVAIL_ADDR, 0x0001);
    	    FLMG_Write_HalfWord(flmg_config.FLAG_UPDATE_FIRM_ADDR, 0x0000);

        	uint8_t resp[1] = {0x00};
        	UDS_SendPos(sid, resp, 1);
        }
        else
        {
    	    FLMG_Eraser_Page(flmg_config.FLAG_APP_AVAIL_ADDR);
    	    FLMG_Write_HalfWord(flmg_config.FLAG_APP_AVAIL_ADDR, 0x0000);
    	    FLMG_Write_HalfWord(flmg_config.FLAG_UPDATE_FIRM_ADDR, 0x0001);
        	UDS_SendNeg(sid, 0x13);
        }
	    flag_write_flash=0;
	    flag_flash_erase = 0;
	    securityUnlocked = 0;

    }
    default:
        break;
    }
}

void UDS_SendNeg(uint8_t req_sid, uint8_t nrc)
{
    uint8_t pdu[3];
    pdu[0] = 0x7F;
    pdu[1] = req_sid;
    pdu[2] = nrc;
    CANTP_Send(UDS_Config.TX_CAN_ID, pdu, 3);
}


void UDS_SendPos(uint8_t sid, uint8_t *data, uint8_t len)
{
    uint8_t pdu[64];
    pdu[0] = (sid+0x40);
    if(len) memcpy(&pdu[1], data,len);
    CANTP_Send(UDS_Config.TX_CAN_ID, pdu, len+1);
}


uint16_t UDS_Generate_Seed(uint16_t key) {
    return key; // ví dụ cố định, bạn có thể đổi thành random
}

// Thuật toán tính Key từ Seed
uint16_t UDS_Calculate_Key(uint16_t seed) {
    return seed ^ 0xAAAA;
}

uint32_t crc32_table[256];

// Khởi tạo bảng tra CRC32
void UDS_CRC32_Init(void) {
    uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (uint32_t j = 0; j < 8; j++) {
            if (c & 1)
                c = polynomial ^ (c >> 1);
            else
                c >>= 1;
        }
        crc32_table[i] = c;
    }
}

// Tính CRC32 trên buffer
/*
static uint32_t CRC32_Calculate(uint8_t *data, uint32_t length) {
    uint32_t c = 0xFFFFFFFF;
    for (uint32_t i = 0; i < length; i++) {
        uint8_t index = (uint8_t)((c ^ data[i]) & 0xFF);
        c = crc32_table[index] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFF;
}
*/

// Tính CRC32 trực tiếp trong Flash (APP_ADDRESS → APP_ADDRESS+size)
uint32_t UDS_CRC32_Flash(uint32_t start_addr, uint32_t size) {
    uint32_t c = 0xFFFFFFFF;
    for (uint32_t i = 0; i < size; i++) {
        uint8_t data = *(volatile uint8_t *)(start_addr + i);
        uint8_t index = (uint8_t)((c ^ data) & 0xFF);
        c = crc32_table[index] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFF;
}

