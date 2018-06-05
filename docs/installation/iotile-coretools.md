# IOTile Core Tools

Make sure you have Python 2.7.9+ installed. Like with any other Python package, it is highly recommended
that you use `virtualenv` to install these packages on:

```
pip install virtualenv
virtualenv --python=python2.7 iotile

#On Mac/Linux
source iotile/bin/activate

#On Windows Powershell
iotile/Scripts/activate.ps1

#On Windows CMD
iotile\Scripts\activate.bat
```

Then install the required packages by typing:

```
pip install --upgrade iotile-core iotile-transport-bled112 iotile-support-firm-arduino-bridge-0 iotile-support-con-nrf52832-2 iotile-support-lib-controller-3
```

To use the IOTile Sensor Graph, you will also need

```
pip install --upgrade iotile-sensorgraph
```

See http://coretools.readthedocs.io/en/latest/ for complete documentation.

To also use the stand-alone **IOTile Cloud API** with python, do

```
pip install --upgrade iotile-cloud
```