/*
 * CAN.c
 *
 *  Created on: Aug 15, 2025
 *      Author: Admin
 */
#include<CAN.h>
#include<RCC_Registers.h>
//#include<GPIO_Registers.h>

#define TX_TIMEOUT 1000000

void CAN_Init(const BAUDRATE_CAN_Type* Config_Baurate)
{
	//RCC->APB1ENR |= (1<<25);                       /*Enable  CAN1*/
	//RCC->APB2ENR |= (1<<2);                        /*Enable  GPIOA*/

	//GPIOA->CRH   &= ~(0xF<<12);                    /*Clear CNF,MODE cua GPIOA_11*/
	//GPIOA->CRH   |=  (0x4<<12);                    /* CNF=01: , MODE =00 :Floating Input GPIOA_11*/

	//GPIOA->CRH   &= ~(0xF<<16);                    /*Clear CNF,MODE cua GPIOA_12*/
	//GPIOA->CRH   |=  (0xB<<16);                    /* CNF=10:Alternate function output Push-pull , MODE =11 :Output 50 MHz. GPIOA_12*/

    CAN_CASR->MCR |= (1<<0);
    uint32_t timeout = TX_TIMEOUT;
    while (!(CAN_CASR->MSR & (1<<0)))
    {
    	if (timeout >0)
    	{
    		timeout--;
    	}
    	else
    	{
    		break;
    	}
    }
    /*CAN_CASR->BTR = (~(0x000003ff << 0)) & (~(0x0000000f<<16)) & (~(0x00000007<<16)) & (~(0x00000003<<16)); // CLEAR BAURATE */

    /*Config BAUDRATE*/
    CAN_CASR->BTR = (((Config_Baurate->PRESCALER) -1) << 0) | (((Config_Baurate->BS2)-1)<<16)
    		        | (((Config_Baurate->BS1)-1)<<20) | (((Config_Baurate->SJW)-1)<<24);


}


void CAN_ConfigFilter(FilterBank_Type filterBank, uint32_t id, uint32_t mask,FIFO_Type fifoAssignment, Scale_Type scale, Mode_Type mode)
{

	if((CAN_CASR->MSR & (1<<0)) == 0)       /* Kiem tra che do Initialization*/
	{
		CAN_CASR->MCR |= (1<<0);            /* Kich hoat vao che do Initialization */
	}


	CAN_FILR->FMR  |= (1<<0);                 // FINIT = 1 (Vao che do cau hinh filter)
	CAN_FILR->FA1R &= ~(1u<<filterBank);       // Disable filter trước khi config

	// SCALE
	if(scale == 1)
	{
		CAN_FILR->FS1R |= (1<<filterBank);    //32 BIT SINGLE
	}
	else
	{
		CAN_FILR->FS1R &= ~(1u<<filterBank);   //16 BIT DUAL
	}

	// MODE
    if(mode == 1)
    {
    	CAN_FILR->FM1R |= (1<<filterBank);    //LIST MODE
    }
    else
    {
    	CAN_FILR->FM1R &= ~(1u<<filterBank);   //MASK MODE
    }

    // FIFO
    if(fifoAssignment == 1)
    {
    	CAN_FILR->FFA1R |= (1<<filterBank);    //FIFO1
    }
    else
    {
        CAN_FILR->FFA1R &= ~(1u<<filterBank);   //FIFO0
    }

    // Ghi ID và MASK vào filter register
    // ID chuẩn (11 bit) -> shift 21 bit
    CAN_FILR1->FxRx[filterBank][0] = (id << 21);
    CAN_FILR1->FxRx[filterBank][1] = (mask << 21);

    // Enable filter
    CAN_FILR->FA1R |= (1<<filterBank);

    // Thoát init mode
    CAN_FILR->FMR  &= ~(1u<<0);
    CAN_CASR->MCR &= ~(1u<<0);

     // Yêu cầu thoát sleep
    CAN_CASR->MCR &= ~(1u << 1); // Clear bit SLEEP
}



