import serial
import struct
import time
# Define struct format based on STM32's data structure
COMMS_MESSAGE_FORMAT = "<IBB16s"  # uint32 (4 bytes), MessageType (1 byte), Device (1 byte), payload (16 bytes)

def read_message_uart(ser):
    if ser.in_waiting == 0:
        return None
    print(struct.calcsize(COMMS_MESSAGE_FORMAT))
    raw_data = ser.read(struct.calcsize(COMMS_MESSAGE_FORMAT))

    if len(raw_data) != struct.calcsize(COMMS_MESSAGE_FORMAT):
        print("Error: Incomplete message received")
        return None

    print("Raw Data:", raw_data)  # Debugging

    sequence_number, msg_type, device, payload = struct.unpack(COMMS_MESSAGE_FORMAT, raw_data)
    print(f"Seq: {sequence_number}, Type: {msg_type}, Device: {device}, Raw Payload: {payload}")

    try:
        payload = payload.hex()
    except Exception as e:
        print(f"Decode error: {e}, Raw payload: {payload}")
        payload = repr(payload)  # Store as raw bytes for debugging


    message = {
        "sequence_number": sequence_number,
        "type": msg_type,
        "device": device,
        "payload": payload
    }

    return message

if __name__ == "__main__":
    ser = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=1)  # Adjust port and baudrate
    while True:
        time.sleep(0.1)
        # Example usage
        message = read_message_uart(ser)
        if message:
            print("Received:", message)