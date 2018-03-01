#include "Service/ServiceLayer.h"
#include "HIL/Communication.h"
#include "stdio.h"
#include "MKL25Z4.h"
#include "HAL/SPI_Driver.h"


void COMM_vfnSubSysInit(void){
	//Initialize SPI Driver
	SPI_vfnInitDriver();
}


uint_8 COMM_bfnSendMessage(uint_8 *data, uint_8 uMsgSize){
	uint_8 bMsgStatus = false;
	bMsgStatus = SPI_bfnSendByte(data);
	return bMsgStatus;
}

uint_8 COMM_bfnReadMessage(uint_8 *bpRxData){
	uint_8 bMsgStatus = 0;
	bMsgStatus = SPI_bfnReadMessage(bpRxData);
	return bMsgStatus;
}

