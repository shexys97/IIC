#include "Service/ServiceLayer.h"
#include <APP/CarTemperature.h>
#include <HIL/Communication.h>
#include <HIL/AnalogGauge.h>
#include <HIL/Temperature.h>
#include <stdio.h>
#include "MKL25Z4.h"

uint_8 msgReceived;
volatile uint_8 max = 0;
volatile int cnt = 10;

int main(void) {
	COMM_vfnSubSysInit();
	GAUGE_vfnSubSysInit();
	uint_8 statusSPI = 0;
	uint_8 msgReceived = 0;
	//uint_8 statusADC = 0;
	//uint_8 sensorData = 0;
	while(1){
		//statusADC = TEMP_bfnReadSensor(&sensorData);
		//statusSPI = COMM_bfnSendMessage(&sensorData, 1);
		statusSPI = COMM_bfnReadMessage(&msgReceived);
		if(statusSPI){
			//printf("%u\n",msgReceived);
			GAUGE_vfnSendCV(&msgReceived);
		}else{
			//printf("No hay\n");
		}
	}

    return 0;
}

