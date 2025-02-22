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
    pb_ostream_t stream = pb_ostream_from_buffer(uart_tx_buffer, UART_BUFFER_SIZE);

    if (!pb_encode(&stream, comms_protocol_CommsMessage_fields, message)) {
        return false;  // Encoding failed
    }

    size_t message_length = stream.bytes_written;

    // Transmit via UART
    if (HAL_UART_Transmit(&huart1, uart_tx_buffer, message_length, HAL_MAX_DELAY) != HAL_OK) {
    		uint8_t msg[] = "Broken transmit!\r\n";
//    	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // Toggle LED
    		HAL_UART_Transmit(&huart1, msg, sizeof(msg) - 1, HAL_MAX_DELAY);
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
