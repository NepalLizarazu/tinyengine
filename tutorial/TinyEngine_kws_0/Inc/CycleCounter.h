#ifndef CYCLECOUNTER
#define CYCLECOUNTER

static volatile unsigned int *DWT_CYCCNT  ;
static volatile unsigned int *DWT_CONTROL ;
static volatile unsigned int *SCB_DEMCR   ;
static volatile unsigned int *DWT_LAR	  ;
// Resets the internal cycle counter to zero
void ResetTimer(void);

//void ResetTimer2(void);

// Starts the internal cycle counter
void StartTimer(void);

// Stops the internal cycle counter
void StopTimer(void);

// Returns the current number of cycles according to the internal cycle counter
unsigned int getCycles(void);

#endif 
