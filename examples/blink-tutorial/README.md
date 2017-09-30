# LED Example

This is the simplest example to demonstrate how to use the PODuino and IOTile core tools to remotely make an LED turn On or Off.

## Part 1: Make the PODuino Blink

The PODuino can be used just like any other **Arduino Mega 2560**, and theefore, any example you can find on the web for this board should work on the PODuino.

So, lets start with the traditional [Blink Example](https://www.arduino.cc/en/Tutorial/Blink) usually used as the first tutorial for a new Arduino user. 

If you have not done it, make sure you have the [latest Arduino IDE](https://www.arduino.cc/en/Main/Software).

But lets get the PODuino version of this example:

### HW Setup

The equivalent of the **Hello World!** for an Arduino is to make its internal LED blink. This can be done on the PODuino (as with most Arduinos) by just following the [Blink Example](https://www.arduino.cc/en/Tutorial/Blink). 

Basically, simple open the Arduino IDE, create a new file, and copy the following code:

```
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```

The first thing you do is to initialize LED_BUILTIN pin as an output pin with the line

```
pinMode(LED_BUILTIN, OUTPUT);
```

In the main loop, you turn the LED on with the line:

```
digitalWrite(LED_BUILTIN, HIGH);
```

which creates a voltage difference across the pins of the LED, and lights it up. Then you turn it off with the line:

```
digitalWrite(LED_BUILTIN, LOW);
```

The loop uses a couple of one second delays between the ON and OFF commands:

```
delay(1000);
```

Connect your PODuino to your computer via the USB, and using the Arduino IDE, click on **Upload** to program the Arduino portion of the PODuino and see the internal LED blink.

## Part 2: Controlling the LED via Bluetooth Low Energy

### 2.1. Setting up IOTile Core Tools

Lets now use the IOTile side of the PODuino to remotely turn the same LED ON or OFF. This will demonstrate the value of the PODuino over other traditional Arduino boards.

#### Install the IOTile Arduino Bridge

The Arduino Mega in the PODuino is connected to an GPIO IOTile, which while simple, allows you to both control the Arduino and have the Arduino communicate back with the IOTile side. 

To make this easy, an Arduino IDE library has to be downloaded and installed on your local IDE.

1. Downlaod the Library from the [/downloads directory](https://github.com/iotile/poduino/tree/master/downloads)
2. From the Arduino IDE, choose Sketch -> Include Library -> Add .zip Library and select the IOTileBridgeMega.zip you just downloaded.

#### Install the Python core tools

Make sure you have Python 2.7.9+ installed. Like with any other Python package, it is highly recommended that you use `virtualenv` to install these packages on:

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

#### BLE Dongle

IOTile Tools currently only support communication to the IOTile Controller in the PODuino via
the [BLE112 Dongle](https://www.digikey.com/catalog/en/partgroup/bled112-bluetooth-smart-dongle/40600)
which either came with your PODuino or you can buy from Arch Systems or other sources.

Connect the BLE112 to another USB port in your computer. Note that at least while programming the Arduino Mega, you may need both the USB cable to the PODuino and the BLE112.

### 2.2. Sketch to enable/disable LED Blink

Modify the Arduino Sketch as following:

```
#include <IOTileBridgeMega.h>

#define kAttentionPin 39
void onEventReceived(unsigned int event);
IOTileBridge bridge(kAttentionPin, onEventReceived);

int active=0;

void setup() {
  // Initialize IOTile Bridge
  bridge.begin();
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (active) {
    // When active, enable LED to blink
    int j;
    for (j=0; j<10; ++j)
    {
      bridge.checkReceivedEvents(); // Check for events
      delay(100);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    for (j=0; j<10; ++j)
    {
      bridge.checkReceivedEvents(); // Check for events
      delay(100);
    }
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    // If not enabled, make sure LED is OFF
    digitalWrite(LED_BUILTIN, LOW);
    bridge.checkReceivedEvents(); // Check for events
    delay(100);
  }
}

void onEventReceived(unsigned int event)
{
  if (event) {
    active = 1; // Enable Blinking
  } else {
    active = 0; // Disable Blinking
  }
}
```

Now, instead of just blinking the LED, we are configuring the Arduino to respond to any signal sent via Pin 39, which is a digital pin that connects the Arduino Mega with the GPIO IOTile. When a new value comes via pin39, the `onEventReceived()` function is called, which simply checks the event ID to know to enable or disable the LED blinking.

Save the new sketch, and click on the **Upload** button to upload it to the Arduino Mega in the PODuino. 

At this point, you can either leave the USB Cable connected to the computer (to power it up, for example), or you can disconnect from the computer and use an alternative USB charger.

### 2.3. Remotely enableling or disableling the LED Blink

With the BLE112 dongle, and on the same terminal where you have your python virtualenv running.

Before we type any commands, we need the **IOTile Device ID** for your PODuino. You are linkely to find this ID on a sticker on your PODuino board.

<img src="/images/iotile-device-id.jpg" alt="PODuino Device ID" width="200">

You can also get the PODuino ID by scanning using for IOTile Devices. Use the following command to scan for IOTile Devices (including your PODuino). Assumes you are within 50 feet from your device:

```
(iotile)$ iotile hw --port=bled112 scan
{
    "connection_string": "F9:B9:B3:C1:8B:3E",
    "expiration_time": "2017-09-30 14:58:25.205869",
    "low_voltage": false,
    "pending_data": false,
    "signal_strength": -60,
    "user_connected": false,
    "uuid": 456
}
```

The `uuid` field at the end shows you the IOTile Device ID (as an integer).

Replacing `<UUID>` with your own PODuino ID, type the following command

```
(iotile)$ iotile hw --port=bled112 connect <UUID> get 11
```

Examples

```
(iotile)$ iotile hw --port=bled112 connect 0x1c8 get 11
(iotile)$ iotile hw --port=bled112 connect 456 get 11
```

you should now see the followng on your console:

```
(ArduinoBridge)
```

Now you can type `send_event` with a `0 or 1` to control the LED:

```
(ArduinoBridge) send_event 1
(ArduinoBridge) send_event 0
```

As said before, after programming teh Arduino portion, you no longer need to keep the USB cable connected to the computer. You can either connec that same cable to a USB charger, or you can use an **Arduino Mega Charger** (Not provided). This means that we can now control the Arduino remotely via the BLE112 dongle and the IOTile Controller and IOTile GPIO on the PODuino.

## Part 3: Reading Data from the PODuino

For simplicity, we will not connect this example to any actual sensors, so instead we will simply log every time the LED Blink is enabled, or disabled, so we know at what time the operation was done. This information will be stored on the device memory, the same way we would be storing sensor data collected by the Arduino side (e.g. temperature readings).

### 3.1. Sketch to log when LED Blink is enabled/disabled

Modify the `onEventReceived()` from the previous sketch with the following:

```
void onEventReceived(unsigned int event)
{
  if (event) {
    active = 1; // Enable Blinking
  } else {
    active = 0; // Disable Blinking
  }
  // Log Event on IOTile Stream Local ID 10
  bridge.sendEvent(10, event);
}
```

We are simply sending the value of `event` everytime an event is recived. We are attaching that event value to `Input 10` (We could have used any stream id you want between 0 and 128). Note that this is an `Input` from the IOTile side point of view.

Now, back to the Python console, type

```
(ArduinoBridge) send_event 1
(ArduinoBridge) last_event
value: 124
stream: 10
(ArduinoBridge) send_event 0
(ArduinoBridge) last_event
value: 0
stream: 10
(ArduinoBridge) quit
```

You can see how `last_event` shows the last value sent by the Arduino via the `bridge.sendEvent()`.

#### But what's `Input 10` anyway? The Sensor Graph

By default all events received from the Arduino are forwarded on the IOTile Controller for processing in Sensor Graph, but without an appropriate configuration they will just be ignored.

In our case, we configured the Arduino to put out an event in stream 10 every time an event occurs (the blink is enabled or disabled) and you want to log that event to flash storage and send it to the cloud automatically with the name output 10.

### 3.2 The Sensor Graph

First, connect to your pod and navigate to the SensorGraph manager:

```
(iotile)$ iotile hw --port=bled112 connect <UUID> controller sensor_graph
(SensorGraph)
```

Now delete whatever SensorGraph might be there:

```
(SensorGraph) clear
(SensorGraph) reset
(SensorGraph) disable
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

The first three lines clear any old sensor graph and prepare for programming a new one. The add_node line is what tells SensorGraph to listen for values on input 10 and copy them to flash with the name `output 1` (`0x5001`).

The next add_node and set_constant line tell SensorGraph to stream historical data when someone connects to the device

The two add_streamer lines configure sensor graph to send data to the cloud in the form of a RobustReport.

The persist line saves the sensor graph to flash so it persists across device resets and the enable line starts the new sensor graph.

Lets go back and log some events. Assuming you have not quit the IOTile console, just type

```
(SensorGraph) back
(NRF52832Controller) back
(HardwareManager) get 11
(ArduinoBridge) send_event 1
(ArduinoBridge) send_event 0
(ArduinoBridge) send_event 1
(ArduinoBridge) back
(HardwareManager) controller
(NRF52832Controller) sensor_graph
(SensorGraph) inspect_virtualstream 'input 10'
124
(SensorGraph) download_stream 'output 1'
Downloading 3 readings: [########################################] 0.1 seconds total   
Stream 20481: 124 at 2017-09-30 15:42:33.178995
Stream 20481: 0 at 2017-09-30 15:42:38.178995
Stream 20481: 124 at 2017-09-30 15:42:43.178995
```

The example above not only shows how to navigate though the IOTile console (`shell`) but after sending a few events, it shows how to check that everything is working with the Sensor Graph by inspecting the input 10 stream:

```
(SensorGraph) inspect_virtualstream 'input 10'
124
```

This shows the last value (in this case `124`) received from the Arduino and updates every time a new event is received.

The last command, `download_stream` is used to download the stream. You can see how three events were downloaded, which makes sense given that we sent three `sent_event` commands a few lines above, so we expected three records to have been recorded. You can see how the `output 1` stream is reported (`20481` or `0x5001`), as well as the event value, and the timestamp.

At this point, we have shown how to send information to the Arduino, and how the Arduino can send streaming information back. Note that an IOTile is optimized for low power, and in most IoT low power applications, we should be careful not to send too much information. For example, if you were measuring temperature, you may want to only store a value every ten minutes or so. This is something that can be controlled on the sensor graph (see [documentation](http://coretools.readthedocs.io/en/latest/tutorials.html#introduction-to-sensorgraph)).

## Part 4: The IOTile Cloud

The **IOTile Cloud** was built especially to work with **IOTile Devices**, so it makes it really easy for you to upload data, visualize it, and access it via a simple Rest API.

See [IOTile Cloud Basics](../../docs/iotile-cloud-basics.md)

### 4.1. Setup Variables

Go back to https://iotile.cloud, and on your project page, use the left menu,
under *Project Settings*, select *Data Stream Variables*, and click **New Variable** :

- **Name:** Count
- **ID (Hex):** 5001
- **Description:** PODuino Counter (Output 1)
- **VarType:** Default (u)
- Check **Web Only**

After creating the Variable, click on **IO Configuration**:

- Leave **Multiply**, **Divide** and **Offset** with the defaults.
- **Input Units**: Unit
- **Value Type**: int
- **Output Units**: Unit

### 4.2. Upload Data

Go back to the PODuino, power it up, and start playing with the UP and DOWN buttons.
When you are ready to upload the data, use the **IOTile Companion** to select the
project.

If you are within 50 feet, click on the **Collect All** button in the bottom of
the app. This process will connect to the device, read the streamer report generated
(which we previously configured with the Sensor Graph), and will upload it to the cloud.

It usually takes a few minutes for the data to propagate to the database, but at this
point, you can go to the [IOTile Web App](https://app.iotile.cloud) and you should see
a graph with the `count` data sent by your PODuino.

### 4.3. API Access to the Data.

Using the API, you can access the Stream Data when ever needed. 

Use https://iotile.cloud/api/v1/data/?filter=s--0000-XXXX--0000-0000-0000--YYYY-5001 to show all data for a given stream. Replace `XXXX` with your Project ID and `YYYY` with your device ID. 

Or get the following APIs to get the list of your projects, devices or streams:

- **Projects**: https://iotile.cloud/api/v1/project/
- **Devices**: https://iotile.cloud/api/v1/device/
- **Streams**: https://iotile.cloud/api/v1/stream/

## Have fun!!!

At this point, you have successfully managed to control the PODuino and build a data
logger. All with BLE support, and without writing any web site code.







