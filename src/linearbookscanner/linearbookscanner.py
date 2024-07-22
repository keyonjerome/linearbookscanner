"""Linear Book Scanner main program"""

import threading
import logging

# from . import helpers
from . import scanner
from . import arduinocomms

_logger = logging.getLogger(__name__)


def motor_thread() -> None:
    """Run the motor communication thread"""
    spi_runner = arduinocomms.ArduinoComms()
    spi_runner.run()


def scanner_thread() -> None:
    """Run the scanner thread"""
    scanner_runner = scanner.Scanner()
    scanner_runner.run()


def main() -> None:
    """Main for the Linear Book Scanner"""
    motor_runner = threading.Thread(target=motor_thread)
    motor_runner.start()

    scan_runner = threading.Thread(target=scanner_thread)
    scan_runner.start()

    motor_runner.join()
    scan_runner.join()
    _logger.info("Ending program.")


if __name__ == "__main__":
    main()
