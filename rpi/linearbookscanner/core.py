# -*- coding: utf-8 -*-
from . import helpers
from . import scanner
from . import spi
import threading

def motor_thread():
    spi_runner = spi.ArduinoComms()
    spi_runner.run()

def scanner_thread():
    scanner_runner = scanner.Scanner()

def main():
    
    pass

if __name__ == "__main__":
    main()