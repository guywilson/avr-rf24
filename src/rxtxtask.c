#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "scheduler.h"
#include "serial_atmega328p.h"
#include "taskdef.h"
#include "wdttask.h"
#include "rxtxtask.h"
#include "version.h"

char				szBuffer[80];
uint32_t			idleCount = 0;
uint32_t			busyCount = 0;
uint32_t			messageCount = 0;

void RxTask(PTASKPARM p) {
	PRXMSGSTRUCT	pMsgStruct;

	pMsgStruct = (PRXMSGSTRUCT)p;

	if (pMsgStruct->rxErrorCode != 0) {
		txNAK(pMsgStruct->frame.msgID, (pMsgStruct->frame.cmd << 4), pMsgStruct->rxErrorCode);
	}
	else {
		switch (pMsgStruct->frame.cmd) {
			case RX_CMD_RF24_PACKET:
				txACKStr(pMsgStruct->frame.msgID, (pMsgStruct->frame.cmd << 4), NULL, 0);
				// txACK(
					// 	pMsgStruct->frame.msgID, 
					// 	(pMsgStruct->frame.cmd << 4), 
					// 	(uint8_t *)(&tph), 
					// 	sizeof(TPH_PACKET));
				break;

			case RX_CMD_PING:
				txACKStr(pMsgStruct->frame.msgID, (pMsgStruct->frame.cmd << 4), NULL, 0);
				break;

			default:
				txNAK(pMsgStruct->frame.msgID, (pMsgStruct->frame.cmd << 4), MSG_NAK_UNKNOWN_CMD);
				break;
		}

		messageCount++;
	}
}
