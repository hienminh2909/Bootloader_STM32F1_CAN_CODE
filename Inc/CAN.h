
#ifndef CAN_H_
#define CAN_H_
#include<stdint.h>
#include<CAN_Registers.h>

typedef enum{
    	SCALE_16BIT = 0,
		SCALE_32BIT = 1
}Scale_Type;

typedef enum{
    	MASK_MODE = 0,
		LIST_MODE = 1
}Mode_Type;

typedef enum{
    	STD_IDE = 0,
		EXT_IDE = 1
}IDE_Type;

typedef enum{
    	DATA_FRAME = 0,
		REMOTE_FRAME = 1
}RTR_Type;

typedef enum{
    	MAILBOX_0 = 0,
		MAILBOX_1 = 1,
 		MAILBOX_2 = 2
}Mailbox_Type;

typedef enum{
    	FIFO_0 = 0,
		FIFO_1 = 1
}FIFO_Type;

typedef enum{
    	FILTER_BANK_0 = 0,
		FILTER_BANK_1 = 1,
		FILTER_BANK_2,
		FILTER_BANK_3,
		FILTER_BANK_4,
		FILTER_BANK_5,
		FILTER_BANK_6,
		FILTER_BANK_7,
		FILTER_BANK_8,
		FILTER_BANK_9,
		FILTER_BANK_10,
		FILTER_BANK_11,
		FILTER_BANK_12,
		FILTER_BANK_13,
}FilterBank_Type;

typedef struct{
	uint16_t PRESCALER;
	uint8_t  SJW;
	uint8_t  BS1;
	uint8_t  BS2;

}BAUDRATE_CAN_Type;

void CAN_Init(const BAUDRATE_CAN_Type* Config_Baurate);
void CAN_ConfigFilter(FilterBank_Type filterBank, uint32_t id, uint32_t mask, FIFO_Type fifoAssignment, Scale_Type scale, Mode_Type mode);
void CAN_Start(void);
void CAN_ConfigTransmit(uint32_t StdId, uint32_t ExtId, IDE_Type IDE, RTR_Type RTR, Mailbox_Type Mailbox);
void CAN_Transmit(uint32_t id, uint8_t *data, uint8_t length, Mailbox_Type Mailbox);
void CAN_Receive(uint32_t *id, uint8_t *data, uint8_t *length, FIFO_Type fifox);
uint8_t CAN_Available(void);



#endif /* CAN_H_ */
