#include "Service/ServiceLayer.h"
#include "HAL/ADC_Driver.h"
#include <stdio.h>
#include "MKL25Z4.h"

void ADC_vfnDriverInit(void){
        //Clock Gate to ADC
        	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);
        //Clock Gate to Port E
            SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
        //Single Ended
            ADC0->SC1[0] &=~ ADC_SC1_DIFF(1);
        //8 Bits resolution
            ADC0->CFG1 &=~ ADC_CFG1_MODE(1);
        //Normal power configuration
            ADC0->CFG1 &=~ ADC_CFG1_ADLPC(1);
        //Normal Conversion Sequence
            ADC0->CFG2 &=~ ADC_CFG2_ADHSC(1);
        //Conversion trigger shall be via software
            ADC0->SC2 &=~ ADC_SC2_ADTRG(1);
        //DMA disabled
            ADC0->SC2 &=~ ADC_SC2_DMAEN(1);
        //Enables the hardware average function of the ADC.
            ADC0->SC3 |= ADC_SC3_AVGE(1);
        //The hardware average shall be selected with at least 16 samples per each average
            ADC0->SC3 |= ADC_SC3_AVGS(2);
        //Select channel
            ADC0->SC1[0] &= ADC_SC1_ADCH(ADCH);
}

uint_8 ADC_bfnReadADC (uint_8 *bpADCValue){
	uint_8 bStatus = false;
	if(ADC0->SC1[0] & ADC_SC1_COCO_MASK)
	{
		//read
		*bpADCValue = (uint_8)ADC0->R[0];
		// this will clear the COCO bit that is also the interrupt flag
		ADC0->SC1[0] |= ADC_SC1_COCO_MASK;
		bStatus = true;
	}
	return bStatus;
}
