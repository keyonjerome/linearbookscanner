/*
 * state_machine.h
 *
 *  Created on: Feb 23, 2025
 *      Author: keyon
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "comms.h"

typedef enum DeviceState
{
    DEVICE_INIT = 1,
    DEVICE_RUNNING = 2,
	DEVICE_PAGE_FLIP = 3,
	DEVICE_STOPPED = 4,
	DEVICE_EMERGENCY_STOPPED = 5,
	DEVICE_RESET = 6
} DeviceState;

void processCommsMessage(CommsMessage msg);
void initStateMachine();
void runStateMachine();

#endif /* INC_STATE_MACHINE_H_ */
