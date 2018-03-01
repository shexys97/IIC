#include "Service/ServiceLayer.h"
#include "HAL/I2C_Driver.h"
#include <stdio.h>
#include "MKL25Z4.h"

void I2C_vfnDriverInit(void){
	//SIM SPI Clock Enable
	SIM->SCGC4 |= SIM_SCGC4_I2C0(1);
	//SIM PORTB
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
	//PORTB PIN 0 SCL
	PORTB->PCR[0] |= PORT_PCR_MUX(2);
	//PORTB PIN 1 SDA
	PORTB->PCR[1] |= PORT_PCR_MUX(2);

	//Pull up to both pins
	PORTB->PRC[0] |= PORT_PCR_PE(1);
	PORTB->PRC[1] |= PORT_PCR_PE(1);
	PORTB->PRC[0] |= PORT_PCR_PS(1);
	PORTB->PRC[1] |= PORT_PCR_PS(1);

	//Mult 2
	I2C0->F |= I2C_F_MULT(1);
	//ICR 0x1F
	I2C0->F |= I2C_F_ICR(0x1F);
}

void I2C_vfnClockConfiguration(void){
	//Clear OUTDIV1 Default value
	SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1(15);
	//Clear OUTDIV4 Default value
	SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4(7);
	//Set OUTDIV1 to Div by 2
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(1);
	//Set OUTDIV4 to Div by 4
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV4(3);


	//Set RANGE0 in C2 to High Fq = 4MHz
	MCG->C2 |= MCG_C2_RANGE0(1);
	//Set HGO0 to High gain operation
	MCG->C2 |= MCG_C2_HGO0(1);
	//Set EREFS0 to indicate the oscillator is in use
	MCG->C2 |= MCG_C2_EREFS0(1);
	//CLKS set ext ref clk as sys clk source
	MCG->C1 |= MCG_C1_CLKS(2);
	//FRDIV set to div by 128 to get in Fq range for FLL
	MCG->C1 |= MCG_C1_FRDIV(2);
	//Set IREFS to select ext ref clk and enable ext osc
	MCG->IREFS &= ~MCG_C1_IREFS(1);

	//fxio pin acelerometro

	//Loop till C2[EREFS0] is intialized
	while(!(MCG->S & MCG_S_OSCINIT0(1)));
	//Loop till ext ref is curr source for ref clk
	while(MCG->S & ~MCG_S_IREFST(0));
	//Loop until ext ref clk is selected to feed MCGOUTCLK
	while(0!=(MCG->S & (MCG_S_CLKST_MASK>>MCG_S_CLKST_SHIFT)));
	//Set pll reference fq to div by 2 = 2MHz
	MCG->C5 |= MCG_C5_PRDIV0(1);
	//TODO CHECK PAGE 396 REF Man

	//Clear OUTDIV1 Default value setting to div by 1
	SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1(15);
	//Clear OUTDIV4 Default value
	SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV4(7);
	//Set OUTDIV4 to Div by 2
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV4(1);
	//Oscillator clock select
	SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL(3);
}

void I2C_vfnReleaseBus(void){
	//Clock enable PortE
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);

	//PORTE 24 y 25
	//MUX => GPIO, Pull select and Pull enable
	PORTE->PCR[24] |= PORT_PCR_MUX(1);
	PORTE->PCR[24] |= PORT_PCR_PE(1);
	PORTE->PCR[24] |= PORT_PCR_PS(1);
	PORTE->PCR[25] |= PORT_PCR_MUX(1);
	PORTE->PCR[25] |= PORT_PCR_PE(1);
	PORTE->PCR[25] |= PORT_PCR_PS(1);

	//GPIO Port E as output each pin
	GPIOE->PDDR |= (1 << I2C_RELEASE_SDA_PIN);
	GPIOE->PDDR |= (1 << I2C_RELEASE_SCL_PIN);

	//GPIO Clear SDA PDOR
	GPIOE->PDOR &= ~(1 << I2C_RELEASE_SDA_PIN);

	//Delay
	I2C_vfnDelay();

	//Train the Acc
	for(int i = 0; i < 9; i++){
		//Clock low
		GPIOE->PDOR &= ~(1 << I2C_RELEASE_SCL_PIN);
		I2C_vfnDelay();
		//SDA High
		GPIOE->PDOR |= (1 << I2C_RELEASE_SDA_PIN);
		I2C_vfnDelay();
		//Clock high
		GPIOE->PDOR |= (1 << I2C_RELEASE_SCL_PIN);
		I2C_vfnDelay();
		I2C_vfnDelay();
	}

	//Send Stop
		//Clock low
		GPIOE->PDOR &= ~(1 << I2C_RELEASE_SCL_PIN);
		I2C_vfnDelay();
		//SDA High
		GPIOE->PDOR |= (1 << I2C_RELEASE_SDA_PIN);
		I2C_vfnDelay();
		//Clock High
		GPIOE->PDOR |= (1 << I2C_RELEASE_SCL_PIN);
		I2C_vfnDelay();
		//SDA High
		GPIOE->PDOR |= (1 << I2C_RELEASE_SDA_PIN);
		I2C_vfnDelay();
}

void I2C_vfnDelay(void){
	for(int i = 0; i < 100; i++){
		__NOP();
	}
}
void vfnCfgPins(void){
	//scl = 24
	//sda = 25
	//Enable PullSelect and PullEnable
	PORTE->PCR[24] |= PORT_PCR_PS(1);
	PORTE->PCR[25] |= PORT_PCR_PE(1);

	PORTE->PCR[24] |= PORT_PCR_PS(1);
	PORTE->PCR[25] |= PORT_PCR_PE(1);

	//Select MUX to be I2C
	PORTE->PCR[24] |= PORT_PCR_MUX(2);
	PORTE->PCR[25] |= PORT_PCR_MUX(2);
}

void vfnI2CMasterInit(void){
	//Enable I2C0 Clock
	SIM->SCGC4 |= SIM_SCGC4_I2C0(1);
	//Reset I2C Values
	I2C0->A1 = 0;
	I2C0->F = 0;
	I2C0->C1 = 0;
	I2C0->S = OxFF;
	I2C0->C2 = 0;
	I2C0->FLT = 0x50;
	I2C0->RA = 0;
	I2C0->C1 &= ~I2C_C1_IICEN(1);
	//Clear Flags
	vfnI2ClearFlags();
	//Mult 2
	I2C0->F |= I2C_F_MULT(1);
	//ICR 0x1F
	I2C0->F |= I2C_F_ICR(0x1F);
	//Enable SHEN and Write 1 to STOP to clear
	I2C0->FLT = 0xC0;
}

