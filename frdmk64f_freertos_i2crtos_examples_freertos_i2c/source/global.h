/*
 * global.h
 *
 *  Created on: Mar 31, 2018
 *      Author: Cursos
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "stdint.h"
void delay(uint16_t delay)
{
	volatile int counter, counter2;

	for(counter2=16; counter2 > 0; counter2--)
	{
	for(counter=delay; counter > 0; counter--);

	}
}

#endif /* GLOBAL_H_ */
