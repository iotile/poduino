# poduino_remotelcd

This example illustrates how to use the Pod's connection as a way to transmit data over bluetooth seamlessly to complex devices interfacing with the Arduino, through a single Poduino device.

Tested LCD screen is an OSEPP LCD shield. Additional characters are provided in disp.h to allow for 2 block tall digits.

The following table describes how to display these custom characters (hex values)


| **Digit** | **Upper digit** | **Lower digit** |
|---	|---|---|
| 0 | `\x06` | `\x07` |
| 1 | `\x00` | `\x00` |
| 2 | `\x04` | `\x02` |
| 3 | `\x04` | `\x01` |
| 4 | `\x03` | `\x00` |
| 5 | `\xC9` | `\x01` |
| 6 | `\xC9` | `\xFC` |
| 7 | `\x05` | `\x00` |
| 8 | `\xFC` | `\xFC` |
| 9 | `\xFC` | `\x01` |
