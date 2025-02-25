/*
 * state_machine.c
 *
 *  Created on: Feb 23, 2025
 *      Author: keyon
 */

#include "state_machine.h"

DeviceState current_state;
DeviceState next_state;

CommsMessage last_msg;
uint32_t last_sequence_number;

/*
 * GPIOs:
 * - Status lights (x2)
 * - Manual page flip button
 * - Emergency stop
 * - Reset Button
 * - Limit switches (x2)
 */

/*
 * Interrupts:
 * - Limit switch interrupt (both sides):
 * 		- switch from running state to stopped state
 * - e-stop interrupt
 * 		- switch from running state to e-stopped state, require reset button press to transition out of e-stopped state
 *
 *
 */

void attemptStateTransition(DeviceState new_state) {
	// valid state transition regardless; no overriding is happening
	if(current_state == next_state) {
		next_state = new_state;
	}
	// overriding a state transition based on interrupt or otherwise; make sure it's valid (e.g: e-stop takes precedence over stopped)
	else {
		if(new_state > next_state) next_state = new_state;
	}
}

void processCommsMessage(comms_protocol_CommsMessage commsMsg) {
    // Check if the message is a duplicate
    if (commsMsg.sequence_number == last_sequence_number) {
        return;
    }

    // Process the message based on its type
    switch (commsMsg.type) {
        case comms_protocol_MessageType_UNKNOWN:
            // Throw out the message
//            printf("Received UNKNOWN message type. Discarding.\n");
            break;

        case comms_protocol_MessageType_COMMAND:
            // Ensure the union contains a command
            if (commsMsg.which_msg == comms_protocol_CommsMessage_command_tag) {
                // Access the command data
                comms_protocol_Command command = commsMsg.msg.command;

                // Handle the command based on its type
                switch (command.command_type) {
                    case comms_protocol_CommandType_FLIP_PAGE:
                        attemptStateTransition(comms_protocol_DeviceState_RUNNING); // Example state transition
//                        printf("Received FLIP_PAGE command.\n");
                        break;

                    case comms_protocol_CommandType_RESET:
                        attemptStateTransition(comms_protocol_DeviceState_INIT); // Example state transition
//                        printf("Received RESET command.\n");
                        break;

                    default:
//                        printf("Received unknown command type: %d\n", command.command_type);
                        break;
                }
            } else {
//                printf("Invalid union access: Expected command, but which_msg is %d\n", commsMsg.which_msg);
            }
            break;

        case comms_protocol_MessageType_RESPONSE:
            // Not currently used
//            printf("Received RESPONSE message. Not handling.\n");
            break;

        case comms_protocol_MessageType_STATUS:
            // Not currently used
//            printf("Received STATUS message. Not handling.\n");
            break;

        default:
//            printf("Received unknown message type: %d\n", commsMsg.type);
            break;
    }

    // Update the last sequence number
    last_sequence_number = commsMsg.sequence_number;
}

void initStateMachine() {
	current_state = DEVICE_INIT;
	next_state = DEVICE_STOPPED;

}
void runStateMachine() {

	switch(current_state) {
		case DEVICE_INIT:	// normal state, let the interrupts do the talking
			initStateMachine();
			break;

		case DEVICE_RUNNING:
			// normal state, let the interrupts do the talking
			processCommsMessage(last_msg);
			break;
		case DEVICE_STOPPED:
			// normal state, let the interrupts do the talking
			processCommsMessage(last_msg);
			break;
		case DEVICE_RESET:
			initStateMachine();
			break;
		case DEVICE_PAGE_FLIP:
			// activate page flip logic
			break;
		case DEVICE_EMERGENCY_STOPPED:
			// disable all outputs, and require that
			// the reset button be pressed before moving from this state
			break;
	}
	current_state = next_state;

}

void motorTask() {

}

