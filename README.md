# PODuino v1.0

The **Poduino v1.0** is a development board that combines the *Arduino Mega 2560*
with and *IOTile Device*.

The **Arduino Mega 2560** in the **PODuino** is just another Arduno, and can be used on its own like
any other Arduino. Most every **Arduino Mega 2560** example/tutorial should work with the **PODuino**.

## PODuino differences

The following pins are used to connect the **Arduino Mega 2560** with the rest of the **IOTile Device**:

| **Pin** | **IOTile GPIO Description** |
|---	|---	|
| 14 | TX3 |
| 15 | RX3 |
| 39 | Digital |

## Installation

In order for you to use the PODuino with the Arduino IDE, you must install the *IOTileBridge library*.

You will also need to install the python IOTile tools to be able to configure the PODuino, controll it via BLE,
or debug it.

### IOTile Core Tools

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
pip install --upgrade iotile-core iotile-transport-bled112 iotile-support-firm-arduino-bridge-0 iotile-support-con-nrf52832-2 iotile-support-lib-controller-3 --extra-index-url https://pypi.fury.io/sKv8PCZngFJ-g_oEqjfc/iotile/
```

See http://coretools.readthedocs.io/en/latest/ for complete documentation.

### Arduino IDE Setup

If you have never used an Arduino, it is recommended you read the [Arduino Mega 2560 Getting Started](https://www.arduino.cc/en/Guide/ArduinoMega2560). 
and the many available tutorials. Just make sure you select **Arduino Mega 2560** when selecting which Arduino you have. 
The **Arduino Mega 2560** in the **PODuino** is just a regular Arduino, and can be used on its own like any other Arduino.



Otherwise, make sure you have the [latest Arduino IDE](https://www.arduino.cc/en/Main/Software)

You need to install the IOTileBridge library (you can find the latest version as a GitHub release).

If you're using the PODuino, go to the Arduino IDE, choose Sketch -> Include Library -> Add .zip Library and select the IOTileBridgeMega.zip
folder downloaded from [here](https://github.com/iotile/poduino/tree/master/downloads)

