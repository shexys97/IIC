#include "Service/ServiceLayer.h"
#include "HAL/ADC_Driver.h"
#include "HIL/Temperature.h"


void TEMP_vfnSubSysInit(void){
	//Initialize ADC Driver
	ADC_vfnDriverInit();
}

uint_8 TEMP_bfnReadSensor(uint_8 *bpADCValue){
	uint_8 bSensorSatus = false;
	bSensorSatus = ADC_bfnReadADC(bpADCValue);
	return bSensorSatus;
}
