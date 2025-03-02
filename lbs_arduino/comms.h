/*
 * comms.h
 *
 *  Created on: Feb 21, 2025
 *      Author: keyon
 */

#ifndef INC_COMMS_H_
#define INC_COMMS_H_

#include "comms_protocol.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

// Typedefs for ease of use
typedef comms_protocol_CommsMessage CommsMessage;
typedef comms_protocol_Command Command;
typedef comms_protocol_Response Response;
typedef comms_protocol_Status Status;

#define UART_BUFFER_SIZE 128  // Adjust based on message size

static uint8_t uart_tx_buffer[UART_BUFFER_SIZE];  // Transmit buffer
static uint8_t uart_rx_buffer[UART_BUFFER_SIZE];  // Receive buffer

bool encode_command(CommsMessage *message);
bool decode_command(CommsMessage *message);


// Function to create a flip page message
void create_flip_page_message(uint32_t seq, comms_protocol_CommsMessage *msg);

// Function to create a status message
void create_status_message(uint32_t seq, comms_protocol_DeviceState state, uint32_t ticks, comms_protocol_CommsMessage *msg);

// Function to create a reset command message
void create_reset_command_message(uint32_t seq, comms_protocol_CommsMessage *msg);

void runCommsTask(uint32_t *seq);


#endif /* INC_COMMS_H_ */