void CAN_ConfigTransmit(uint32_t StdId, uint32_t ExtId, IDE_Type IDE, RTR_Type RTR, Mailbox_Type Mailbox )
{
	//RTR
	if(RTR == 1) // Remote frame
	{
		if(Mailbox ==1)
		{
			CAN_MBR->TI1R |= (1<<1);
		}
		else if (Mailbox ==2)
		{
			CAN_MBR->TI2R |= (1<<1);
		}
		else
		{
			CAN_MBR->TI0R |= (1<<1);
		}
	}
	else // Data frame
	{
		if(Mailbox ==1)
		{
			CAN_MBR->TI1R &= ~(1u<<1u);
		}
		else if (Mailbox ==2)
		{
			CAN_MBR->TI2R &= ~(1u<<1u);
		}
		else
		{
			CAN_MBR->TI0R &= ~(1u<<1u);
		}
	}


	// IDE
	if(IDE == 1)  //  Extended identifier
	{
		CAN_MBR->TI0R |= (1<<2);
		CAN_MBR->TI0R &= ~(0x1FFFFFFFu << 3u);

		if(Mailbox == 1)
		{
			CAN_MBR->TI1R |= (1<<2);  // IDE
		    CAN_MBR->TI1R &= ~(0x1FFFFFFFu << 3u);  // CLEAR ID
			CAN_MBR->TI1R |= (ExtId << 3);
		}
		else if(Mailbox == 2)
		{
			CAN_MBR->TI2R |= (1<<2);  // IDE
			CAN_MBR->TI2R &= ~(0x1FFFFFFFu << 3u);  // CLEAR ID
			CAN_MBR->TI2R |= (ExtId << 3);
	    }
		else{
			CAN_MBR->TI0R |= (1<<2);  // IDE
			CAN_MBR->TI0R &= ~(0x1FFFFFFFu << 3u);  // CLEAR ID
			CAN_MBR->TI0R |= (ExtId << 3);
		}
	}

	else //Standard identifier
	{
		if(Mailbox == 1)
		{
			CAN_MBR->TI1R &= ~(1u<<2u);
			CAN_MBR->TI1R &= ~(0x7FFu << 21u);
			CAN_MBR->TI1R |=  (StdId << 21);
		}
		else if(Mailbox == 2)
		{
			CAN_MBR->TI2R &= ~(1u<<2u);
		    CAN_MBR->TI2R &= ~(0x7FFu << 21u);
		    CAN_MBR->TI2R |=  (StdId << 21);CAN_MBR->TI0R &= ~(0x1FFFFFFFu << 3u);
		}
		else{
			CAN_MBR->TI0R &= ~(1u<<2u);
			CAN_MBR->TI0R &= ~(0x7FFu << 21u);
			CAN_MBR->TI0R |=  (StdId << 21);
		}
	}

}


