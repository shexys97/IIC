#include "Service/ServiceLayer.h"
#include "HIL/AnalogGauge.h"
#include "HAL/PWM_Driver.h"
#include <stdio.h>
#include "MKL25Z4.h"
static uint_8 cPrevTemp = 0;
void GAUGE_vfnSubSysInit(void){
	PWM_vfnInitDriver();
}

void GAUGE_vfnSendCV(uint_8 *bpTemp){
	uint_8 bOp = false;
	if(cPrevTemp > *bpTemp){
		bOp = false;
	}else{
		bOp = true;
	}
	uint_8 bpCV = GAUGE_bfnTempToCV(bpTemp);
	//TODO call pwm driver
	PWM_vfnSetCV(&bpCV, &bOp);
	cPrevTemp = *bpTemp;
}

uint_8 GAUGE_bfnTempToCV(uint_8 *bpTemp){
	uint_8 bpCV = ((*bpTemp * GAUGE_MAX_CV) / GAUGE_MAX_TEMPERATURE) + GAUGE_MIN_CV ;
	return bpCV;
}
