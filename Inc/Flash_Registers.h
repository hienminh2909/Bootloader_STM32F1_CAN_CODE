#ifndef FLASH_REGISTERS_H_
#define FLASH_REGISTERS_H_

#define FLASH_BASE    (0x40022000)

#define FLASH_RDPRT_KEY     (0x00A5)
#define FLASH_KEY_1         (0x45670123)
#define FLASH_KEY_2         (0xCDEF89AB)


typedef struct{
	volatile unsigned int ACR;
	volatile unsigned int KEYR;
	volatile unsigned int OPTKEYR;
	volatile unsigned int SR;
	volatile unsigned int CR;
	volatile unsigned int AR;
	volatile unsigned int Reserved;
	volatile unsigned int OBR;
	volatile unsigned int WRPR;

}FLASH_Type;

#define FLASH     ((FLASH_Type *)(FLASH_BASE))

#endif
