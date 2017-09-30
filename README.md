# PODuino v1.0

The **Poduino v1.0** is a development board that combines the *Arduino Mega 2560*
with and *IOTile Device*.

The **Arduino Mega 2560** in the **PODuino** is just another Arduno, and can be used on its own like
any other Arduino. Most every **Arduino Mega 2560** example/tutorial should work with the **PODuino**.

![PODuino IOTile Device](/images/poduino.jpg)

## PODuino differences

The following pins are used to connect the **Arduino Mega 2560** with the rest of the **IOTile Device**:

| **Pin** | **IOTile GPIO Description** |
|---	|---	|
| 14 | TX3 |
| 15 | RX3 |
| 39 | Digital |

Make sure you have a jumper installed on your PODuino as follows

<img src="/images/jumper.jpg" alt="PODuino Jumper Setup" width="400">

## Installation

In order for you to use the PODuino with the Arduino IDE, you must install the *IOTileBridge library*.

You will also need to install the python IOTile tools to be able to configure the PODuino, controll it via BLE, or debug it.

Read the installation instructions for the [IOTile Coretools](/docs/installation/iotile-coretools.md) and [IOTile Arduino Bridge](/docs/installation/iotile-arduino-bridge.md)

## Hello World!

#### Start with the [PODuino Blink Tutorial](/examples/blink-tutorial/README.md)

## Additional Tutorials and Examples

A few tutorials and examples are included under the `/examples` directory.

| **Example** | **Description** |
|---	|---	|
| [blink-tutorial](/examples/blink-tutorial/README.md) | Start with this simple example |
| [count-example](/examples/count-example/README.md) | A more elaborated but still simple count up/down example |
| [rfid-example](/examples/rfid-example/README.md) | Example of an RFID reader (incomplete) |
