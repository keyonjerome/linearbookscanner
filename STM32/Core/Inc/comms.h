/*
 * comms.h
 *
 *  Created on: Feb 21, 2025
 *      Author: keyon
 */

#ifndef INC_COMMS_H_
#define INC_COMMS_H_

#include "stm32f4xx_hal.h"
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

extern UART_HandleTypeDef huart1;
bool encode_command(CommsMessage *message);
bool decode_command(CommsMessage *message);


#endif /* INC_COMMS_H_ */
