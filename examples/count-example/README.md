# PODuino Example - Up/Down Counter

This simple example demonstrates how to use the PODuino and the OSEPP LCD display for a basic counting application.  The PODuino will record each time the count changes and allow you to upload the historical record to IOTile Cloud over Bluetooth Low Energy.  

## Materials

1. PODuino v1.0
2. BLE112 Dongle
3. [OSEPP *16×2 LCD Display & Keypad Shield*](https://www.osepp.com/electronic-modules/shields/45-16-2-lcd-display-keypad-shield)

## Software

You will need access to following tools:

1. Install the [IOTile Coretools](/docs/installation/iotile-coretools.md) 
2. Install the [IOTile Arduino Bridge](/docs/installation/iotile-arduino-bridge.md)

# Introduction

### Sketch

Using the Arduino IDE, open the `firmware.ino` file included in this example.

This sketch will listen to the Up and Down buttons in the LCD display and, when pressed,
will increment or decrement an in-memory counter. This counter is restricted within the range 0 to 100.

The sketch will also send the counter value every time it is modified to `input 10` in the IOTile Controller.
Later we will describe the required IOTile Configuration to have this input copied to a buffered output that
will be sent to the cloud every time the IOTile Mobile app or an IOTile Gateway connects to the PODuino.

Finally, the sketch also listens to `Event 0` from the IOTile Controller, and will reset the counter when
called.

### Programming the PODuino

The PODuino requires two separate steps to configure, programming the Arduino-side and IOTile-side:

1. Arduino-side: The Arduino Mega is programmed via a USB Cable and a computer the same way any Arduino compatible board is programmed. There are a number of tutorials and blogs explaining how to program an Arduino. Just make sure you use instructions compatible with the *Arduino Mega 2560*.

2. IOTile-side: The IOTile portion is pre-programmed with its firmware, so you can create what we call a SensorGraph 
to configure the device for your own data logging. We will explain how to configure the SensorGraph later on.

# Tutorial

## Part 1: Arduino

### 1. Hardware Setup

Connect the PODuino with a USB Cable to a compute; you will need the device to be connected
to upload the program initially. The computer should have both the Arduino IDE and IOTile Tools installed.

Attach the LCD display shield to your PODuino board as shown below:

INSERT IMAGE HERE

### 2. Upload the Sketch to the Arduino

Follow instructions in [Arduino Mega 2560 Getting Started](https://www.arduino.cc/en/Guide/ArduinoMega2560).

At this point, the Arduino portion is configured and you can disconnect the USB cable if you wish.
The USB cable is only used for programming the Arduino portion, but can also be used as power source.

You should see the LCD Display show *Poduino Counter*, showing *0* as the current value of the counter. If you press the Up or Down buttons, you should see this value change.

## Part 2: IOTile PODuino Controller

### 3. Introduction to the IOTile Coretools

Probably one of the key advantages of the PODuino over other Arduino boards is the fact that you
can use the IOTile Device and IOTile Tools to control or read information from the Arduino via
BLE.

IOTile Tools currently only support communication to the IOTile Controller in the PODuino via
the [BLE112 Dongle](https://www.digikey.com/catalog/en/partgroup/bled112-bluetooth-smart-dongle/40600)
which either came with your PODuino or you can buy from Arch Systems or other sources.

Once you connect the BLE112 dongle to your computer, you can immediately use it to communicate with
the PODuino in general, and the Arduino Mega in particular. Make sure you are within 50 feet of your PODuino as you scan for it.

See [IOTile Coretools Documentation](http://coretools.readthedocs.io/en/latest/) to learn the full
power of the IOTile Platform.

To understand the basic functionality needed to control the PODuino, you need to understand the
following key concepts:

- All IOTile Devices are represented with a globally unique ID similar to `d--0000-0000-0000-001f`
where the last 12 digits represent the device ID in HEX, so `0x00000000001f == 0x1f == 31`.
The ID of your device should be shown on the label sticker on your PODuino.

- Using the IOTile Coretools, you connect to a given device from your terminal with `iotile hw --port=bled112 connect <ID>`
(using ID=0x1f, `iotile hw --port=bled112 connect 0x1f`). Commands can be chained, so you can
also do `iotile hw --port=bled112 connect 0x1f get 11` to connect to device `0x1f` and then to
tile `11`.

- IOTile Devices are built from a set of Tiles, each responding to a set of RPC commands.
In the case of the PODuino, the Arduino Mega bridge is represented as Tile number `11`.

- Tile `11` has a very simple interface, supporting basically two basic commands:

   - `send_event` can be used to send an event code, currently supporting `0` to `9` as
   possible events to send
   - `last_event` returns the last value sent from the Arduino via the `bridge.sendEvent()`.
   which returns the stream input ID, `10` in our example, and the actual value sent, which
   in our case, is the value of `count`.

### 4. Connecting and controlling our Arduino via the IOTile device

Now that we understand the most basic functionality of the PODuino IOTile bridge, we can actually
control our device.

Note that in this example, the sketch only uses event `0`, which it uses to reset the count to zero.

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

### 5. Introduction to the sensor graph

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

### 6. Programming the Sensor Graph

The IOTile Coretools does not only allow you to control the device via the BLE112 dongle,
but also allows you to program the sensor graph via BLE.

By default all events received from the Arduino are forwarded on the IOTile Controller for processing in Sensor Graph, but without an appropriate configuration they will just be ignored.

In our example, the Arduino is configured to put out an event in stream 10 every time an event occurs and we want to log that event to flash storage and send it to the cloud automatically with the name output 1.

A sample `sensor-graph.sgf` is included. It simply listens to `input 10`, and copies its values to `output 1`. To compile and download to the device, you need to:

```
pip install --upgrade iotile-sensorgraph 
iotile-sgcompile sensor-graph.sgf -f snippet | iotile hw --port=bled112 connect <UUID> controller sensor_graph
```

Now that you have done this step by step at least once, you can use the following command to program the sensor graph and confirm it worked:

```
# prepare_device.sh <UUID>
# e.g.
prepare_device.sh 0x01f
```

## Part 3: The IOTile Cloud

The **IOTile Cloud** was built especially to work with **IOTile Devices**, so it makes it really easy for you to upload data, visualize it, and access it via a simple Rest API.

See [IOTile Cloud Basics](../../docs/iotile-cloud-basics.md)

### 7. Setup Variables and Streams

For this example, we will use a Python script to create the required variable and stream for `Output 1` ('5001').

Using the provided `cloud-create-stream.py` script, do

```
pip install -U iotile-cloud  # if you have not done it
python cloud-create-stream.py --device=d--xxxx 0x5001
```

replacing xxxx with your PODuino device id.

At this time, if you go back to https://iotile.cloud and select you Project, you can see the project
variables and streams using the left side menu.

You are now ready to upload data to this stream.

### 8. Upload Data

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
