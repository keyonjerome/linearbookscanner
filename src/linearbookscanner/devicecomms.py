import logging
import time
import serial
import google.protobuf as nanopb
from queue import Queue, Empty
from comms_protocol_pb2 import CommsMessage, Command, CommandType

# Set up logging
logging.basicConfig(level=logging.INFO)
_logger = logging.getLogger()

HEARTBEAT_COMMAND = 55
STOP_COMMAND = 0x01
START_COMMAND = 0x02
REVERSE_COMMAND = 0x03
SHUTDOWN_COMMAND = 0x04

class DeviceComms:
    """Handles communication to the Arduino over UART"""

    def __init__(self, port: str = "/dev/serial0", baudrate: int = 115200):

        print(CommsMessage)
        self._port = port
        self._baudrate = baudrate
        self._running = False
        self._msg = []
        self._queue = Queue()

        # Initialize UART connection using pyserial
        self._uart = serial.Serial(self._port, self._baudrate, timeout=1)

    def create_comms_message(self, sequence_number: int, device: int, command_type: int, payload=None):
        """Create a protobuf CommsMessage with a given command type."""
        # Create Command message
        command = Command(command_type=command_type)

        # Create the CommsMessage
        comms_msg = CommsMessage(
            sequence_number=sequence_number,
            type=1,  # Assuming it's a COMMAND message
            device=device,
            command=command
        )
        return comms_msg

    def encode_and_send(self, message):
        """Encode the protobuf message and send over UART."""
        # Encode the message into binary using nanopb
        encoded_message = message.SerializeToString() 

        # Send the encoded message over UART
        self._uart.write(encoded_message)

    def decode_and_print(self, data):
        print()
        try:
            comms_message = CommsMessage()
            comms_message.ParseFromString(data)
            # comms_message = data.ParseFromString(CommsMessage, data)

            print(f"Received CommsMessage:")
            print(f"  Sequence Number: {comms_message.sequence_number}")
            print(f"  Device: {'RPI' if comms_message.device == 0 else 'STM32'}")
            print(f"  Type: {comms_message.type}")

            # Handle "oneof" field correctly
            if comms_message.HasField("command"):
                command = comms_message.command
                print(f"  Command Type: {command.command_type}")
            elif comms_message.HasField("response"):
                print(f"  Response Code: {comms_message.response.response_code}")
            elif comms_message.HasField("status"):
                print(f"  Device State: {comms_message.status.state}")

        except Exception as e:
            print(f"Error decoding message: {e}")


    def get_command_name(self, command_type):
        """Helper function to map command type to human-readable text."""
        if command_type == CommandType.FLIP_PAGE:
            return "FLIP_PAGE"
        elif command_type == CommandType.RESET:
            return "RESET"
        else:
            return "UNKNOWN"

    def shutdown(self):
        """Shutdown the UART communication."""
        self._running = False
        self._uart.close()

    def run(self):
        """Main thread for the STM32 communication.
        Sends heartbeat commands if none has been received."""
        self._running = True
        sequence_number = 1
        device = 0  # Assuming 0 corresponds to RPI

        while self._running:
            # time.sleep(1)
            try:
                cmd = self._queue.get_nowait()  # Try to get command from the queue
                self._msg = [cmd]
                if cmd == SHUTDOWN_COMMAND:
                    self._running = False
            except Empty:
                # Default to heartbeat command if no command received
                self._msg = [HEARTBEAT_COMMAND]

            # Create the CommsMessage and send it
            command_type = self._msg[0]  # The command type will be the first element in the list
            comms_message = self.create_comms_message(sequence_number, device, command_type)
            # print(f"Sending {comms_message}")
            self.encode_and_send(comms_message)
            sequence_number += 1

            # Optionally, you can read response after sending if needed:
            if self._uart.in_waiting > 0:
                response = self._uart.read(self._uart.in_waiting)
                print("Received:", response)
                self.decode_and_print(response)
                _logger.info(response)


if __name__ == "__main__":
    device = DeviceComms()
    try:
        device.run()
    except KeyboardInterrupt:
        device.shutdown()
