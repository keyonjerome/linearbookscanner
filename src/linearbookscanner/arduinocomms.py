"""
spitest.py
A brief demonstration of the Raspberry Pi SPI interface, using the Sparkfun
Pi Wedge breakout board and a SparkFun Serial 7 Segment display:
https://www.sparkfun.com/products/11629
"""

import logging
from queue import Queue, Empty

# import time
import spidev
import RPi.GPIO as GPIO
import time

_logger = logging.getLogger()


CS_PIN = 8 #GPIO8 (SPI CS0)
HEARTBEAT_COMMAND = 55
STOP_COMMAND = 0x01
START_COMMAND = 0x02
REVERSE_COMMAND = 0x03
SHUTDOWN_COMMAND = 0x04



class ArduinoComms:
    """Handles communication to the Arduino over SPI"""

    # We only have SPI bus 0 available to us on the Pi
    _bus = 0

    # Device is the chip select pin. Set to 0 or 1, depending on the connections
    _device = 0

    _spi = None

    def __init__(
        self, bus: int = 0, device: int = 0, speed: int = 500000, mode: int = 0
    ):
        self._bus = bus
        self._device = device
        self._running = False
        self._msg = []
        self._queue = Queue()

        GPIO.setmode(GPIO.BCM)
        GPIO.setup(CS_PIN,GPIO.OUT)
        GPIO.output(CS_PIN,GPIO.HIGH)

        # Enable SPI
        self._spi = spidev.SpiDev()

        # Open a connection to a specific bus and device (chip select pin)
        self._spi.open(self._bus, self._device)

        # Set SPI speed and mode
        self._spi.max_speed_hz = speed

        self._spi.mode = mode
    
    def spi_exchange(self,data):
        GPIO.output(CS_PIN, GPIO.LOW)  # Select slave
        time.sleep(0.001)  # Small delay for stability
        response = self._spi.xfer2(data)  # Send data
        time.sleep(0.001)  # Small delay
        GPIO.output(CS_PIN, GPIO.HIGH)  # Deselect slave
        return response
    def shutdown(self):
        """Asynchronously shutdown the SPI thread."""
        self._running = False

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
            result = self.spi_exchange(self._msg)
            print(result)
            _logger.info(result)


if __name__ == "__main__":
    device = ArduinoComms()
    device.run()
