/*
 * CANTP.c
 *
 *  Created on: Sep 9, 2025
 *      Author: Admin
 */
#include<CANTP.h>


uint8_t CANTP_Buffer[1028];
uint8_t CANTP_Buflen;

uint32_t cantp_received=0;
uint32_t cantp_expected_length=0;
uint8_t  currentSN = 1;  // Sequence Number (dùng cho CF)
uint8_t  nextSN = 0;

void CANTP_Send_Flow_Control(uint32_t tx_id, uint8_t flow_status, uint8_t bs, uint8_t stmin)
{
    uint8_t fc[8] = {0};
    fc[0] = (0x3 << 4) | (flow_status & 0x0F);
    fc[1] = bs;
    fc[2] = stmin;
    CAN_Transmit(tx_id,fc, 8, MAILBOX_0);
}


void CANTP_Handler(uint8_t *data, uint8_t length)
{
   uint8_t pci = data[0] & 0xF0;


   if(pci == 0x00)  //single frame
   {
       uint8_t sf_len = data[0] & 0x0F;
       if(sf_len > length -1 ) sf_len = length-1;

       CANTP_Buflen = sf_len;
       memcpy(&CANTP_Buffer, &data[1],sf_len);

       UDS_Process(CANTP_Buffer,CANTP_Buflen);

   }

   else if(pci == 0x10)  //Fist Frame
   {

        uint16_t total_len = ((data[0] & 0x0F) << 8) | data[1];
        cantp_expected_length = total_len;
        cantp_received = 0;
        nextSN = 1;

        memcpy(&CANTP_Buffer[0], &data[2],6);
        cantp_received += 6;

        CANTP_Send_Flow_Control(0x7E8, 0x0, 0x00, 0x00);
   }

   else if (pci == 0x20)
   {
        // Consecutive Frame
        uint8_t sn = data[0] & 0x0F;
        if(sn != nextSN)
        {
            cantp_expected_length =0;
            cantp_received = 0;
            UDS_SendNeg(data[0], 0x13);
            return;
        }

        nextSN = (nextSN +1) & 0x0F;
        if(nextSN==0) nextSN =1;

        //uint8_t copy_len = (cantp_received + 7 <= cantp_expected_length) ? 7 : (cantp_received - cantp_expected_length);
        memcpy(&CANTP_Buffer[cantp_received], &data[1],7);
        cantp_received += 7;

        if(cantp_received >= cantp_expected_length)
        {
             UDS_Process(CANTP_Buffer, cantp_received);   //{0x02,0x76,seq_block,0,0,0,0,0};
        }
   }

   else
   {
	   return;
   }
}


void CANTP_Send(uint32_t tx_id, uint8_t *pdu, uint32_t len)
{
    uint8_t frame[8];

    if (len <= 7) {
        frame[0] = (0x00) | (uint8_t)(len & 0x0F);
        memset(&frame[1], 0x00, 7);
        memcpy(&frame[1], pdu, len);
        CAN_Transmit(tx_id, frame, 8, MAILBOX_0);
        return;
    }

    CAN_Transmit(tx_id, frame, 8, MAILBOX_0);

    /* For simplicity, don't wait for FC (could be improved) */
    uint32_t pos = 6;
    uint8_t seq = 1;
    while (pos < len) {
        uint8_t cf[8];
        cf[0] = (0x2 << 4) | (seq & 0x0F);
        uint32_t chunk = (len - pos > 7) ? 7 : (len - pos);
        memset(&cf[1], 0x00, 7);
        memcpy(&cf[1], &pdu[pos], chunk);
        //CAN_Transmit(0x7E8, cf, 8, MAILBOX_0);
        pos += chunk;
        seq = (seq + 1) & 0x0F;

    }
}


