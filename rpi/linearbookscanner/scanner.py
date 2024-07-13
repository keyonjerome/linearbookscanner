import sane 
import PIL
import os
import time

class Scanner:

    images = []
    image_path = "../../images/"
    image_number = 1
    def scan_and_save(self):
        # start the scan
        self.device.start()
        image = self.device.snap()
        timestr = time.strftime("%Y%m%d-%H%M%S")
        res = image.save(f"{self.image_path}/image_{timestr}.png")
        

    def __init__(self,devname: str):

        sane_info = sane.init()
        print("SANE INFO:", sane_info)

        # for non-production use; hardcode the device name later to the appropriate one
        devname  = sane.get_devices()[0][0]

        self.device = sane.open(devname)
        print("DEVICE PARAMETERS", self.device.get_parameters())
        print("SCANNING DEVICE INITIALIZED")

        os.mkdir(self.image_path)
