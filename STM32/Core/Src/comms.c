/*
 * comms.c
 *
 *  Created on: Feb 21, 2025
 *      Author: keyon
 */

#include "comms.h"

// Define buffer sizes and UART settings
#define UART_BUFFER_SIZE 128
#define UART_TIMEOUT 100  // 100ms timeout

// Function to encode and send a CommsMessage
bool encode_command(CommsMessage *message) {
    pb_ostream_t stream = pb_ostream_from_buffer(uart_tx_buffer + 4, UART_BUFFER_SIZE - 4);

    if (!pb_encode(&stream, comms_protocol_CommsMessage_fields, message)) {
        return false;  // Encoding failed
    }

    size_t message_length = stream.bytes_written;

    // Prepend the 4-byte length header (little-endian format)
    uart_tx_buffer[0] = message_length & 0xFF;
    uart_tx_buffer[1] = (message_length >> 8) & 0xFF;
    uart_tx_buffer[2] = (message_length >> 16) & 0xFF;
    uart_tx_buffer[3] = (message_length >> 24) & 0xFF;

    // Transmit via UART
    if (HAL_UART_Transmit(&huart1, uart_tx_buffer, message_length + 4, HAL_MAX_DELAY) != HAL_OK) {
        return false;  // UART transmission failed
    }

    return true;
}

// Function to receive and decode a CommsMessage
bool decode_command(CommsMessage *message) {
    memset(uart_rx_buffer, 0, UART_BUFFER_SIZE);

    // Receive data via UART
    if (HAL_UART_Receive(&huart1, uart_rx_buffer, UART_BUFFER_SIZE, HAL_MAX_DELAY) != HAL_OK) {
        return false;  // UART receive failed
    }

    pb_istream_t stream = pb_istream_from_buffer(uart_rx_buffer, UART_BUFFER_SIZE);

    if (!pb_decode(&stream, comms_protocol_CommsMessage_fields, message)) {
        return false;  // Decoding failed
    }

    return true;
}

// Function to perform concurrent transmit and receive
bool uart_transmit_receive(CommsMessage *tx_message, CommsMessage *rx_message) {
    pb_ostream_t ostream = pb_ostream_from_buffer(uart_tx_buffer, UART_BUFFER_SIZE);

    if (!pb_encode(&ostream, comms_protocol_CommsMessage_fields, tx_message)) {
        return false;  // Encoding failed
    }

    size_t tx_length = ostream.bytes_written;
    memset(uart_rx_buffer, 0, UART_BUFFER_SIZE);

    // Perform UART transmit and receive
    if (HAL_UART_TransmitReceive(&huart1, uart_tx_buffer, uart_rx_buffer, tx_length, HAL_MAX_DELAY) != HAL_OK) {
        return false;  // UART transmission/reception failed
    }

    pb_istream_t istream = pb_istream_from_buffer(uart_rx_buffer, UART_BUFFER_SIZE);

    if (!pb_decode(&istream, comms_protocol_CommsMessage_fields, rx_message)) {
        return false;  // Decoding failed
    }

    return true;
}


void runCommsTask(uint32_t *seq) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // Toggle LED
    CommsMessage message = {0};
    message.sequence_number = seq;
	message.type = comms_protocol_MessageType_STATUS;
	message.device = comms_protocol_Device_STM32;
	message.which_msg = 2;  // Corresponding to status message

	// Set the status message fields
	message.msg.status.state = comms_protocol_DeviceState_RUNNING;
	message.msg.status.ticks = HAL_GetTick();  // Example tick value
//	tx_message.which_msg = 2;
//	tx_message.msg = 1;
	encode_command(&message);
//	//HAL_UART_Transmit(&huart2, msg, sizeof(msg) - 1, HAL_MAX_DELAY);
//	//HAL_UART_Transmit(&huart6, msg, sizeof(msg) - 1, HAL_MAX_DELAY);
//
	HAL_Delay(500);
}
