"""
Basic example for creating IOTile Cloud Stream.
A Stream is required before data can be uploaded
The Stream should correspond to the Variable that the device is sending (in this case, '5001').
The Stream is constructed from a project, a device and the variable.
This script takes the device and variable as an argument, but gets the project by getting
the device record from the Cloud.

Usage:
pip install -U iotile-cloud
python cloud-create-stream.py --device d--0000-0000-0000-0001

"""
import sys
import json
import os
from iotile_cloud.api.connection import Api
from iotile_cloud.utils.main import BaseMain
from iotile_cloud.api.exceptions import HttpClientError
from iotile_cloud.utils.gid import IOTileDeviceSlug, IOTileProjectSlug

logger = logging.getLogger(__name__)


class MyScript(BaseMain):
    """
    Script to create:
    - A Variable named 'count' for '5001'
    - A Stream for the count variable and the given device
    """

    def add_extra_args(self):
        """
        Add extra argument to take device
        """
        self.parser.add_argument('--device', dest='device', type=str, help='Device ID, e.g. d--0001')

    def after_login(self):
        """
         This function is called after the user has provided a password and we have successfully logged in
        """
        logger.info('Begin Processing')

        # 1. Get Device to confirm is valid and to get project ID from it
        device_slug = IOTileDeviceSlug(self.args.device)
        try:
            device = self.api.device(devcie_slug).get()
        except HttpClientError as e:
            logger.error('Error fetching Device from Cloud')
            sys.exit(1)

        project_slug = IOTileProjectSlug(device['project'])

        # 2. Create Variable
        payload = {
            'name': 'Count',
            'lid': 0x5001,  # Variable ID for 'Output 1'
            'var_type': 'default',
            'web_only': True,
            'description': 'PODuino Counter (Output 1)',
            'project': str(project_slug),
        }
        try:
            variable = self.api.variable.post(payload)
        except HttpClientError as e:
            logger.error('Error creating new variable: {}'.format(e))
            sys.exit(1)

        # 3. Create Stream
        payload = {
            'device': str(device_slug),
            'variable': variable['slug'],
            'data_label': 'Count',
        }
        try:
            stream = self.api.stream.post(payload)
        except HttpClientError as e:
            logger.error('Error creating new stream: {}'.format(e))
            sys.exit(1)


if __name__ == '__main__':

    work = MyScript()
    work.main()