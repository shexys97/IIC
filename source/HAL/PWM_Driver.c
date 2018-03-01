#include "Service/ServiceLayer.h"
#include "HAL/PWM_Driver.h"
#include <stdio.h>
#include "MKL25Z4.h"
uint_8 cPrevCv;
void PWM_vfnInitDriver(void){
	//Select Clock Source MCGFLLCLK
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	//SIM->SOPT2 = SIM_SOPT2_PLLFLLSEL(1);
	//Clock Gate to TPM 0
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
	//Clock gate to ports
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1);
	//Alternative 4 to TPM0 Port C Pin 1
	PORTC->PCR[1] |= PORT_PCR_MUX(4);
	//Disable the LPTPM to modify the registers
	//Channel Mode Select MSB-MSA 10 ElSB-ElSA 10
	TPM0->CONTROLS[0].CnSC = PWM_CENTER_ALIGN_CONFIG;
	//Initialize counter on 0
	TPM0->CNT = 0;
	//Set the max counter
	TPM0->MOD = PWM_MOD;
	//Set the offset
	TPM0->CONTROLS[0].CnV = 0;
	// Center-aligned Pre scale 64 Enable the LPTPM
	TPM0->SC = PWM_SC_CONFIG;
}

void PWM_vfnSetCV(uint_8 *bpCV, uint_8 *bpOp){
	cPrevCv = TPM0->CONTROLS[0].CnV;
	if(*bpOp){
		while(cPrevCv <= *bpCV){
			TPM0->CONTROLS[0].CnV += PWM_SPEED;
			cPrevCv = TPM0->CONTROLS[0].CnV;
		}
	}else{
		while(cPrevCv >= *bpCV){
			TPM0->CONTROLS[0].CnV -= PWM_SPEED;
			cPrevCv = TPM0->CONTROLS[0].CnV;
		}
	}
}

