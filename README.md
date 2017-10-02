# PODuino v1.0

The **Poduino v1.0** is a development board that combines the *Arduino Mega 2560*
with an *IOTile Device* for data collection and control over Bluetooth.

The **Arduino Mega 2560** within the **PODuino** can be used on its own just like any other Arduino.
Therefore, any **Arduino Mega 2560** example/tutorial should work with the **PODuino** as well allowing you 
to combine Arduino projects with low power wireless connectivity and the cloud.

![PODuino IOTile Device](/images/poduino.jpg)

## PODuino differences

The following pins are used to connect the **Arduino Mega 2560** with the rest of the **IOTile Device**:

| **Pin** | **IOTile GPIO Description** |
|---	|---	|
| 14 | TX3 |
| 15 | RX3 |
| 39 | Digital |

Make sure you have a jumper installed on your PODuino as shown:

<img src="/images/jumper.jpg" alt="PODuino Jumper Setup" width="250">

## Setup

You will need access to Python 2.7+ and the following tools. Install them now as needed:

1. Install the [latest Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Install the [IOTile Coretools](/docs/installation/iotile-coretools.md) 
3. Install the [IOTile Arduino Bridge](/docs/installation/iotile-arduino-bridge.md)

## Hello World!

#### Start with the [PODuino Blink Tutorial](/examples/blink-tutorial/README.md)

## Additional Tutorials and Examples

A few tutorials and examples are included under the `/examples` directory.

| **Example** | **Description** |
|---	|---	|
| [blink-tutorial](/examples/blink-tutorial/README.md) | Start with this simple example |
| [count-example](/examples/count-example/README.md) | A more elaborated but still simple count up/down example |
| [rfid-example](/examples/rfid-example/README.md) | Example of an RFID reader (incomplete) |
