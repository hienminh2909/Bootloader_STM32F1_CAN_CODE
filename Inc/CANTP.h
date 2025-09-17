/*
 * CANTP.h
 *
 *  Created on: Sep 9, 2025
 *      Author: Admin
 */

#ifndef CANTP_H_
#define CANTP_H_

#include<stdint.h>
#include<CAN.h>
#include<string.h>
#include<UDS.h>


void CANTP_Handler(uint8_t *data, uint8_t length);
void CANTP_Send_Flow_Control(uint32_t tx_id, uint8_t flow_status, uint8_t bs, uint8_t stmin);
void CANTP_Send(uint32_t tx_id, uint8_t *pdu, uint32_t len);


#endif /* CANTP_H_ */
