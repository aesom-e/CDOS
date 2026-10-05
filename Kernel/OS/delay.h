#ifndef OS_DELAY_H
#define OS_DELAY_H

#include "types.h"

/**
* \brief Initializes the delay subsystem
*/
void delay_Init(void);

/**
* \brief Delays execution for the specified amount of time
* \param ms The number of milliseconds to delay for
*/
void delay_Milliseconds(dWord ms);

#endif