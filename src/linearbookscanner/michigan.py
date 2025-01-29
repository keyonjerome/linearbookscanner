#! /usr/bin/env python3

"""
Linear Book Scanner Control Code 
"""

import shelve
import spidev
import RPi.GPIO as GPIO
import time

# GPIO Pin Configuration
GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.OUT)  # Micro Reset Pin
GPIO.setup(17, GPIO.OUT)  # Micro Enable Pin
GPIO.output(18, GPIO.HIGH)
GPIO.output(17, GPIO.LOW)

# SPI Configuration
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 500000

class LBS_Control:
    FILENAME = "LBS_Settings.db"

    def __init__(self):
        self.ENABLE_PIN = 0
        self.MOVING = 0
        self.CURRENT_COMMAND = "Idle"
        self.variable_1 = 1600  # Steps per inch
        self.variable_2 = 10    # Distance to end of scan (in inches)
        self.variable_3 = 20    # Scan velocity (%)
        self.variable_4 = 20    # Distance to end stop (in inches)
        self.variable_5 = 60    # Post-scan velocity (%)
        self.variable_6 = 60    # Reverse velocity (%)

        self.micro_reset()
        print("System Initialized")

    def micro_reset(self):
        GPIO.output(18, GPIO.LOW)
        time.sleep(0.01)  # Sleep 10ms
        GPIO.output(18, GPIO.HIGH)

    def enable_pin(self, enable):
        GPIO.output(17, GPIO.HIGH if enable else GPIO.LOW)
        self.ENABLE_PIN = 1 if enable else 0

    def send_position(self, position, velocity):
        self.enable_pin(0)
        temp1 = int(position) >> 8
        temp2 = int(position) & 0xFF

        print("Sending Position:")
        print(spi.xfer([240]))
        print(spi.xfer([temp1]))
        print(spi.xfer([temp2]))
        print(spi.xfer([velocity]))

        self.enable_pin(1)
        self.MOVING = 1

    def set_position(self, position):
        print("Setting Current Position")
        self.enable_pin(0)
        temp1 = int(position) >> 8
        temp2 = int(position) & 0xFF

        print(spi.xfer([241]))
        print(spi.xfer([temp1]))
        print(spi.xfer([temp2]))
        print(spi.xfer([0]))  # Placeholder byte

        self.enable_pin(1)

    def update_current_position(self):
        if self.ENABLE_PIN == 0:
            return

        temp1 = spi.xfer([0])
        temp2 = spi.xfer([0])[0]
        temp3 = spi.xfer([0])[0]

        negative = 1
        if temp2 & 0b10000000:
            temp2 = (~temp2 & 0xFF)
            temp3 = (~temp3 & 0xFF) + 1
            negative = -1

        self.MOVING = temp1[0]
        current_pos = (temp2 * 256 + temp3) * negative
        print(f"Current Position: {current_pos}, MOVING: {self.MOVING}")

    def scan_cycle(self):
        # Move through scan
        print("Starting Scan Cycle")
        self.send_position(-32000 + int(self.variable_1 * self.variable_2), int(self.variable_3))
        time.sleep(1)
        
        # Move to end stop
        # self.send_position(-32000 + int(self.variable_1 * self.variable_4), int(self.variable_5))
        self.send_position(100,50)
        time.sleep(1)

        # Return to home
        self.send_position(-32000, int(self.variable_6))
        print("Scan Cycle Complete")
    
    def scan_test_cycle(self):
        self.send_position(0,200)
        time.sleep(1)
        self.send_position(250,100)

    def cleanup(self):
        GPIO.cleanup()
        print("GPIO Cleaned Up")

if __name__ == "__main__":
    try:
        lbs = LBS_Control()
        while True:
            lbs.scan_test_cycle()
    except KeyboardInterrupt:
        print("Process Interrupted")
    finally:
        lbs.cleanup()
