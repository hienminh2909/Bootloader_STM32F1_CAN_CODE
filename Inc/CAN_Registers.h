

#ifndef CAN_REGISTERS_H
#define CAN_REGISTERS_H

#include <stdint.h>
#define CAN_BASE        (0x40006400)

/* CAN CONTROL AND STATUS REGISTERS */



typedef struct{
	volatile uint32_t MCR;
	volatile uint32_t MSR;
	volatile uint32_t TSR;
	volatile uint32_t RF0R;
	volatile uint32_t RF1R;
	volatile uint32_t IER;
	volatile uint32_t ESR;
	volatile uint32_t BTR;

}CAN_CASR_Type;
#define CAN_CASR ((CAN_CASR_Type *)CAN_BASE)


typedef struct{
	volatile uint32_t TI0R;
	volatile uint32_t TDT0R;
	volatile uint32_t TDL0R;
	volatile uint32_t TDH0R;
	volatile uint32_t TI1R;
	volatile uint32_t TDT1R;
	volatile uint32_t TDL1R;
	volatile uint32_t TDH1R;
	volatile uint32_t TI2R;
	volatile uint32_t TDT2R;
	volatile uint32_t TDL2R;
	volatile uint32_t TDH2R;

	volatile uint32_t RI0R;
	volatile uint32_t RDT0R;
	volatile uint32_t RDL0R;
	volatile uint32_t RDH0R;
	volatile uint32_t RI1R;
	volatile uint32_t RDT1R;
	volatile uint32_t RDL1R;
	volatile uint32_t RDH1R;


}CAN_MBR_Type;
#define CAN_MBR   ((CAN_MBR_Type *)(CAN_BASE + 0x180))



typedef struct{
	volatile uint32_t FMR;
	volatile uint32_t FM1R;
	volatile uint32_t RESERVED;
	volatile uint32_t FS1R;
	volatile uint32_t RESERVED1;
	volatile uint32_t FFA1R;
	volatile uint32_t RESERVED2;
	volatile uint32_t FA1R;

}CAN_FILR_Type;
#define CAN_FILR ((CAN_FILR_Type *)(CAN_BASE + 0x200))


typedef struct{
	volatile uint32_t FxRx[28][2];
}CAN_FILR1_Type;
#define CAN_FILR1 ((CAN_FILR1_Type *)(CAN_BASE + 0x240))










#endif
