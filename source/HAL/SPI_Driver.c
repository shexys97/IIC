#include "Service/ServiceLayer.h"
#include "HAL/SPI_Driver.h"
#include <stdio.h>
#include "MKL25Z4.h"



void SPI_vfnInitDriver(void){
	//Clock Gate to SPI
	SIM->SCGC4 |= SIM_SCGC4_SPI0(1);
	//Clock gate to ports
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
	//Chip Select (SPI0_PCS0) Port A Pin 14 alternative 2
	PORTC->PCR[4] |= PORT_PCR_MUX(2);
	//Source Clock (SPI0_SCK) Port A Pin 15 alternative 2
	PORTC->PCR[5] |= PORT_PCR_MUX(2);
	//Master Output Slave Input (SPI0_MOSI) Port A Pin 16 alternative 2
	PORTC->PCR[6] |= PORT_PCR_MUX(2);
	//Master Input Slave Output (SPI0_MISO) Port A Pin 17 alternative 2
	PORTC->PCR[7] |= PORT_PCR_MUX(2);

	//Check if I am a master or Slave
	if(IM_A_MASTER){
		// I am master

		//SPI master enable
		SPI0->C1 |= SPI_C1_MSTR(1);
		//Master mode-fault enable
		SPI0->C2 |= SPI_C2_MODFEN(1);
		//Slave select output
		SPI0->C1 |= SPI_C1_SSOE(1);
	}else{
		// I am slave

		//SPI slave enable
		SPI0->C1 &= ~SPI_C1_MSTR(1);
		//Master mode-fault enable
		SPI0->C2 &= ~SPI_C2_MODFEN(1);
		//Slave select output
		SPI0->C1 &= ~SPI_C1_SSOE(1);
	}
	//Clock Polarity High
	SPI0->C1 &= ~SPI_C1_CPOL(1);
	//Most Significant Bit
	SPI0->C1 &= ~SPI_C1_LSBFE(1);
	//Active SPI System
	SPI0->C1 |= SPI_C1_SPE(1);
	//Baud Rate pre-scale
	SPI0->BR &= ~SPI_BR_SPPR(6);
	//Baud Rate scale
	SPI0->BR &= ~SPI_BR_SPR(4);
}

uint_8 SPI_bfnSendByte(uint_8 *bpData){
	uint_8 bSPIMsgStatus = 0;
	//Check if the buffer is empty
	if((SPI0->S & SPI_S_SPTEF_MASK) && (SPI0->S | SPI_S_MODF_MASK)){
		SPI0->D = *bpData;
		bSPIMsgStatus = 1;
	}
	return bSPIMsgStatus;
}

uint_8 SPI_bfnReadMessage(uint_8 *bpRxData){
	uint_8 bSPIMsgStatus = 0;
	//Check if there is data to read
	if(SPI0->S & SPI_S_SPRF_MASK){
		*bpRxData = (uint_8) SPI0->D;
		bSPIMsgStatus = 1;
	}
	return bSPIMsgStatus;
}
