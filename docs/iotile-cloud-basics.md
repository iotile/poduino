# The IOTile Cloud

## Basic Overview

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
   how to come up with the variable ID that you need to use, but for now, just know that `output 1`
   is represented as ID `0x5001` just as `output 2` would be represented by `0x5002` and so on.
   - The stream ID is then formatted as `s--<projectID>--<deviceID>--<variableID>`. In our example,
   we would need Stream ID `s--0000-1234--0000-0000-0000-001f--5001` where we assume the project ID is
   `0x1234`.
- The data stream is always a *Time Series* with a timestamp and a value.

## Step by step instructions

Register and/or Login and create an Organization and/or Project by:

### 1. Register to IOTile Cloud

Get an account at https://iotile.cloud

Make sure you check your email and confirm the registration.

### 2. Setup an Organization and Project

Create an Organization and a Project. Remember that this Project will contain your device
and stream data.

### 3. Download the IOTile Companion App

The **IOTile Companion** Mobile App enables you to easily claim and connect to the NFC300 via Bluetooth Low Energy (BLE) using your mobile phone or tablet.
Install and open the IOTile Companion App from the iOS or Google Play App Store.

[Apple Store](https://itunes.apple.com/ca/app/iotile-companion/id1142010560?mt=8)

[Google Play](https://play.google.com/store/apps/details?id=com.archiot.iotileapp&hl=en)

### 4. Claim your PODuino Device

1. Using the **IOTile Companion**, login using your email and password.
1. Tap the menu icon in the upper left to bring up the main menu. You will need to enable Bluetooth on your mobile phone or tablet to complete the setup.
1. Select “New Device Setup”. Make sure you are within 50 feet of your PODuino as you scan for it.
1. Select your device. If there are multiple choices, match your selection with the number on the side of the PODuino you want to set up.
1. Select the project you created previously on https://iotile.cloud
1. Name your PODuino
1. Set the location and finish process

After you claim your device, the Device ID will be associated to your project.

You are now ready to collect data.

## Variable and Streams

Before you can upload any data to IOTile Cloud, you need to create a *Stream* record. A *Stream* represents a given device output under a given project. It therefore requires a *Project*, *Device* and *Variable*. The *Variable* represents a given output in the device's [Sensor Graph](http://coretools.readthedocs.io/en/latest/tutorials.html#introduction-to-sensorgraph).

After claiming a device under a given Project, all you need is to create a new Variable under the Project. Go back to https://iotile.cloud, and on your project page, use the left menu,
under *Project Settings*, select *Data Stream Variables*, and click **New Variable**:

- **Name:**. Name you want to refer this variable. e.g. IO1, Count.
- **ID (Hex):**. HEX number to refer to the specific Sensor Graph buffered output. If you use `Ouptut 10` on the Sensor Graph, then the ID would be `5010`. (`50` encodes the fact that it is a buffered output).
- **Description:** Add an optional description
- **VarType:** The variable Type. For a PODuino, always use `Default (u)`
- Check **Web Only**

After creating the Variable, click on **IO Configuration**:

- Leave **Multiply**, **Divide** and **Offset** with the defaults.
- **Input Units**: For a PODuino, use `Unit`.
- **Value Type**: For a PODuino, use `int` if you plan to upload Negative Numbers or `unsigned int` if you will ensure a positive number.
- **Output Units**: For a PODuino, use `Unit`.

While not needed in this case, the **Multiply**, **Divide** and **Offset** could be
used to make the cloud modify each value that it receives by `value * m / d + o`.

Note that you should NOT pick any other option as **VarType** as those may not work
as expected with your PODuino.

### 6. Review your Data Stream

After creating the variable, you should automatically get a Data Stream created
for you.
Use the left menu, under *Project Settings*, select *Data Streams*, and confirm
that you have a stream that matches your device and variable. Click on the *S* button to also confirmed the Variable Type and Input/Output settings were set correctly. They should match the same values as above. You can change the *Stream Label* as needed.

## IOTile Cloud API

The IOTile Cloud API is documented at [https://iotile.cloud/api/docs](https://iotile.cloud/api/docs).

### 1. API Access to the Data.

Using the API, you can access the Stream Data when ever needed. 

Use https://iotile.cloud/api/v1/data/?filter=s--0000-XXXX--0000-0000-0000--YYYY-5001 to show all data for a given stream. Replace `XXXX` with your Project ID and `YYYY` with your device ID. 

Or get the following APIs to get the list of your projects, devices or streams:

- **Projects**: https://iotile.cloud/api/v1/project/
- **Devices**: https://iotile.cloud/api/v1/device/
- **Streams**: https://iotile.cloud/api/v1/stream/

### 2. Available SDKs

Use the following two SDKs:

- **Angular**: This is an Angular V2+ wrapper. While not currently documented, tt is the actual API used by https://app.iotile.cloud. [ng-iotile-cloud](https://github.com/iotile/ng-iotile-cloud)
- **Python**: This is a stable Python 2.7+ or Python 3.4+ wrapper to the Rest API: [python_iotile_cloud](https://github.com/iotile/python_iotile_cloud).
- **Python**: The [IOTile Core Tools](https://github.com/iotile/coretools) also includes a similar **iotile_ext_cloud** providing some access to the IOTile Cloud.