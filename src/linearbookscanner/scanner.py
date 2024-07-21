"""Wrappers for interacting with SANE printing devices"""

import os
import time
import sane

# import PIL


class ScannerCommand:
    """Passes command information to Scanner"""

    def __init__(self, cmd: str, data: dict):
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

    def scan_and_save(self):
        """Starts a scan and saves the new image to the filesystem"""
        # start the scan
        self.device.start()
        image = self.device.snap()
        timestr = time.strftime("%Y%m%d-%H%M%S")
        image.save(f"{self.image_path}/image_{timestr}.png")

    def get_images(self):
        """Get all images saved so far"""
        return self.images

    def __init__(self):
        devname = ""
        sane_info = sane.init()
        print("SANE INFO:", sane_info)

        if devname == "":
            # for non-production use; hardcode the device name later to the appropriate one
            devname = sane.get_devices()[0][0]

        self.device = sane.open(devname)
        print("DEVICE PARAMETERS", self.device.get_parameters())
        print("SCANNING DEVICE INITIALIZED")

        os.mkdir(self.image_path)
