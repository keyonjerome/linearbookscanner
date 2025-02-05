import logging
from queue import Queue, Empty
import spidev
import RPi.GPIO as GPIO
import time

# Set up logging
logging.basicConfig(level=logging.INFO)
_logger = logging.getLogger()

HEARTBEAT_COMMAND = 55
STOP_COMMAND = 0x01
START_COMMAND = 0x02
REVERSE_COMMAND = 0x03
SHUTDOWN_COMMAND = 0x04


class ArduinoComms:
    """Handles communication to the Arduino over SPI"""

    _bus = 0
    _device = 0
    _spi = None

    def __init__(self, bus: int = 0, device: int = 0, speed: int = 500000, mode: int = 0):
        self._bus = bus
        self._device = device
        self._running = False
        self._msg = []
        self._queue = Queue()

        self._spi = spidev.SpiDev()
        self._spi.open(self._bus, self._device)
        self._spi.max_speed_hz = speed
        self._spi.mode = mode

    def spi_exchange(self, data):
        # Let the hardware SPI handle the CS pin automatically
        response = self._spi.xfer2(data)  # Send data
        return response

    def shutdown(self):
        """Asynchronously shutdown the SPI thread."""
        self._running = False
        self._spi.close()

    def run(self):
        """Main thread for the Arduino communication.
        Sends heartbeat commands if none has been received."""
        self._running = True
        while self._running:
            time.sleep(1)
            try:
                cmd = self._queue.get_nowait()
                self._msg = [cmd]
                if cmd == SHUTDOWN_COMMAND:
                    self._running = False
            except Empty:
                self._msg = [HEARTBEAT_COMMAND]
            print("Sending", self._msg)
            result = self.spi_exchange(self._msg)
            print("Received", result)
            _logger.info(result)


if __name__ == "__main__":
    device = ArduinoComms()
    try:
        device.run()
    except KeyboardInterrupt:
        device.shutdown()
