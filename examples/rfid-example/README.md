# Up/Down Counter

Simple example shows how to use the PODuino and the OSEPP LCD display to allow users to count up or down,
recording each time the count changes and uploading that to the IOTile Cloud

## Need

1. PODuino v1.0
1. BLE112 Dongle
1. OSEPP *16×2 LCD Display & Keypad Shield*: https://www.osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield

# Introduction

### Arduino IDE Setup

If you have never used an Arduino, read the following [Arduino Mega 2650 Getting Started](https://www.arduino.cc/en/Guide/ArduinoMega2560)

If you're using the PODuino, go to the Arduino IDE, choose Sketch -> Include Library -> Add .zip Library and select the IOTileBridgeMega.zip
folder downloaded from [here](https://github.com/iotile/poduino/tree/master/downloads)

### IOTile Tool Setup

See the main readme to learn how to install the required Arduino IDE and IOTile tools:

```
virtualenv poduino
source poduino/bin/activate

pip install --upgrade iotile-core iotile-transport-bled112 iotile-support-firm-arduino-bridge-0 iotile-support-con-nrf52832-2 iotile-support-lib-controller-3 --extra-index-url https://pypi.fury.io/sKv8PCZngFJ-g_oEqjfc/iotile/
```

### Sketch

Using the Arduino IDE, open the `firmware.ino` file included in this example.

This sketch will listen at the Up and Down buttons in the LCD display, and when pressed,
it will increment or decrement an in-memory counter. This counter is restricted within 0 and 100.

The sketch will also send the counter value every time it is modified to `input 10` in the IOTile Controller.
Later we will describe the required IOTile Configuration to have this input copied to a buffered output that
will be sent to the cloud every time the IOTile Mobile app or IOTile Gateway connects to the PODuino.

Finally, the sketch also listens to `Event 0` from the IOTile Controller, and will reset the counter when
called.

### Programming the PODuino

The PODuino requires two seperate steps to configure:

1. The Arduino Mega needs to be programmed via the USB Cable and a computer the same way any Arduino compatible
board is programmed. There is a number of tutorials and blogs explaining how to program an Arduino. Just make sure
you select the *Arduino Mega 2560* when

1. The rest of the IOTile device is already pre-programmed with the IOTile firmware, but requires a Sensor Graph 
to be configured the device is to be used as a logger. We will explain how to configure the Sensor Graph later on.

# Tutorial

## 1. Connect the PODuino with a USB Cable to a computer

The computer should have both the Arduino IDE and IOTile Tools installed.

## 2. Upload the Sketch to the Arduino

Follow instructions in [Arduino Mega 2560 Getting Started](https://www.arduino.cc/en/Guide/ArduinoMega2560).

At this point, the Arduino portion is configured and you can disconnect the USB cable if you wish. 
The USB cable is only used for programming the Arduino portion, but can also be used as power source.

You should see the LCD Display show *Poduino Counter*, showing *0* as value of counter. If you press
the Up or Down buttons, you should see this value change.

## 3. Introduction to the IOTile Coretools

Probably one of the key advatages of the PODuino over other Arduino boards is the fact that you
can use the IOTile Device and IOTile Tools to control or read information from the Arduino via
BLE.

IOTile Tools currently only support communication to the IOTile Controller in the PODuino via
the [BLE112 Dongle](https://www.digikey.com/catalog/en/partgroup/bled112-bluetooth-smart-dongle/40600)
which either came with your PODuino or you can buy from Arch Systems or other sources.

Once you connect the BLE112 dongle to your computer, you can imediately use it to communicate with
the PODuino in general, and the Arduino Mega in particular. Make sure you are within 50 feet of your PODuino as you scan for it.

See [IOTile Coretools Documentation](http://coretools.readthedocs.io/en/latest/) to learn the full
power of the IOTile Platform.

To understand the basic functionality needed to control the PODuino, you need to understand the
following key concepts:

- All IOTile Devices are represented with a globally unique ID similar to `d--0000-0000-0000-001f`
where the last 12 digits represent the device ID in HEX, so `0x00000000001f == 0x1f == 31`.
The ID of your device should be shown on the label sticker on your PODuino.

- Using the IOTile tools, you connect to a given device with `iotile hw --port=bled112 connect <ID>`
(using ID=0x1f, `iotile hw --port=bled112 connect 0x1f`). Commands can be chained, so you can
also do `iotile hw --port=bled112 connect 0x1f get 11` to connect to device `0x1f` and then to
tile `11`.

- IOTile Devices are build from a set of Tiles, each responding to a set of RPC commands. 
In the case of the PODuino, the Arduino Mega bridge is represented as Tile number `11`.

- Tile `11` has a very simple interface, supporting basically two basic commands:

   - `send_event` can be used to send an event code, currently supporting `0` to `9` as
   possible events to send
   - `last_event` returns the last value sent from the Arduino via the `bridge.sendEvent()`.
   which returns the stream input ID, `10` in our example, and the actual value sent, which
   in our case, is the value of `count`.

## 4. Connecting and controller our Arduino via the IOTile device

Now that we understand the most basic functionlity of the PODuino IOTile bridge, we can actually
control our device. 

Note that in this example, the sketch only uses event `0`, which is uses to reset the count to zero.

```
# Connect to Device d--0000-0000-0000-001f and to the Arduino bridge
iotile hw --port=bled112 connect 0x1f get 11

# Check the last count (3 assumes you pressed the up button three times)
(ArduinoBridge) last_event
value: 3
stream: 10

# Reset count to zero
(ArduinoBridge) send_event 0

# Confirm that count is now zero
(ArduinoBridge) last_event
value: 0
stream: 10
```

## 5. Introduction to the sensor graph

The IOTile devices can be configured with what is called a Sensor Graph, which can be
considered as a simple app which basically represent the data flow. So for example, 
the sensor graph can be configured to take readings every 10min, or 1hr.

The sensor graph is specially useful when you want to use the device as a data logger,
as it automatically handles the storage of any output you define, and then configured
to send a *streamer report* (which is basically a data file with all readings) when 
somebody connects via the BLE. If using the *IOTile Companion* App or *IOTile Gateway*,
this streamer report will automatically be uploaded to the *IOTile Cloud*, taking care
of an incredibly amount of complexity for you.

In our example, the Sensor Graph makes the device read `input 10` every time that it
changes, and copies the value to `output 1` which represents the data stream.

The `add_streamer` lines in the sensor graph adds both a user and system streamer,
and configures them to be sent together. Note that the system report should ALWAYS
be sent when you send your own streamer. This is critical because the system
report contains information about reboots, which are used to timestamps (as the
IOTile device clock counts time from the last reboot of the device). The
streamer contains all `output 1` readings (remember that `output 1` came from `input 10`
which itself came  from the Arduino via `bridge.sendEvent()` command).

## 6. Programming the Sensor Graph

The IOTile Coretools does not only allow you to control the device via the BLE112 dongle,
but also allows you to program the sensor graph via BLE.

By default all events received from the Arduino are forwarded on the IOTile Controller for processing in Sensor Graph, but without an appropriate configuration they will just be ignored.

In our example, the Arduino is configured to put out an event in stream 10 every time an event occurs and we want to log that event to flash storage and send it to the cloud automatically with the name output 1.

First, connect to your pod and navigate to the SensorGraph manager (again, assuming device `0x1f`)

```
> iotile hw --port=bled112 connect 0x1f controller sensor_graph
(SensorGraph)

# Now delete whatever SensorGraph might be there:
(SensorGraph) disable
(SensorGraph) clear
(SensorGraph) reset
```

Now enter the following commands (you can just copy-paste them directly into your terminal):

```
disable
clear
reset

add_node "(input 10 always) => output 1 using copyA"

add_node "(system input 1025 always && constant 1 always) => unbuffered node 20 using triggerStreamer"
set_constant "constant 1" 0

add_streamer "all outputs" controller False hashedlist telegram
add_streamer "all system outputs" controller False hashedlist telegram --withother=0

persist
enable
```

The first three lines clear any old sensor graph and prepare for programming a new one. The add_node line is what tells SensorGraph to listen for values on input 10 and copy them to flash with the name output 1 (0x5000).

The next add_node and set_constant line tell SensorGraph to stream historical data when someone connects to the device

The two add_streamer lines configure sensor graph to send data to the cloud in the form of a RobustReport.

The persist line saves the sensor graph to flash so it persists across device resets and the enable line starts the new sensor graph.

Now that you have done this step by step at least once, you can use the following command to
program the sensor graph and confirm it worked (again, change `0x1f` with your own device ID):

```
prepare_device.sh 0x1f
```

## 7. The IOTile Cloud

The **IOTile Cloud** was built especially to work with **IOTile Devices**, so it makes it really
easy for you to upload data, visualize it, and access it via a simple Rest API.

Data is represented as follows:

- Users belong to Organizations
- Organizations own Projects
- IOTile Devices are claimed into Projects, and therefore data sent from devices is organized
by Project.
- Data sent by the IOTile Device is represented as *Streams*, which are formed from a
   - A globally unique Project ID
   - Your globally unique device ID
   - A variable ID representing the `output stream` you used in your device (i.e. `output 1`).
   The variable is used to mapped the encoded output stream ID (`output 1`) to a readable, user
   friendly name (say, `COUNT`). Trying to keep this tutorial simple, we are not explaining 
   how to come up with the vaiable ID that you need to use, but for now, just know that `output 1`
   is represented as ID `0x5001` just as `output 2` would be represented by `0x5002` and so on.
   - The stream ID is then formatted as `s--<projectID>--<deviceID>--<variableID>`. In our example,
   we would need Stream ID `s--0000-1234--0000-0000-0000-001f--5001` where we assume the project ID is 
   `0x1234`.
- The data stream is always a *Time Series* with a timestamp and a value.

## 8. Register to IOTile Cloud

Get an account at https://iotile.cloud

Make sure you check your email and confirm the registration.

## 9. Setup an Organization and Project

Create an Organization and a Project. Remember that this Project will contain your device
and stream data.

## 10. Download the IOTile Companion App

The **IOTile Companion** Mobile App enables you to easily claim and connect to the NFC300 via Bluetooth Low Energy (BLE) using your mobile phone or tablet.
Install and open the IOTile Companion App from the iOS or Google Play App Store.

[Apple Store](https://itunes.apple.com/ca/app/iotile-companion/id1142010560?mt=8)

[Google Play](https://play.google.com/store/apps/details?id=com.archiot.iotileapp&hl=en)

## 11. Claim your PODuino Device

1. Using the **IOTile Companion**, login using your email and password. 
1. Tap the menu icon in the upper left to bring up the main menu. You will need to enable Bluetooth on your mobile phone or tablet to complete the setup.
1. Select “New Device Setup”. Make sure you are within 50 feet of your PODuino as you scan for it.
1. Select your device. If there are multiple choices, match your selection with the number on the side of the PODuino you want to set up.
1. Select the project you created previously on https://iotile.cloud
1. Name your PODuino
1. Set the location and finish process

After you claim your device, the Device ID will be associated to your project.

You are now ready to collect data.

## 12. Setup Variables

Go back to https://iotile.cloud, and on your project page, use the left menu, 
under *Project Settings*, select *Data Stream Variables*, and click **New Variable**:

- **Name:** Count
- **ID (Hex):** 5001
- **Description:** PODuino Counter (Output 1)
- **VarType:** Default (u)
- Check **Web Only**

After creating the Variable, click on **IO Configuration**:

- Leave **Multiply**, **Divide** and **Offset** with thedefaults.
- **Input Units**: Unit
- **Value Type**: int
- **Output Units**: Unit

While not needed in this case, the **Multiply**, **Divide** and **Offset** could be
used to make the cloud modify each value that it receives by `value * / d + o`.

Note that you should NOT pick any other option as **VarType** as those may not work 
as expected with your PODuino.

## 13. Review your Data Stream

After creating the variable, you should automatically get a Data Stream created
for you.
Use the left menu, under *Project Settings*, select *Data Streams*, and confirm
that you have a stream that matches your device and variable.

## Upload Data

Go back to the PODuino, power it up, and start playing with the UP and DOWN buttons.
When you are ready to upload the data, use the **IOTile Companion** to select the 
project.

If you are within 50 feet, click on the **Collect All** button in the bottom of
the app. This process will connect to the device, read the streamer report generated
(which we previously configured with the Sensor Graph), and will upload it to the cloud.

It usually takes a few minutes for the data to propagate to the database, but at this
point, you can go to the [IOTile Web App](https://app.iotile.cloud) and you should see
a graph with the `count` data sent by your PODuino.

## Have fun!!!

At this point, you have successfully managed to control the PODuino and build a data
logger. All with BLE support, and without writing any web site code.









