# spitest.py
# A brief demonstration of the Raspberry Pi SPI interface, using the Sparkfun
# Pi Wedge breakout board and a SparkFun Serial 7 Segment display:
# https://www.sparkfun.com/products/11629

import time
import spidev

class ArduinoComms:
        
    # We only have SPI bus 0 available to us on the Pi
    bus = 0

    #Device is the chip select pin. Set to 0 or 1, depending on the connections
    device = 1
    
    spi = None

    def __init__(self):

        # Enable SPI
        self.spi = spidev.SpiDev()

        # Open a connection to a specific bus and device (chip select pin)
        self.spi.open(self.bus, self.device)

        # Set SPI speed and mode
        self.spi.max_speed_hz = 500000
    
        self.spi.mode = 0

    def run(self):
        while True:

            # The decimals, colon and apostrophe dots
            msg = [0x77]
            msg.append(i)
            result = spi.xfer2(msg)

            # The first character
            msg = [0x7b]
            msg.append(i)
            result = spi.xfer2(msg)

            # The second character
            msg = [0x7c]
            msg.append(i)
            result = spi.xfer2(msg)

            # The third character
            msg = [0x7d]
            msg.append(i)
            result = spi.xfer2(msg)

            # The last character
            msg = [0x7e]
            msg.append(i)
            result = spi.xfer2(msg)

            # Increment to next segment in each character
            i <<= 1

            # Clear display again
            msg = [0x76]
            self.spi.xfer2(msg)
            # Pause so we can see them
            time.sleep(5)

