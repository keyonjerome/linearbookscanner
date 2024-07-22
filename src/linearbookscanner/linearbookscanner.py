"""Linear Book Scanner main program"""

import threading
import logging

# from . import helpers
from . import scanner
from . import spi


def motor_thread() -> None:
    """Run the motor communication thread"""
    spi_runner = spi.ArduinoComms()
    spi_runner.run()


def scanner_thread() -> None:
    """Run the scanner thread"""
    scanner_runner = scanner.Scanner()
    scanner.run()


def main() -> None:
    """Main for the Linear Book Scanner"""
    motor = threading.Thread(target=motor_thread)
    motor.start()

    scanner = threading.Thread(target=scanner_thread)
    scanner.start()

    motor.join()
    scanner.join()


if __name__ == "__main__":
    main()
