#include "CycleCounter.h"
//#include "core_cm7.h"
void ResetTimer(){
	DWT_CYCCNT   = (volatile unsigned int *)0xE0001004; //address of the register
	DWT_CONTROL  = (volatile unsigned int *)0xE0001000; //address of the register
	SCB_DEMCR    = (volatile unsigned int *)0xE000EDFC; //address of the register
	DWT_LAR      = (volatile unsigned int *)0xE0001FB0;

	*DWT_LAR = 0xC5ACCE55; // unlock (CM7)
	*SCB_DEMCR   = *SCB_DEMCR | 0x01000000;
	*DWT_CYCCNT  = 0; // reset the counter
	*DWT_CONTROL = 0; 
}

//void ResetTimer2(){
//	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
//	DWT->CYCCNT = 0;
//	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
//}

void StartTimer(){
	*DWT_CONTROL = *DWT_CONTROL | (unsigned int)1 ; // enable the counter
//	DWT->CTRL = DWT_CTRL_CYCCNTENA_Msk;
}

void StopTimer(){
	*DWT_CONTROL = *DWT_CONTROL & (unsigned int)0 ; // disable the counter
//	DWT->CTRL = DWT_CTRL_CYCCNTENA_Pos;
}

unsigned int getCycles(){
	return *DWT_CYCCNT;
}