void CAN_Transmit(uint32_t id, uint8_t *data, uint8_t length, Mailbox_Type Mailbox)
{
	if (CAN_CASR->TSR & (1 << (26+Mailbox)))
    { // TMEx = 1 (Mailbox x trống)

		if(Mailbox == 1)// Gửi dữ liệu
	    {
	    	CAN_MBR->TI1R &= ~(0x3u<<1u);
	    	CAN_MBR->TI1R &= ~(0x7FFu<<21u);// clear ID
	    	CAN_MBR->TI1R |= ((id & 0x7FF)<<21); // ghi ID

	    	CAN_MBR->TDT1R &= ~(0xFu); // clear do dai du lieu
	    	CAN_MBR->TDT1R |= (length & 0xFu); // ghi do dai du lieu

		// GHI DU LIEU 8 BYTE
	    	CAN_MBR->TDL1R = (data[3]<<24) | (data[2]<<16) | (data[1]<<8) | (data[0]);
	    	CAN_MBR->TDH1R = (data[7]<<24) | (data[6]<<16) | (data[5]<<8) | (data[4]);

	        CAN_MBR->TI1R |= (0x1<<0); // YEU CAU GUI DU LIEU

	    // Chờ gửi xong
	       uint32_t timeout = TX_TIMEOUT;
	       while ((CAN_CASR->TSR & (1 << 8)) == 0) // RQCP1=1: request complete
	       {
	    	   timeout--;
	    	   if(timeout <= 0)
	    	   {
	    	   	   break;
	    	   }
	       }
	       CAN_CASR->TSR |= (1 << 8); // Clear RQCP1
	    }

	    else if(Mailbox == 2)
	    {
	    	CAN_MBR->TI2R &= ~(0x3u<<1u);
	    	CAN_MBR->TI2R &= ~(0x7FFu<<21u);// clear ID
	    	CAN_MBR->TI2R |= ((id & 0x7FF)<<21); // ghi ID

	    	CAN_MBR->TDT2R &= ~(0xFu); // clear do dai du lieu
	    	CAN_MBR->TDT2R |= (length & 0xF); // ghi do dai du lieu

	    	// GHI DU LIEU 8 BYTE
	    	CAN_MBR->TDL2R = (data[3]<<24) | (data[2]<<16) | (data[1]<<8) | (data[0]);
	    	CAN_MBR->TDH2R = (data[7]<<24) | (data[6]<<16) | (data[5]<<8) | (data[4]);

	    	CAN_MBR->TI2R |= (0x1<<0); // YEU CAU GUI DU LIEU

	    		    // Chờ gửi xong
	    	uint32_t timeout = TX_TIMEOUT;
	    	while ((CAN_CASR->TSR & (1 << 16)) == 0)// RQCP2=1: request complete
	    	{
	    		timeout--;
	    		if(timeout <= 0)
	    		{
	    			break;
	    		}
	    	}
	    	CAN_CASR->TSR |= (1 << 16); // Clear RQCP2
	    }

	    else
	    {
	    	CAN_MBR->TI0R &= ~(0x3u<<1u);
	    	CAN_MBR->TI0R &= ~(0x7FFu<<21u);// clear ID
	    	CAN_MBR->TI0R |= ((id & 0x7FF)<<21); // ghi ID

	    	CAN_MBR->TDT0R &= ~(0xFu); // clear do dai du lieu
	    	CAN_MBR->TDT0R |= (length & 0xF); // ghi do dai du lieu

	    	// GHI DU LIEU 8 BYTE
	    	CAN_MBR->TDL0R = (data[3]<<24) | (data[2]<<16) | (data[1]<<8) | (data[0]);
	    	CAN_MBR->TDH0R = (data[7]<<24) | (data[6]<<16) | (data[5]<<8) | (data[4]);

	    	CAN_MBR->TI0R |= (0x1<<0); // YEU CAU GUI DU LIEU

	    	 // Chờ gửi xong
	    	uint32_t timeout = TX_TIMEOUT;
	    	 while ((CAN_CASR->TSR & (1 << 0)) == 0) // RQCP2=1: request complete
	    	 {
	    		 timeout--;
	    		 if(timeout <= 0)
	    		 {
	    			 break;
	    		 }

	    	 }
	    	 CAN_CASR->TSR |= (1 << 0); // Clear RQCP2
	    }
	}
}

void CAN_Receive(uint32_t *id, uint8_t *data, uint8_t *length, FIFO_Type fifox)
{
  if(fifox == 1)
  {
	if(CAN_CASR->RF1R & (0x3<<0))
	{
			*id     = (CAN_MBR->RI1R >>21) & 0x7FF;
			*length = (CAN_MBR->RDT1R & 0xF);
			data[0] = (CAN_MBR->RDL1R>>0) &0xFF;
			data[1] = (CAN_MBR->RDL1R>>8) &0xFF;
			data[2] = (CAN_MBR->RDL1R>>16) &0xFF;
			data[3] = (CAN_MBR->RDL1R>>24) &0xFF;
			data[4] = (CAN_MBR->RDH1R>>0) &0xFF;
			data[5] = (CAN_MBR->RDH1R>>8) &0xFF;
			data[6] = (CAN_MBR->RDH1R>>16) &0xFF;
			data[7] = (CAN_MBR->RDH1R>>24) &0xFF;

			CAN_CASR->RF1R |= (1<<5);

	}
  }
  else
  {
	  if(CAN_CASR->RF0R & (0x3<<0))
	  	{
	  			*id     = (CAN_MBR->RI0R >>21) & 0x7FF;
	  			*length = (CAN_MBR->RDT0R & 0xF);
	  			data[0] = (CAN_MBR->RDL0R>>0) &0xFF;
	  			data[1] = (CAN_MBR->RDL0R>>8) &0xFF;
	  			data[2] = (CAN_MBR->RDL0R>>16) &0xFF;
	  			data[3] = (CAN_MBR->RDL0R>>24) &0xFF;
	  			data[4] = (CAN_MBR->RDH0R>>0) &0xFF;
	  			data[5] = (CAN_MBR->RDH0R>>8) &0xFF;
	  			data[6] = (CAN_MBR->RDH0R>>16) &0xFF;
	  			data[7] = (CAN_MBR->RDH0R>>24) &0xFF;

	  			CAN_CASR->RF0R |= (1<<5);

	  	}
  }
}

uint8_t CAN_Available(void)
{

	if((CAN_CASR->RF0R & (0x3<<0)) == 1) return 1;
	else return 0;
}
