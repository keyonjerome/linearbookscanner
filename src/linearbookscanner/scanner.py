"""Wrappers for interacting with SANE printing devices"""

import os
import time
import logging
from queue import Queue

import sane

# import PIL

_logger = logging.getLogger(__name__)


class ScannerCommand:
    """Passes command information to Scanner"""

    # COMMAND_SCAN = 1
    COMMAND_SCAN_AND_SAVE: str = "SCAN_AND_SAVE"
    COMMAND_SHUTDOWN: str = "SHUTDOWN"

    def __init__(self, cmd: str, data: dict = None):
        self.cmd = cmd
        self.data = data

    def get_data(self):
        """Returns command data"""
        return self.data

    def get_cmd(self):
        """Returns command"""
        return self.cmd


class Scanner:
    """Wraps the SANE scanning device for scanning and saving"""

    images = []
    image_path = "../../images/"
    image_number = 1

    def _scan_and_save(self):
        """Starts a scan and saves the new image to the filesystem"""
        _logger.info("Scanning image.")
        # start the scan
        self._device.start()
        image = self._device.snap()
        timestr = time.strftime("%Y%m%d-%H%M%S")
        _logger.info("Saving image.")
        image.save(f"{self.image_path}/image_{timestr}.png")
        self.images.append(image)

    def _process_command(self, cmd: ScannerCommand) -> bool:
        """Process an incoming command."""
        match cmd.cmd:
            case ScannerCommand.COMMAND_SCAN_AND_SAVE:
                self._scan_and_save()
                return True
            case ScannerCommand.COMMAND_SHUTDOWN:
                self._device.cancel()
                self._device.close()
                return True
            case _:
                return False

    def put_command(self,cmd: ScannerCommand):
        """Add a command to the command queue"""
        self._queue.put(cmd)

    def shutdown(self):
        """Properly shutdown the scanner."""
        # Stop the while loop
        self._running = False
        # Unblock the queue
        self.put_command(ScannerCommand(ScannerCommand.COMMAND_SHUTDOWN))

    def run(self):
        """Run the command queue. This function is blocking, and should only be called once."""
        self._running = True
        while self._running:
            command = self._queue.get()  # blocks until the next command is received
            ret = self._process_command(command)
            if not ret:
                _logger.error("Command %s failed.",command.cmd)

    def __init__(self, devname: str = ""):

        self._devname = devname
        self._queue = Queue()
        self._sane_info = sane.init()
        self._running = False

        _logger.info("SANE INFO: %s",self._sane_info)

        if self._devname == "":
            # for non-production use; hardcode the device name later to the appropriate one
            self._devname = sane.get_devices()[0][0]

        self._device = sane.open(devname)
        _logger.info("DEVICE PARAMETERS: \r\n %s",self._device.get_parameters())
        _logger.info("SCANNING DEVICE INITIALIZED")

        os.mkdir(self.image_path)
