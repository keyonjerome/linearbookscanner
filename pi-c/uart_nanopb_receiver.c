#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "comms_protocol.pb.h"  // Include the nanopb-generated header

#define UART_DEVICE "/dev/ttyS0"  // Change this to the correct UART device
#define UART_BUFFER_SIZE 256

// Global variable for UART buffer
uint8_t uart_rx_buffer[UART_BUFFER_SIZE];

// Function to open and configure the UART port
int uart_open(const char *device) {
    int uart_fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Unable to open UART");
        return -1;
    }

    struct termios options;
    tcgetattr(uart_fd, &options);

    // Set the baud rate
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    // 8N1 (8 data bits, no parity, 1 stop bit)
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Disable flow control
    options.c_cflag &= ~CRTSCTS;

    // Raw input/output
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tcsetattr(uart_fd, TCSANOW, &options);
    return uart_fd;
}

// Function to receive data from UART
int uart_receive(int uart_fd, uint8_t *buffer, size_t size) {
    int bytes_read = read(uart_fd, buffer, size);
    if (bytes_read < 0) {
        perror("Failed to read from UART");
        return -1;
    }
    return bytes_read;
}

// Function to decode a received message
void decode_command(uint8_t *data, size_t len) {
    comms_protocol_CommsMessage msg = comms_protocol_CommsMessage_init_zero;

    pb_istream_t stream = pb_istream_from_buffer(data, len);

    if (pb_decode(&stream, comms_protocol_CommsMessage_fields, &msg)) {
        // Successfully decoded the message
        printf("Decoded message:\n");
        printf("  Sequence Number: %d\n", msg.sequence_number);
        printf("  Type: %d\n", msg.type);
        printf("  Device: %d\n", msg.device);

        if (msg.type == comms_protocol_MessageType_COMMAND) {
            printf("  Command Type: %d\n", msg.msg.command.command_type);
        } else if (msg.type == comms_protocol_MessageType_RESPONSE) {
            printf("  Response Code: %d\n", msg.msg.response.response_code);
        } else if (msg.type == comms_protocol_MessageType_STATUS) {
            printf("  Device State: %d\n", msg.msg.status.state);
            printf("  Ticks: %d\n", msg.msg.status.ticks);
        }
    } else {
        // Failed to decode the message
        printf("Error decoding message\n");
    }
}

// UART receive loop to continuously listen for incoming data
void uart_receive_loop(int uart_fd) {
    while (1) {
        int received_len = uart_receive(uart_fd, uart_rx_buffer, UART_BUFFER_SIZE);
        if (received_len > 0) {
            // Decode the received message
            decode_command(uart_rx_buffer, received_len);
        } else {
           // printf("No data received\n");
        }
    }
}

int main() {
    // Open the UART device
    int uart_fd = uart_open(UART_DEVICE);
    if (uart_fd == -1) {
        return 1;  // Unable to open UART
    }

    // Start the UART receive loop
    uart_receive_loop(uart_fd);

    // Close the UART device
    close(uart_fd);
    return 0;
}
