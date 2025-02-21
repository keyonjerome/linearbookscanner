/*
 * comms.c
 *
 *  Created on: Feb 21, 2025
 *      Author: keyon
 */


bool decode_command(CommsMessage *message) {
    memset(uart_rx_buffer, 0, UART_BUFFER_SIZE);

    // Receive data via UART
    if (HAL_UART_Receive(&huart2, uart_rx_buffer, UART_BUFFER_SIZE, HAL_MAX_DELAY) != HAL_OK) {
        return false;  // UART receive failed
    }

    pb_istream_t stream = pb_istream_from_buffer(uart_rx_buffer, UART_BUFFER_SIZE);

    if (!pb_decode(&stream, CommsMessage_fields, message)) {
        return false;  // Decoding failed
    }

    return true;
}
