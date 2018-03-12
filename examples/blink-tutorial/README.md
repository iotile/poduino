# PODuino Tutorial - Blink

This simple example demonstrates the ease of connecting any Arduino project to a low power wireless network via IOTile. In this tutorial, we will use the Arch PODuino and IOTile core tools to remotely make an LED turn On or Off via Bluetooth Low Energy.

## Part 1: Make the PODuino Blink

The Arch PODuino is meant to be used just like any other **Arduino Mega 2560**, while providing a bridge to IOTile.  Therefore, any Arduino example you can find on the web for this board should work on the PODuino as well.

So, let's start with the traditional [Arduino Blink Example](https://www.arduino.cc/en/Tutorial/Blink) usually used as the first tutorial and equivalent of **Hello World!** for a new Arduino user. 

If you don't have it yet, make sure you are using the [latest Arduino IDE](https://www.arduino.cc/en/Main/Software).

**Important**: If you are on Linux, be sure to have followed the [Arduino installation tutorial](https://www.arduino.cc/en/Guide/Linux#toc6) to
allow non-root access to the serial port (else you will have errors while uploading your sketch).

Now we illustrate the PODuino version of the Blink example:

### HW Setup

Open the Arduino IDE, create a new file, and copy the following code:

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

Initialize the LED_BUILTIN pin as an output pin:

```
pinMode(LED_BUILTIN, OUTPUT);
```

In the main loop, turn on the LED by setting the pin to HIGH:

```
digitalWrite(LED_BUILTIN, HIGH);
```

This pulls the voltage HIGH across the LED, turning it ON. To turn the LED back OFF, switch the voltage to LOW:

```
digitalWrite(LED_BUILTIN, LOW);
```

Note that the code above uses a couple of one second delays between the ON and OFF commands to make the blinking easily visible:

```
delay(1000);
```

Now connect the PODuino to your computer via USB, and using the Arduino IDE, click on **Upload** to program the Arduino Mega.  The internal LED should blink.  Hello World!

## Part 2: Controlling the LED via IOTile and Bluetooth Low Energy

### 2.1. Setting up IOTile Core Tools

Let's now use the IOTile side of the PODuino to say Hello World i.e. remotely turn the same LED ON or OFF via wireless Bluetooth Low Energy. This will demonstrate the ease of integrating IOTile and low power wireless capabilities into all kinds of Arduino projects.  

#### Install necessary Software Tools

You will need access to Python 2.7+ and the following tools.  Install them now as needed:

1. Install the [latest Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Install the [IOTile Coretools](/docs/installation/iotile-coretools.md) 
3. Install the [IOTile Arduino Bridge](/docs/installation/iotile-arduino-bridge.md)

#### BLE Dongle

Currently, communication to the PODuino IOTIle Controller is only supported via the [BLED112 Dongle](https://www.digikey.com/catalog/en/partgroup/bled112-bluetooth-smart-dongle/40600).  You should have received this dongle with your PODuino, but if not it can be bought separately from Arch Systems or other online vendors. 

Connect the BLED112 dongle to another USB port in your computer. Note that while programming the Arduino Mega, you may need to keep the USB cable connection intact in addition to the wireless BLED112 dongle connection.

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

Now, instead of just blinking the LED, we are configuring the Arduino to respond to any signal sent via Pin 39, which is a digital pin that connects the Arduino Mega with the GPIO IOTile. When a new value comes via pin39, the `onEventReceived()` function is called. This function checks the event ID to know whether to enable or disable LED blinking.

Save the new sketch, and click on the **Upload** button to program the Arduino Mega. 

You no longer need the USB cable for programming, but do need to continue powering the PODuino.  Leave the USB cable connected to the computer for power or use an alternative USB charger.

### 2.3. Remotely enableling or disableling the LED Blink

Now we will remotely enable and disable the LED blink with the same terminal we have the python virtualenv runnning.  

Before we type any commands, we need the **IOTile Device ID** for your PODuino. Find the ID on the device label on your PODuino board.

<img src="/images/iotile-device-id.jpg" alt="PODuino Device ID" width="200">

You can also get the PODuino's Device ID by scanning. Use the following command to scan for any powered IOTile Devices in the vicinity of your BLE wireless network:

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

Replace `<UUID>` with your own PODuino ID, and type the following command to connect to your PODuino through BLED112:

```
(iotile)$ iotile hw --port=bled112 connect <UUID> get 11
```

`get <tileID>` is an iotile command to get access to a specific tile (here 11 is the Arduino board)

It should look something like this:

```
(iotile)$ iotile hw --port=bled112 connect 0x1c8 get 11
(iotile)$ iotile hw --port=bled112 connect 456 get 11
```

After exectuing the command, you should see the followng on your console:

```
(ArduinoBridge)
```

Now you can type `send_event` with a `0` or `1` to control the LED:

```
(ArduinoBridge) send_event 1
(ArduinoBridge) send_event 0
```

Now you are triggering the LED Blink over an IOTile wireless network! Next step is to understand how to extend this capability to data reading and cloud integration. 

## Part 3: Reading Data from the PODuino

You may want to connect all sorts of more interesting sensors for data collection, but to begin with, we will simply log data for every time the LED Blink is enabled, or disabled.  This will generate a time series stream of LED status, very similar to logging the uptime or downtime of a connected appliance. The data will be stored initially in device memory on the IOTile, the same way sensor data is stored on the Arduino side in typical Arduino examples. 

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

Here we send the value of `event` everytime an event, the LED blink in this case, occurs. We are attaching that event value to `Input 10` (any stream id between 0 and 128 is valid). Note that this event stream is an `Input` from the IOTile perspective, as it will be injected over the wireless network into the cloud.

Now, turning back to the Python console, input the following:

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

You can see how `last_event` shows the last value sent by the Arduino Mega via the `bridge.sendEvent()`.

#### But what's `Input 10` anyway? Introducing SensorGraph

By default all events received from the Arduino Mega are forwarded to the IOTile Controller for processing via the SensorGraph.  SensorGraph is a core part of the IOTile architecture [Read more here - coming soon]. Effectively, it is a graph-based scheduler that allows for easy algorithm development within an optimized environment.  This is a key component in how IOTile allows one to design custom wireless systems and automatically achieve secure, power-optimized, and latency-optimized configurations.  

In this example, we configured the Arduino Mega to put out an event in stream 10 every time the LED blink occurs.  This event is logged in flash and will be sent to the cloud automatically with the name output 10.  This configuration is important or else the stream will be ignored by SensorGraph.

### 3.2 The SensorGraph

First, connect to your PODuino via Bluetooth and navigate to the SensorGraph manager:

```
(iotile)$ iotile hw --port=bled112 connect <UUID> controller sensor_graph
(SensorGraph)
```

Now clear the SensorGraph currently in place:

```
(SensorGraph) clear
(SensorGraph) reset
(SensorGraph) disable
```

Enter the following commands (you can just copy-paste them directly into your terminal):

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

The first three lines clear any old sensor graph and prepare for programming a new one [David - why do you have to do the same commands again in a different order.  Didn't you just do them above?]. The add_node line tells SensorGraph to listen for values on input 10 and to copy them to flash with the name `output 1` (`0x5001`).

The second add_node and set_constant line tell SensorGraph to stream historical data when someone connects to the device.

The two add_streamer lines configure SensorGraph to send data to the cloud in the form of a Robust Report [Read more about Robust Reports- coming soon].

The persist line saves the SensorGraph to flash so it persists across device resets.  And finally, the enable line starts the new SensorGraph.

Now let's go back and log some events. Assuming you have not quit the IOTile console, just enter:

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

The example above not only shows how to navigate though the IOTile console (`shell`), but after sending a few events, it shows how to check that everything is working with the SensorGraph you created by inspecting the input 10 stream:

```
(SensorGraph) inspect_virtualstream 'input 10'
124
```

This shows the last value (in this case `124`) received from the Arduino Mega.  It updates every time a new event is received.

The last command, `download_stream`, is used to download the stream. You can see how three events were downloaded, which makes sense given that we sent three `sent_event` commands a few lines above, so we expected exactly three records. You can see the format of the `output 1` stream (`20481` or `0x5001`), as well as the event value, and the timestamp.

At this point, we have reviewed how to send information to the Arduino Mega, and how the Arduino Mega can send streaming information back through IOTile. Note that IOTile is optimized for low power networks.  A key part of achieving low power, as well as a best practice for security and robustness, is processing most data at the point of collection and sending only necessary information to other nodes or the cloud. For example, if you were measuring temperature, you may want to only store the instantaneous value every ten minutes or even an average over a greater period of time. This is something that can be easily configured on the SensorGraph (see [documentation](http://coretools.readthedocs.io/en/latest/tutorials.html#introduction-to-sensorgraph)).

## Part 4: IOTile Cloud

**IOTile Cloud** was built to provide automatic cloud integration for any **IOTile Device**, making it easy to upload, store, and visualize data as well as access it for 3rd party application development via a simple Rest API.

See [IOTile Cloud Basics](../../docs/iotile-cloud-basics.md)

### 4.1. Setup Variables

Go back to https://iotile.cloud. On the project page, using the left menu 
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
The easiest way to upload data is to use the companion mobile app.  Download **IOTile Companion** 
from the iOS or Android app stores if you don't have it already, log in, select your project, 
and scan for the device. 

If you are within 50 feet, you will see a `green light` icon next to the device.  
Click on the **Collect All** button in the bottom of the app. 
This process will connect to the device, read the streamer report generated
(which we previously configured with the SensorGraph), and upload it to the cloud.

It usually takes a few minutes for the data to propagate to the database.  At this
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

Congratulations! You moved from blinking the Arduino Mega LED via the standard Arduino IDE 
to setting up an IOTile low power wireless network with a cloud-connected data logger.  You've
done a complete device-to-cloud integration without any of the typical complexity of wireless
development or web site code.  What will you build next?
